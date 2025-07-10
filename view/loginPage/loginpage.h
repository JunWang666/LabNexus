//
// Created by gouzuang on 25-7-7.
//

#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include "pch.h"

namespace view::loginPage {
    class changePassword;
}

namespace view::login {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class loginPage;
    }

    QT_END_NAMESPACE

    class loginPage : public QWidget {
        Q_OBJECT

    public:
        explicit loginPage(QWidget *parent = nullptr);

        ~loginPage() override;

        QString name;
        QString ID;

    public slots:
        void showLoginPage();

    private slots:
        void on_loginButton_clicked();

        void on_registerButton_clicked();

        void on_changePasswordButton_clicked();

    private:
        Ui::loginPage *ui;
        QPoint mouseOffset;
        int confirm_user(QString &ID_c, QString &password_c);
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
    };
} // view::login

#endif //LOGINPAGE_H
