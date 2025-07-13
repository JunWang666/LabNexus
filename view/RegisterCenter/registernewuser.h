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

    private:
        Ui::RegisterNewUser *ui;
    };
} // view::RegisterCenter

#endif //REGISTERNEWUSER_H
