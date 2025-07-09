//
// Created by gouzuang on 25-7-7.
//

#ifndef TEACHERHOMEPAGE_H
#define TEACHERHOMEPAGE_H

#include <QWidget>
#include <QString>
#include "view/bookingService/rentteacher.h"
#include "pch.h"
namespace view::homepage {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class teacherHomepage;
    }

    QT_END_NAMESPACE

    class teacherHomepage : public QWidget {
        Q_OBJECT

    public:
        explicit teacherHomepage(const QString &name, const QString &ID, QWidget *parent = nullptr);

        ~teacherHomepage() override;

    private slots:
        void on_borrowEquipmentButton_clicked(); // 器材借用
        void on_returnEquipmentButton_clicked(); // 器材归还
        void on_borrowHistoryButton_clicked(); // 我的借用日志
        void on_reportMaintenanceButton_clicked(); // 器材报修
        void on_editProfileButton_clicked(); // 修改个人信息
        void on_approvalButton_clicked(); // 审批
        void on_logoutButton_clicked(); // 退出登录
        void on_messageButton_clicked(); // 消息
        void on_Button_clicked();
        void on_Button2_clicked();
    private:
        Ui::teacherHomepage *ui;
        QString T_name; // 教师姓名
        QString T_ID; // 教师ID
        QPoint mouseOffset;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void setupUI(); // 初始化界面
    private:
        view::Order::RentTeacher* rentTeacher;

    };
} // view::homepage

#endif //TEACHERHOMEPAGE_H
