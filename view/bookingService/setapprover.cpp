//
// Created by Nana7mi_ on 25-7-10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SetApprover.h" resolved

#include "setapprover.h"
#include "ui_SetApprover.h"

namespace view::Order {
SetApprover::SetApprover(QString&theTeacherId,QWidget *parent) :
    QDialog(parent), ui(new Ui::SetApprover),teacherId(theTeacherId) {
    ui->setupUi(this);
}

SetApprover::~SetApprover() {
    delete ui;
}

QString SetApprover::getTeacherId() {
    return ui->lineEdit->text();
}

void SetApprover::on_btnSet_clicked() {
    teacherId = ui->lineEdit->text();
    this->close();
}

void SetApprover::on_btnCancel_clicked() {
    this->close();
}
} // view::Order
