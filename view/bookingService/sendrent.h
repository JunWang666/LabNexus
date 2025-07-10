//
// Created by Nana7mi_ on 25-7-7.
//

#ifndef SENDRENT_H
#define SENDRENT_H

#include <QDialog>
#include "module/data/data_mail.h"
#include "module/data/data_Booking.h"
namespace view::Order {
QT_BEGIN_NAMESPACE
namespace Ui { class SendRent; }
QT_END_NAMESPACE

class SendRent : public QDialog {
Q_OBJECT

public:
    explicit SendRent(const QString& name,const QString& id,const QString& devName,int equipmentId, int equipmentClassId,QWidget *parent = nullptr);
    explicit SendRent(const QString& name,const QString& id,QWidget* parent = nullptr);
    ~SendRent() override;
public slots:
    void on_btnSend_clicked();
private:
    Ui::SendRent *ui;
    // --- 新增成员变量来存储预选设备的ID ---
    int m_equipmentId = -1;
    int m_equipmentClassId = -1;
};
} // view::Order

#endif //SENDRENT_H
