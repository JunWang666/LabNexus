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
#include <chrono>
#include <iomanip>
#include <expected>
#include <vector>
#include <map>
#include <unordered_map>
#include <QString>
#include <QList>
#include <QMap>
#include <QHash>
#include <type_traits>
#include <QtDebug>
#include <QByteArray>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#else
#include <execinfo.h>
#include <cxxabi.h>
#endif

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
        void enableStackTrace(bool enable = true);

        logger &operator<<(LogLevel level);

        // 特别支持 std::expected
        template<typename T, typename E>
        logger& operator<<(const std::expected<T, E>& exp) {
            if (exp.has_value()) {
                buffer_ << "[Expected: ";
                *this << exp.value();
                buffer_ << "]";
            } else {
                buffer_ << "[Expected Error: ";
                *this << exp.error();
                buffer_ << "]";
            }
            return *this;
        }

        // 特别支持 QList
        template<typename T>
        logger& operator<<(const QList<T>& list) {
            buffer_ << "[";
            for (int i = 0; i < list.size(); ++i) {
                if (i > 0) buffer_ << ", ";
                *this << list[i];
            }
            buffer_ << "]";
            return *this;
        }

        // 特别支持 std::vector
        template<typename T>
        logger& operator<<(const std::vector<T>& vec) {
            buffer_ << "[";
            for (size_t i = 0; i < vec.size(); ++i) {
                if (i > 0) buffer_ << ", ";
                *this << vec[i];
            }
            buffer_ << "]";
            return *this;
        }

        // 特别支持 QMap
        template<typename K, typename V>
        logger& operator<<(const QMap<K, V>& map) {
            buffer_ << "{";
            bool first = true;
            for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
                if (!first) buffer_ << ", ";
                first = false;
                *this << it.key();
                buffer_ << ": ";
                *this << it.value();
            }
            buffer_ << "}";
            return *this;
        }

        // 特别支持 QHash
        template<typename K, typename V>
        logger& operator<<(const QHash<K, V>& hash) {
            buffer_ << "{";
            bool first = true;
            for (auto it = hash.constBegin(); it != hash.constEnd(); ++it) {
                if (!first) buffer_ << ", ";
                first = false;
                *this << it.key();
                buffer_ << ": ";
                *this << it.value();
            }
            buffer_ << "}";
            return *this;
        }

        // 特别支持 std::map
        template<typename K, typename V>
        logger& operator<<(const std::map<K, V>& map) {
            buffer_ << "{";
            bool first = true;
            for (const auto& pair : map) {
                if (!first) buffer_ << ", ";
                first = false;
                *this << pair.first;
                buffer_ << ": ";
                *this << pair.second;
            }
            buffer_ << "}";
            return *this;
        }

        // 特别支持 std::unordered_map
        template<typename K, typename V>
        logger& operator<<(const std::unordered_map<K, V>& map) {
            buffer_ << "{";
            bool first = true;
            for (const auto& pair : map) {
                if (!first) buffer_ << ", ";
                first = false;
                *this << pair.first;
                buffer_ << ": ";
                *this << pair.second;
            }
            buffer_ << "}";
            return *this;
        }

        // 支持 std::ostream 可流化的类型
        template<typename T>
        typename std::enable_if_t<
            std::is_same_v<decltype(std::declval<std::ostream&>() << std::declval<T>()), std::ostream&>,
            logger&
        > operator<<(const T &msg) {
            buffer_ << msg;
            return *this;
        }

        // 支持 QDebug 可流化的类型（排除已支持的 std::ostream 类型）
        template<typename T>
        typename std::enable_if_t<
            !std::is_same_v<decltype(std::declval<std::ostream&>() << std::declval<T>()), std::ostream&> &&
            std::is_same_v<decltype(std::declval<QDebug&>() << std::declval<T>()), QDebug&>,
            logger&
        > operator<<(const T &msg) {
            QByteArray ba;
            QDebug db(&ba);
            db << msg;
            buffer_ << ba.constData();
            return *this;
        }

        // 支持有 toString() 方法的类型
        template<typename T>
        typename std::enable_if_t<
            !std::is_same_v<decltype(std::declval<std::ostream&>() << std::declval<T>()), std::ostream&> &&
            !std::is_same_v<decltype(std::declval<QDebug&>() << std::declval<T>()), QDebug&> &&
            std::is_same_v<decltype(std::declval<T>().toString()), std::string>,
            logger&
        > operator<<(const T &msg) {
            buffer_ << msg.toString();
            return *this;
        }

        // 支持有 what() 方法的类型（如异常类）
        template<typename T>
        typename std::enable_if_t<
            !std::is_same_v<decltype(std::declval<std::ostream&>() << std::declval<T>()), std::ostream&> &&
            !std::is_same_v<decltype(std::declval<QDebug&>() << std::declval<T>()), QDebug&> &&
            !std::is_same_v<decltype(std::declval<T>().toString()), std::string> &&
            std::is_same_v<decltype(std::declval<T>().what()), const char*>,
            logger&
        > operator<<(const T &msg) {
            buffer_ << msg.what();
            return *this;
        }

        // 兜底方案：尝试转换为字符串
        template<typename T>
        typename std::enable_if_t<
            !std::is_same_v<decltype(std::declval<std::ostream&>() << std::declval<T>()), std::ostream&> &&
            !std::is_same_v<decltype(std::declval<QDebug&>() << std::declval<T>()), QDebug&> &&
            !std::is_same_v<decltype(std::declval<T>().toString()), std::string> &&
            !std::is_same_v<decltype(std::declval<T>().what()), const char*>,
            logger&
        > operator<<(const T &msg) {
            buffer_ << "[Unsupported type: " << typeid(T).name() << "]";
            return *this;
        }

        logger &operator<<(std::ostream & (*manip)(std::ostream &));

        logger &operator<<(const QString &msg) {
            buffer_ << msg.toStdString();
            return *this;
        }

        void flush(); // 添加公共flush方法声明

    private:
        logger();

        ~logger();

        logger(const logger &) = delete;

        logger &operator=(const logger &) = delete;

        void write(const std::string &msg, LogLevel level);
        std::string getCurrentTimestamp();
        std::string getCallStack();

        std::ostringstream buffer_;
        LogLevel currentLevel_;
        LogLevel minLevel_;
        std::ofstream file_;
        std::ofstream dataFile_;
        std::mutex mtx_;
        bool enableStackTrace_;
    };

    // 日志包装器，用于自动flush
    class LoggerWrapper {
    public:
        LoggerWrapper(logger& log_instance, LogLevel level)
            : logger_(log_instance), level_(level) {
            logger_ << level;
        }

        ~LoggerWrapper() {
            logger_.flush();  // 析构时自动flush
        }

        template<typename T>
        LoggerWrapper& operator<<(const T& msg) {
            logger_ << msg;
            return *this;
        }

        LoggerWrapper& operator<<(std::ostream& (*manip)(std::ostream&)) {
            if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
                logger_.flush();
            } else {
                logger_ << manip;
            }
            return *this;
        }

    private:
        logger& logger_;
        LogLevel level_;
    };

    template<typename T>
    void log(const T &msg, LogLevel level = LogLevel::INFO) {
        auto& instance = logger::instance();
        instance << level << msg;
        instance.flush();  // 自动换行，不需要手动添加std::endl
    }

    inline LoggerWrapper log(LogLevel level = LogLevel::INFO) {
        return LoggerWrapper(logger::instance(), level);
    }
}


using service::LogLevel;
using service::log;

#endif //LOG_H
