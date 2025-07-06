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
        write(buffer_.str(), currentLevel_);
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
