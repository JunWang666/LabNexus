//
// Created by gouzuang on 25-7-7.
//

#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QWidget>
#include <QLineEdit>
#include <QString>

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

    signals:
        void backToLogin();

    private slots:
        void onConfirmChangePassword();

        void onCancel();

    private:
        Ui::changePassword *ui;

        // 成员变量
        QString userId;
        QString prePassword;
        QString newPassword;
        QString userName;

        // 私有方法
        bool isPasswordValid(const QString &password);

        void setupConnections();

        void setupPasswordFields();

        bool validateUserCredentials(const QString &id, const QString &name, const QString &oldPassword);

        bool updateUserPassword(const QString &id, const QString &newPassword);
    };
} // view::loginPage

#endif //CHANGEPASSWORD_H
