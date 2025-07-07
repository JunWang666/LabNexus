//
// Created by Nana7mi_ on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ManagerCheck.h" resolved

#include "managercheck.h"
#include "ui_ManagerCheck.h"

namespace view::Order {
ManagerCheck::ManagerCheck(QWidget *parent) :
    QWidget(parent), ui(new Ui::ManagerCheck) {
    ui->setupUi(this);
}

ManagerCheck::~ManagerCheck() {
    delete ui;
}

void ManagerCheck::on_btnclose_clicked()
{
    this->close();
}

} // view::Order
