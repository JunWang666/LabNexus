//
// Created by gouzuang on 25-7-13.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CheckUserBlock.h" resolved

#include "checkuserblock.h"
#include "ui_CheckUserBlock.h"

namespace view::RegisterCenter {
    CheckUserBlock::CheckUserBlock(QWidget *parent) : QWidget(parent), ui(new Ui::CheckUserBlock) {
        ui->setupUi(this);
    }

    CheckUserBlock::~CheckUserBlock() {
        delete ui;
    }
} // view::RegisterCenter
