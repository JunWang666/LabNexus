//
// Created by gouzuang on 25-7-7.
//

#ifndef ADMINISTRATORHOMEPAGE_H
#define ADMINISTRATORHOMEPAGE_H

#include "pch.h"
#include "view/bookingService/apply.h"
#include "view/bookingService/managercheck.h"

namespace view::homepage {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class administratorHomepage;
    }

    QT_END_NAMESPACE

    class administratorHomepage : public QWidget {
        Q_OBJECT

    public:
        explicit administratorHomepage(const QString &name, const QString &ID, QWidget *parent = nullptr);

        ~administratorHomepage() override;

    public slots:
        void on_equipmentManageButton_clicked(); // 器材管理
        void on_approvalButton_clicked(); // 审批
        void on_userManageButton_clicked(); // 用户管理
        void on_systemManageButton_clicked(); // 系统管理
        void on_editProfileButton_clicked(); // 修改个人信息
        void on_logoutButton_clicked(); // 退出登录
        void on_messageButton_clicked(); // 消息
        void on_Button_clicked();
        void on_Button2_clicked();
        void on_EquipmentClassButton_clicked();
        void on_RegisterControlButton_clicked();
    private:
        Ui::administratorHomepage *ui;
        QString A_name; // 管理员姓名
        QString A_ID; // 管理员ID
        QPoint mouseOffset;
        view::Order::ManagerCheck *orderCheck;

        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void setupUI(); // 初始化界面
    };
} // view::homepage

#endif //ADMINISTRATORHOMEPAGE_H
