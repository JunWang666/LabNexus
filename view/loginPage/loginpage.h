//
// Created by gouzuang on 25-7-7.
//

#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include <QtSql/QSqlDatabase>

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

        int confirm_user(QString &ID_c, QString &password_c);

        int user;
        // 0无效
        // 001学生
        // 010老师
        // 100管理员
        // 还有的组合可能是110，即既是老师也是管理员,但是我想后面如果有需求再完善
    };
} // view::login

#endif //LOGINPAGE_H
