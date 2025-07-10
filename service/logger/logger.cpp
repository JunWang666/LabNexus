//
// Created by gouzuang on 25-7-6.
//

#include "logger.h"
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

namespace service {
    logger &logger::instance() {
        static logger instance;
        return instance;
    }

    logger::logger() : currentLevel_(LogLevel::INFO), minLevel_(LogLevel::DEBUG), enableStackTrace_(false) {
#if defined(_WIN32) || defined(_WIN64)
        // 设置控制台UTF-8编码
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        // 初始化符号处理器
        SymInitialize(GetCurrentProcess(), NULL, TRUE);
#endif
        
        // 输出初始化消息来测试控制台输出
        std::cout << "[LOGGER] Logger initialized successfully" << std::endl;
        std::cout.flush();
    }

    logger::~logger() {
        if (file_.is_open()) file_.close();
        if (dataFile_.is_open()) dataFile_.close();
#if defined(_WIN32) || defined(_WIN64)
        SymCleanup(GetCurrentProcess());
#endif
    }

    void logger::setLevel(LogLevel level) {
        std::lock_guard<std::mutex> lock(mtx_);
        minLevel_ = level;
    }

    void logger::setLogFile(const std::string &filename) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (file_.is_open()) file_.close();
        
        // 确保目录存在
        std::string path = filename;
        size_t lastSlash = path.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            std::string dir = path.substr(0, lastSlash);
            // 创建目录
#if defined(_WIN32) || defined(_WIN64)
            std::string createDirCmd = "mkdir \"" + dir + "\" 2>nul";
            system(createDirCmd.c_str());
#else
            std::string createDirCmd = "mkdir -p \"" + dir + "\"";
            system(createDirCmd.c_str());
#endif
        }
        
        file_.open(filename, std::ios::app);
        if (!file_.is_open()) {
            std::cout << "[LOGGER ERROR] Failed to open log file: " << filename << std::endl;
        } else {
            std::cout << "[LOGGER] Log file opened: " << filename << std::endl;
        }
    }

    void logger::setDataLogFile(const std::string &filename) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (dataFile_.is_open()) dataFile_.close();
        
        // 确保目录存在
        std::string path = filename;
        size_t lastSlash = path.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            std::string dir = path.substr(0, lastSlash);
            // 创建目录
#if defined(_WIN32) || defined(_WIN64)
            std::string createDirCmd = "mkdir \"" + dir + "\" 2>nul";
            system(createDirCmd.c_str());
#else
            std::string createDirCmd = "mkdir -p \"" + dir + "\"";
            system(createDirCmd.c_str());
#endif
        }
        
        dataFile_.open(filename, std::ios::app);
        if (!dataFile_.is_open()) {
            std::cout << "[LOGGER ERROR] Failed to open data log file: " << filename << std::endl;
        } else {
            std::cout << "[LOGGER] Data log file opened: " << filename << std::endl;
        }
    }

    void logger::enableStackTrace(bool enable) {
        std::lock_guard<std::mutex> lock(mtx_);
        enableStackTrace_ = enable;
    }

    logger &logger::operator<<(LogLevel level) {
        currentLevel_ = level;
        return *this;
    }

    logger &logger::operator<<(std::ostream & (*manip)(std::ostream &)) {
        if (manip == static_cast<std::ostream& (*)(std::ostream &)>(std::endl)) {
            flush();
        } else {
            buffer_ << manip;
        }
        return *this;
    }

    void logger::flush() {
        std::lock_guard<std::mutex> lock(mtx_);
        if (currentLevel_ < minLevel_) {
            buffer_.str("");
            buffer_.clear();
            return;
        }

        std::string message = buffer_.str();
        if (!message.empty()) {  // 只有在有内容时才输出
            write(message, currentLevel_);
        }

        buffer_.str("");
        buffer_.clear();
    }

    void logger::write(const std::string &msg, LogLevel level) {
        std::string levelStr;
        switch (level) {
            case LogLevel::DEBUG:
                levelStr = "[DEBUG]";
                break;
            case LogLevel::INFO:
                levelStr = "[INFO]";
                break;
            case LogLevel::WARN:
                levelStr = "[WARN]";
                break;
            case LogLevel::ERR:
                levelStr = "[ERROR]";
                break;
            case LogLevel::DATA:
                levelStr = "[DATA]";
                break;
        }
        
        std::string timestamp = getCurrentTimestamp();
        std::string out = timestamp + " " + levelStr + " " + msg;
        
        // 添加堆栈信息（仅对错误级别或显式启用时）
        // if (enableStackTrace_ || level == LogLevel::ERR) {
        //     std::string stack = getCallStack();
        //     if (!stack.empty()) {
        //         out += "\nCall Stack:\n" + stack;
        //     }
        // }
        
        out += "\n";
        
        // 输出到控制台，强制刷新
        std::cout << out;
        std::cout.flush();  // 强制刷新控制台输出
        
        if (level == LogLevel::DATA) {
            if (dataFile_.is_open()) dataFile_ << out, dataFile_.flush();
        } else {
            if (file_.is_open()) file_ << out, file_.flush();
        }
    }

    std::string logger::getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return oss.str();
    }

    std::string logger::getCallStack() {
        std::string result;
        
#if defined(_WIN32) || defined(_WIN64)
        // Windows 堆栈跟踪
        void* stack[64];
        WORD numberOfFrames = CaptureStackBackTrace(0, 64, stack, NULL);
        
        HANDLE process = GetCurrentProcess();
        SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
        symbol->MaxNameLen = 255;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        
        for (int i = 0; i < numberOfFrames; i++) {
            DWORD64 address = (DWORD64)(stack[i]);
            if (SymFromAddr(process, address, 0, symbol)) {
                result += "    " + std::string(symbol->Name) + "\n";
            } else {
                result += "    <unknown>\n";
            }
        }
        free(symbol);
#else
        // Linux/Unix 堆栈跟踪
        void* array[64];
        size_t size = backtrace(array, 64);
        char** strings = backtrace_symbols(array, size);
        
        if (strings != NULL) {
            for (size_t i = 0; i < size; i++) {
                std::string frame = strings[i];
                
                // 尝试解析函数名
                size_t begin = frame.find('(');
                size_t end = frame.find('+');
                if (begin != std::string::npos && end != std::string::npos && begin < end) {
                    std::string mangled = frame.substr(begin + 1, end - begin - 1);
                    int status;
                    char* demangled = abi::__cxa_demangle(mangled.c_str(), 0, 0, &status);
                    if (status == 0) {
                        result += "    " + std::string(demangled) + "\n";
                        free(demangled);
                    } else {
                        result += "    " + mangled + "\n";
                    }
                } else {
                    result += "    " + frame + "\n";
                }
            }
            free(strings);
        }
#endif
        
        return result;
    }
}
