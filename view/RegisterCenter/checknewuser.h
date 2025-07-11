//
// Created by gouzuang on 25-7-11.
//

#ifndef CHECKNEWUSER_H
#define CHECKNEWUSER_H

#include <QWidget>

namespace view::RegisterCenter {
QT_BEGIN_NAMESPACE
namespace Ui { class CheckNewUser; }
QT_END_NAMESPACE

class CheckNewUser : public QWidget {
Q_OBJECT

public:
    explicit CheckNewUser(QWidget *parent = nullptr);
    ~CheckNewUser() override;

private:
    Ui::CheckNewUser *ui;
};
} // view::RegisterCenter

#endif //CHECKNEWUSER_H
