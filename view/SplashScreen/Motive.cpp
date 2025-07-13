// view/SplashScreen/splashscreen.cpp
#include "Motive.h"
#include "ui_Motive.h"
#include <QUrl>
#include <QTimer>

namespace view {
    namespace SplashScreen {
        Motive::Motive(QWidget *parent) :
            QWidget(parent),
            ui(new Ui::SplashScreen)
        {
            ui->setupUi(this);

            service::style::setMica(this);

            // 设置窗口属性
            setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            setAttribute(Qt::WA_TranslucentBackground);

            // 加载QML内容
            ui->quickWidget->setSource(QUrl("qrc:/SplashScreen.qml"));
            // 可以选择在QML加载完成后进行一些操作
            // connect(ui->quickWidget->engine(), &QQmlApplicationEngine::objectCreated, this, [this](QObject* obj, const QUrl& url) {
            //     if (obj && url == QUrl("qrc:/SplashScreen.qml")) {
            //         qDebug() << "SplashScreen QML loaded.";
            //         // 这里可以根据需要触发关闭逻辑或动画
            //     }
            // });


            // 居中显示
            QScreen *screen = QGuiApplication::primaryScreen();
            QRect screenGeometry = screen->geometry();
            // 假设你的SplashScreen在.ui文件中已经设置了固定大小，
            // 或者你可以在这里根据QML内容动态调整quickWidget的大小，进而影响this->width()
            int x = (screenGeometry.width() - this->width()) / 2;
            int y = (screenGeometry.height() - this->height()) / 2;
            this->move(x, y);

        }

        Motive::~Motive()
        {
            delete ui;
        }

        void Motive::closeSplashScreen()
        {
            // 停止定时器，避免重复触发
            closingTimer->stop();

            // 隐藏并关闭启动屏幕
            this->hide(); // 可以先隐藏
            emit splashScreenClosed(); // 发出信号通知主程序
            this->close(); // 调用QWidget的close()方法销毁窗口
        }

    } // namespace SplashScreen
} // namespace view