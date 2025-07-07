//
// Created by Nana7mi_ on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SendRent.h" resolved

#include "sendrent.h"
#include "ui_SendRent.h"

namespace view::Order {
SendRent::SendRent(QWidget *parent) :
    QDialog(parent), ui(new Ui::SendRent) {
    ui->setupUi(this);
}

SendRent::~SendRent() {
    delete ui;
}
} // view::Order
