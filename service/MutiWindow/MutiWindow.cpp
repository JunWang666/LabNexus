//
// Created by gouzuang on 25-7-9.
//

#include "MutiWindow.h"
#include <QApplication>
#include <QScreen>
#include <QDebug>

namespace service::MutiWindow {
    // 静态成员初始化
    WindowManager *WindowManager::s_instance = nullptr;

    WindowManager &WindowManager::instance() {
        if (!s_instance) {
            s_instance = new WindowManager();
        }
        return *s_instance;
    }

    WindowManager::WindowManager(QObject *parent) : QObject(parent) {
        log(LogLevel::DEBUG) << "窗口管理器初始化";
    }

    WindowManager::~WindowManager() {
        closeAllWindows();
        log(LogLevel::DEBUG) << "窗口管理器销毁";
    }

    WindowManager &manager() {
        return service::MutiWindow::WindowManager::instance();
    }

    void WindowManager::addWindow(const QString &windowId, QWidget *widget, const QString &title) {
        if (!widget) {
            log(LogLevel::WARN) << "尝试添加空窗口指针，窗口ID:" << windowId;
            return;
        }

        if (m_windows.contains(windowId)) {
            log(LogLevel::WARN) << "窗口ID已存在:" << windowId;
            return;
        }

        WindowInfo info(widget, title.isEmpty() ? windowId : title);
        m_windows[windowId] = info;

        // 连接窗口销毁信号，确保窗口被删除时能自动清理
        connect(widget, static_cast<void (QObject::*)(QObject *)>(&QObject::destroyed), this,
                &WindowManager::onWindowDestroyed);

        emit windowAdded(windowId);
        log(LogLevel::DEBUG) << "添加窗口:" << windowId << "标题:" << info.title;
    }

    void WindowManager::addWindow(QWidget *widget, const QString &title) {
        QString windowId = widget->objectName();
        if (windowId.isEmpty()) {
            windowId = QUuid::createUuid().toString();
            widget->setObjectName(windowId);
        } else {
            windowId += QUuid::createUuid().toString();
            widget->setObjectName(windowId);
        }
        addWindow(windowId, widget, title);
    }

    void WindowManager::showWindow(const QString &windowId) {
        if (!m_windows.contains(windowId)) {
            log(LogLevel::WARN) << "窗口不存在:" << windowId;
            return;
        }

        WindowInfo &info = m_windows[windowId];
        if (!info.widget) {
            log(LogLevel::WARN) << "窗口指针为空:" << windowId;
            return;
        }

        info.widget->show();
        info.widget->raise();
        info.widget->activateWindow();
        info.isVisible = true;

        emit windowShown(windowId);
        log(LogLevel::DEBUG) << "显示窗口:" << windowId;
    }

    void WindowManager::hideWindow(const QString &windowId) {
        if (!m_windows.contains(windowId)) {
            log(LogLevel::WARN) << "窗口不存在:" << windowId;
            return;
        }

        WindowInfo &info = m_windows[windowId];
        if (!info.widget) {
            return;
        }

        info.widget->hide();
        info.isVisible = false;

        emit windowHidden(windowId);
        log(LogLevel::DEBUG) << "隐藏窗口:" << windowId;
    }

    void WindowManager::removeWindow(const QString &windowId) {
        if (!m_windows.contains(windowId)) {
            return;
        }

        WindowInfo &info = m_windows[windowId];
        if (info.widget) {
            // 断开连接
            disconnect(info.widget, static_cast<void (QObject::*)(QObject *)>(&QObject::destroyed), this,
                       &WindowManager::onWindowDestroyed);
            // 不删除窗口，只是移除管理
            info.widget = nullptr;
        }

        m_windows.remove(windowId);
        emit windowRemoved(windowId);
        log(LogLevel::DEBUG) << "移除窗口:" << windowId;
    }

    void WindowManager::removeAllWindows() {
        QStringList windowIds = m_windows.keys();
        for (const QString &id: windowIds) {
            removeWindow(id);
        }
    }

    QWidget *WindowManager::getWindow(const QString &windowId) {
        if (m_windows.contains(windowId)) {
            return m_windows[windowId].widget;
        }
        return nullptr;
    }

    bool WindowManager::hasWindow(const QString &windowId) {
        return m_windows.contains(windowId) && m_windows[windowId].widget != nullptr;
    }

    bool WindowManager::isWindowVisible(const QString &windowId) {
        return hasWindow(windowId) && m_windows[windowId].isVisible && m_windows[windowId].widget->isVisible();
    }

    QStringList WindowManager::getWindowIds() {
        QStringList ids;
        for (auto it = m_windows.begin(); it != m_windows.end(); ++it) {
            if (it.value().widget != nullptr) {
                ids.append(it.key());
            }
        }
        return ids;
    }

    void WindowManager::bringToFront(const QString &windowId) {
        if (!hasWindow(windowId)) {
            return;
        }

        QWidget *widget = m_windows[windowId].widget;
        widget->raise();
        widget->activateWindow();
        log(LogLevel::DEBUG) << "置顶窗口:" << windowId;
    }

    void WindowManager::centerWindow(const QString &windowId) {
        if (!hasWindow(windowId)) {
            return;
        }

        QWidget *widget = m_windows[windowId].widget;
        QScreen *screen = QApplication::primaryScreen();
        if (screen) {
            QRect screenGeometry = screen->geometry();
            int x = (screenGeometry.width() - widget->width()) / 2;
            int y = (screenGeometry.height() - widget->height()) / 2;
            widget->move(x, y);
            log(LogLevel::DEBUG) << "居中窗口:" << windowId;
        }
    }

    void WindowManager::hideAllWindows() {
        for (auto it = m_windows.begin(); it != m_windows.end(); ++it) {
            if (it.value().widget && it.value().isVisible) {
                hideWindow(it.key());
            }
        }
    }

    void WindowManager::showAllWindows() {
        for (auto it = m_windows.begin(); it != m_windows.end(); ++it) {
            if (it.value().widget && !it.value().isVisible) {
                showWindow(it.key());
            }
        }
    }

    void WindowManager::closeAllWindows() {
        for (auto it = m_windows.begin(); it != m_windows.end(); ++it) {
            if (it.value().widget) {
                // 断开连接
                disconnect(it.value().widget, static_cast<void (QObject::*)(QObject *)>(&QObject::destroyed), this,
                           &WindowManager::onWindowDestroyed);
                // 关闭并删除窗口
                it.value().widget->close();
                it.value().widget->deleteLater();
            }
        }
        m_windows.clear();
        log(LogLevel::DEBUG) << "关闭所有窗口";
    }

    void WindowManager::onWindowDestroyed(QObject *obj) {
        // 查找并清理被销毁的窗口
        for (auto it = m_windows.begin(); it != m_windows.end(); ++it) {
            if (it.value().widget == obj) {
                const auto &windowId = it.key();
                m_windows.remove(windowId);
                emit windowRemoved(windowId);
                log(LogLevel::DEBUG) << "窗口被销毁，自动清理:" << windowId;
                break;
            }
        }
    }
}
