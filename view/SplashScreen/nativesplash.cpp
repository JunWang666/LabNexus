//
// Created by gouzuang on 25-7-13.
//

// You may need to build the project (run Qt uic code generator) to get "ui_nativeSplash.h" resolved

#include "nativesplash.h"
#include "ui_nativeSplash.h"

namespace view::SplashScreen {
nativeSplash::nativeSplash(QWidget *parent) :
    QWidget(parent), ui(new Ui::nativeSplash) {
    ui->setupUi(this);
    service::style::setMica(this);
}

nativeSplash::~nativeSplash() {
    delete ui;
}
} // view::SplashScreen
