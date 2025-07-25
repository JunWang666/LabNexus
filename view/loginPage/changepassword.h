//
// Created by gouzuang on 25-7-7.
//

#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include "pch.h"

namespace view::loginPage {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class changePassword;
    }

    QT_END_NAMESPACE

    class changePassword : public QWidget {
        Q_OBJECT

    public:
        explicit changePassword(QWidget *parent = nullptr);
        ~changePassword() override;
        void setData(const QString& IDD);

    signals:
        void backToLogin();

    private slots:
        void onConfirmChangePassword();

        void onCancel();

    private:
        Ui::changePassword *ui;
        int change_name;//0就是不变，1就是要变
        // 成员变量
        QString userId;
        QString prePassword;
        QString newPassword;
        QString userName;
        QPoint mouseOffset;
        // 私有方法
        bool isPasswordValid(const QString &password);

        void setupConnections();

        void setupPasswordFields();

        bool validateUserCredentials(const QString &id, const QString &name, const QString &oldPassword);

        bool updateUserPassword(const QString &id, const QString &newPassword);
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
    };
} // view::loginPage

#endif //CHANGEPASSWORD_H
