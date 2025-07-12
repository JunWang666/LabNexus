// view/SplashScreen/splashscreen.cpp
#include "splashscreen.h"
#include "ui_splashscreen.h"
#include <QUrl>
#include <QTimer>

namespace view {
    namespace SplashScreen {

        // 构造函数的实现
        SplashScreen::SplashScreen(QWidget *parent) :
            QWidget(parent),
            ui(new Ui::SplashScreen) // Ui::SplashScreen 不在我们的命名空间内，所以写法不变
        {
            ui->setupUi(this);

            setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            setAttribute(Qt::WA_TranslucentBackground);

            ui->quickWidget->setSource(QUrl("qrc:/SplashScreen.qml"));

            QScreen *screen = QGuiApplication::primaryScreen();
            QRect screenGeometry = screen->geometry();
            int x = (screenGeometry.width() - this->width()) / 2;
            int y = (screenGeometry.height() - this->height()) / 2;
            this->move(x, y);

            startClosingTimer();
        }

        // 析构函数的实现
        SplashScreen::~SplashScreen()
        {
            delete ui;
        }

        // 成员函数的实现
        void SplashScreen::startClosingTimer()
        {
            QTimer::singleShot(3000, this, [this](){
                emit splashFinished();
                this->close();
            });
        }

    } // namespace SplashScreen
} // namespace view