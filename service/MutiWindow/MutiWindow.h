//
// Created by gouzuang on 25-7-9.
//

#ifndef MUTIWINDOW_H
#define MUTIWINDOW_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include <QString>
#include <QList>
#include <memory>

namespace service::MutiWindow {
    // 窗口信息结构
    struct WindowInfo {
        QWidget* widget;
        QString title;
        bool isVisible;

        WindowInfo() : widget(nullptr), isVisible(false) {}
        WindowInfo(QWidget* w, QString t) : widget(w), title(std::move(t)), isVisible(false) {}
    };

    class WindowManager : public QObject {
        Q_OBJECT

    public:
        static WindowManager& instance();

        // 禁用拷贝构造函数和赋值操作符
        WindowManager(const WindowManager&) = delete;
        WindowManager& operator=(const WindowManager&) = delete;

        // 基本窗口管理
        void addWindow(const QString& windowId, QWidget* widget, const QString& title = "");
        void addWindow(QWidget* widget, const QString& title = "");
        void showWindow(const QString& windowId);
        void hideWindow(const QString& windowId);
        void removeWindow(const QString& windowId);
        void removeAllWindows();

        // 窗口查询
        QWidget* getWindow(const QString& windowId);
        bool hasWindow(const QString& windowId);
        bool isWindowVisible(const QString& windowId);
        QStringList getWindowIds();

        // 窗口操作
        void bringToFront(const QString& windowId);
        void centerWindow(const QString& windowId);

        // 批量操作
        void hideAllWindows();
        void showAllWindows();
        void closeAllWindows();

        signals:
            void windowAdded(const QString& windowId);
            void windowRemoved(const QString& windowId);
            void windowShown(const QString& windowId);
            void windowHidden(const QString& windowId);

    private slots:
        void onWindowDestroyed(QObject* obj);

    private:
        explicit WindowManager(QObject* parent = nullptr);
        ~WindowManager() override;

        // 成员变量
        QMap<QString, WindowInfo> m_windows;

        static WindowManager* s_instance;
    };

    WindowManager& manager();
}

#endif //MUTIWINDOW_H
