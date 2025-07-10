//
// Created by Nana7mi_ on 25-7-7.
//

#ifndef SENDRENT_H
#define SENDRENT_H

#include <QDialog>
#include "module/data/data_mail.h"
namespace view::Order {
QT_BEGIN_NAMESPACE
namespace Ui { class SendRent; }
QT_END_NAMESPACE

class SendRent : public QDialog {
Q_OBJECT

public:
    explicit SendRent(const QString& name,const QString& id,const QString& devName,QWidget *parent = nullptr);
    explicit SendRent(const QString& name,const QString& id,QWidget* parent = nullptr);
    ~SendRent() override;
public slots:
    void on_btnSend_clicked();
private:
    Ui::SendRent *ui;
};
} // view::Order

#endif //SENDRENT_H
