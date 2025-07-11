//
// Created by gouzuang on 25-7-7.
//

#ifndef STUDENTHOMEPAGE_H
#define STUDENTHOMEPAGE_H

#include "pch.h"
#include "view/bookingService/apply.h"
#include "view/bookingService/rent.h"

namespace view::homepage {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class studentHomepage;
    }

    QT_END_NAMESPACE

    class studentHomepage : public QWidget {
        Q_OBJECT

    public:
        explicit studentHomepage(const QString &name, const QString &ID, QWidget *parent = nullptr);

        ~studentHomepage() override;

    private slots:
        void on_borrowEquipmentButton_clicked(); // 器材借用
        void on_returnEquipmentButton_clicked(); // 器材归还
        void on_borrowHistoryButton_clicked(); // 我的借用日志
        void on_reportMaintenanceButton_clicked(); // 器材报修
        void on_editProfileButton_clicked(); // 修改个人信息
        void on_logoutButton_clicked(); // 退出登录
        void on_messageButton_clicked(); // 消息
        void on_Button_clicked();
        void on_Button2_clicked();
    private:
        Ui::studentHomepage *ui;
        QString S_name; // 学生姓名
        QString S_ID; // 学生ID
        void setupUI(); // 初始化界面
        QPoint mouseOffset;
        view::Order::Rent *rent = nullptr; // 器材借用对象
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
    };
} // view::homepage

#endif //STUDENTHOMEPAGE_H
