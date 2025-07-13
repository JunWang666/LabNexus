//
// Created by gouzuang on 25-7-13.
//

#ifndef CHANGEPASSWORDADMIN_H
#define CHANGEPASSWORDADMIN_H

#include <QWidget>

namespace view::RegisterCenter {
QT_BEGIN_NAMESPACE
namespace Ui { class ChangePasswordAdmin; }
QT_END_NAMESPACE

class ChangePasswordAdmin : public QWidget {
Q_OBJECT

public:
    explicit ChangePasswordAdmin(QWidget *parent = nullptr);
    ~ChangePasswordAdmin() override;

public slots:
    void on_UpdateButton_clicked();

private:
    Ui::ChangePasswordAdmin *ui;
    void show_message(QString message, int timeout = 10000);

};
} // view::RegisterCenter

#endif //CHANGEPASSWORDADMIN_H
