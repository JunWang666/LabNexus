//
// Created by Nana7mi_ on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Apply.h" resolved

#include "apply.h"
#include "ui_Apply.h"

namespace view::Order {
Apply::Apply(QWidget *parent) :
    QWidget(parent), ui(new Ui::Apply) {
    ui->setupUi(this);
}

Apply::~Apply() {
    delete ui;
}

void Apply::on_btnclose_clicked()
{
    this->close();
}

} // view::Order
