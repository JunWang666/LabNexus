//
// Created by gouzuang on 25-7-6.
//

#ifndef LOG_H
#define LOG_H

#include <string>
#include <sstream>
#include <fstream>
#include <mutex>
#include <iostream>
#include <memory>
#include <QString>

namespace service {
    enum class LogLevel {
        DEBUG,
        INFO,
        WARN,
        ERR,
        DATA
    };

    class logger {
    public:
        static logger &instance();

        void setLevel(LogLevel level);

        void setLogFile(const std::string &filename);
        void setDataLogFile(const std::string &filename);
        void enableConsole(bool enable);

        logger &operator<<(LogLevel level);

        template<typename T>
        logger &operator<<(const T &msg) {
            buffer_ << msg;
            return *this;
        }

        logger &operator<<(std::ostream & (*manip)(std::ostream &));

        // Specialized template for QString
        logger &operator<<(const QString &msg) {
            buffer_ << msg.toStdString();
            return *this;
        }

        void flush();

    private:
        logger();

        ~logger();

        logger(const logger &) = delete;

        logger &operator=(const logger &) = delete;

        void write(const std::string &msg, LogLevel level);

        std::ostringstream buffer_;
        LogLevel currentLevel_;
        LogLevel minLevel_;
        std::ofstream file_;
        std::ofstream dataFile_;
        bool console_;
        std::mutex mtx_;
    };

    class LogStream {
    public:
        LogStream(LogLevel level = LogLevel::INFO);
        ~LogStream();

        LogStream &operator<<(LogLevel level);

        template<typename T>
        LogStream &operator<<(const T &msg) {
            buffer_ << msg;
            return *this;
        }

        // Specialized template for QString
        LogStream &operator<<(const QString &msg) {
            buffer_ << msg.toStdString();
            return *this;
        }

        LogStream &operator<<(std::ostream & (*manip)(std::ostream &));

    private:
        LogLevel level_;
        std::ostringstream buffer_;
    };

    LogStream log(LogLevel level = LogLevel::INFO);
}

using service::LogLevel;
using service::log;

#endif //LOG_H
