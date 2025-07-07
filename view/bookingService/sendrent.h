//
// Created by Nana7mi_ on 25-7-7.
//

#ifndef SENDRENT_H
#define SENDRENT_H

#include <QDialog>

namespace view::Order {
QT_BEGIN_NAMESPACE
namespace Ui { class SendRent; }
QT_END_NAMESPACE

class SendRent : public QDialog {
Q_OBJECT

public:
    explicit SendRent(QWidget *parent = nullptr);
    ~SendRent() override;

private:
    Ui::SendRent *ui;
};
} // view::Order

#endif //SENDRENT_H
