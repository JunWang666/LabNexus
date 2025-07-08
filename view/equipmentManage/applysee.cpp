#include "applysee.h"
#include "ui_applysee.h"

namespace view::equipment {

applysee::applysee(QWidget *parent)
    : QDialog(parent)
    , ui(new view::equipment::Ui::applysee) {
    ui->setupUi(this);
    setWindowTitle("申请预约");
}

applysee::~applysee() {
    delete ui;
}

} // view::equipment
