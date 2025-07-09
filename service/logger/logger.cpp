//
// Created by gouzuang on 25-7-6.
//

#include "logger.h"
#include <regex>
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

    logger::logger() : currentLevel_(LogLevel::INFO), minLevel_(LogLevel::DEBUG), console_(true) {
    }

    logger::~logger() {
        if (file_.is_open()) file_.close();
        if (dataFile_.is_open()) dataFile_.close();
    }

    void logger::setLevel(LogLevel level) {
        std::lock_guard<std::mutex> lock(mtx_);
        minLevel_ = level;
    }

    void logger::setLogFile(const std::string &filename) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (file_.is_open()) file_.close();
        file_.open(filename, std::ios::app);
    }

    void logger::setDataLogFile(const std::string &filename) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (dataFile_.is_open()) dataFile_.close();
        dataFile_.open(filename, std::ios::app);
    }

    void logger::enableConsole(bool enable) {
        std::lock_guard<std::mutex> lock(mtx_);
        console_ = enable;
    }

    // 过滤器相关方法实现
    void logger::setFilter(const LogFilter& filter) {
        std::lock_guard<std::mutex> lock(mtx_);
        filter_ = filter;
    }

    void logger::clearFilter() {
        std::lock_guard<std::mutex> lock(mtx_);
        filter_ = nullptr;
    }

    void logger::addKeywordFilter(const std::string& keyword, bool exclude) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (exclude) {
            excludeKeywords_.push_back(keyword);
        } else {
            includeKeywords_.push_back(keyword);
        }
    }

    void logger::addRegexFilter(const std::string& pattern, bool exclude) {
        std::lock_guard<std::mutex> lock(mtx_);
        try {
            std::regex regexPattern(pattern);
            LogFilter regexFilter = [pattern, exclude](const std::string& msg, LogLevel level) -> bool {
                std::regex regex(pattern);
                bool matches = std::regex_search(msg, regex);
                return exclude ? !matches : matches;
            };

            if (filter_) {
                // 如果已经有过滤器，组合使用
                LogFilter oldFilter = filter_;
                filter_ = [oldFilter, regexFilter](const std::string& msg, LogLevel level) -> bool {
                    return oldFilter(msg, level) && regexFilter(msg, level);
                };
            } else {
                filter_ = regexFilter;
            }
        } catch (const std::regex_error& e) {
            // 忽略无效的正则表达式
        }
    }

    void logger::clearKeywordFilters() {
        std::lock_guard<std::mutex> lock(mtx_);
        includeKeywords_.clear();
        excludeKeywords_.clear();
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
        if (shouldLog(message, currentLevel_)) {
            write(message, currentLevel_);
        }

        buffer_.str("");
        buffer_.clear();
    }

    bool logger::shouldLog(const std::string &msg, LogLevel level) {
        // 应用自定义过滤器
        if (filter_ && !filter_(msg, level)) {
            return false;
        }

        // 应用关键词过滤器
        if (!includeKeywords_.empty()) {
            bool found = false;
            for (const auto& keyword : includeKeywords_) {
                if (msg.find(keyword) != std::string::npos) {
                    found = true;
                    break;
                }
            }
            if (!found) return false;
        }

        // 应用排除关键词过滤器
        for (const auto& keyword : excludeKeywords_) {
            if (msg.find(keyword) != std::string::npos) {
                return false;
            }
        }

        return true;
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
        std::string out = levelStr + " " + msg + "\n";
#if defined(_WIN32) || defined(_WIN64)
        if (console_) {
            SetConsoleOutputCP(CP_UTF8);
            std::cout << out;
        }
#else
        if (console_) {
            std::cout << out;
        }
#endif
        if (level == LogLevel::DATA) {
            if (dataFile_.is_open()) dataFile_ << out, dataFile_.flush();
        } else {
            if (file_.is_open()) file_ << out, file_.flush();
        }
    }

    LogStream::LogStream(LogLevel level) : level_(level) {
    }

    LogStream::~LogStream() {
        logger::instance() << level_ << buffer_.str() << std::endl;
    }

    LogStream &LogStream::operator<<(LogLevel level) {
        level_ = level;
        return *this;
    }

    LogStream &LogStream::operator<<(std::ostream &(*manip)(std::ostream &)) {
        if (manip == static_cast<std::ostream &(*)(std::ostream &)>(std::endl)) {
            buffer_ << manip;
        }
        return *this;
    }

    LogStream log(LogLevel level) {
        return {level};
    }
}
