// view/SplashScreen/splashscreen.h
#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include <QScreen>
#include <QGuiApplication>


namespace view {
    namespace SplashScreen {
        QT_BEGIN_NAMESPACE

        namespace Ui {
            class Motive;
        }

        QT_END_NAMESPACE

        class Motive : public QWidget {
            Q_OBJECT

        public:
            explicit Motive(QWidget *parent = nullptr);

            ~Motive();

        signals:
            void splashScreenClosed();

        private slots:
            void closeSplashScreen();

        private:
            Ui::Motive *ui;
            QTimer *closingTimer; // 定时器，用于延迟关闭
        };
    } // namespace SplashScreen
} // namespace view

#endif // SPLASHSCREEN_H
