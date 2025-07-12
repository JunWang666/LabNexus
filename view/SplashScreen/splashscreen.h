// view/SplashScreen/splashscreen.h
#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include <QScreen>
#include <QGuiApplication>

QT_BEGIN_NAMESPACE
namespace Ui { class SplashScreen; }
QT_END_NAMESPACE

namespace view {
    namespace SplashScreen {

        class SplashScreen : public QWidget
        {
            Q_OBJECT

public:
            explicit SplashScreen(QWidget *parent = nullptr);
            ~SplashScreen();

            signals:
                void splashScreenClosed(); // 定义一个信号，用于通知主程序启动屏幕已关闭

        private slots:
            void closeSplashScreen(); // Slot来处理关闭逻辑

        private:
            Ui::SplashScreen *ui;
            QTimer *closingTimer; // 定时器，用于延迟关闭
        };

    } // namespace SplashScreen
} // namespace view

#endif // SPLASHSCREEN_H