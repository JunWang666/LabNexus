#include "exam.h"
#include "ui_exam.h"

namespace view::equipment {

exam::exam(QWidget *parent)
    : QDialog(parent)
    , ui(new view::equipment::Ui::exam) {
    ui->setupUi(this);
    setWindowTitle("学生预约审查");
}

exam::~exam() {
    delete ui;
}

} // view::equipment
