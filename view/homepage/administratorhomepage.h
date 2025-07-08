//
// Created by gouzuang on 25-7-7.
//

#ifndef ADMINISTRATORHOMEPAGE_H
#define ADMINISTRATORHOMEPAGE_H

#include <QWidget>
#include <QString>

#include "view/bookingService/managercheck.h"
#include "view/bookingService/rent.h"

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

    private slots:
        void on_equipmentManageButton_clicked(); // 器材管理
        void on_approvalButton_clicked(); // 审批
        void on_userManageButton_clicked(); // 用户管理
        void on_systemManageButton_clicked(); // 系统管理
        void on_editProfileButton_clicked(); // 修改个人信息
        void on_logoutButton_clicked(); // 退出登录
        void on_messageButton_clicked(); // 消息

    private:
        Ui::administratorHomepage *ui;
        QString A_name; // 管理员姓名
        QString A_ID; // 管理员ID

        void setupUI(); // 初始化界面

    private:
        view::Order::ManagerCheck* orderCheck;
    };
} // view::homepage

#endif //ADMINISTRATORHOMEPAGE_H
