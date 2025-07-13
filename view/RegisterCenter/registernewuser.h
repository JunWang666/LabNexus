//
// Created by gouzuang on 25-7-13.
//

#ifndef REGISTERNEWUSER_H
#define REGISTERNEWUSER_H

#include <QWidget>

namespace view::RegisterCenter {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class RegisterNewUser;
    }

    QT_END_NAMESPACE

    class RegisterNewUser : public QWidget {
        Q_OBJECT

    public:
        explicit RegisterNewUser(QWidget *parent = nullptr);

        ~RegisterNewUser() override;

    public slots:
        void on_UpdateButton_clicked();

        void checkVaild();

        void on_NewUser_textChanged();

        void on_UserGroup_currentTextChanged(const QString &arg1);

    private:
        Ui::RegisterNewUser *ui;

        void show_message(QString message, int timeout = 10000);
    };
} // view::RegisterCenter

#endif //REGISTERNEWUSER_H
