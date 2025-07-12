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
                void splashFinished();

        private:
            // 指向UI的指针不变，因为它是由uic工具在全局Ui命名空间生成的
            Ui::SplashScreen *ui;

            void startClosingTimer();
        };

    } // namespace SplashScreen
} // namespace view

#endif // SPLASHSCREEN_H