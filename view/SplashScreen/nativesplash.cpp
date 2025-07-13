//
// Created by gouzuang on 25-7-13.
//

// You may need to build the project (run Qt uic code generator) to get "ui_nativeSplash.h" resolved

#include "nativesplash.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

#include "ui_nativeSplash.h"

namespace view::SplashScreen {
    nativeSplash::nativeSplash(QWidget *parent) : QWidget(parent), ui(new Ui::nativeSplash) {
        ui->setupUi(this);
        service::style::setMica(this);
        setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);

        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
        this->setGraphicsEffect(effect);

        QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
        anim->setDuration(800);
        anim->setStartValue(0);
        anim->setEndValue(1);
        anim->setEasingCurve(QEasingCurve::OutQuad);
        anim->start(QPropertyAnimation::DeleteWhenStopped);

        ui->progressBar->setMaximum(0);
        ui->progressBar->setMinimum(0);

        play_animation();
    }

    nativeSplash::~nativeSplash() {
        delete ui;
    }

    void nativeSplash::closeSplash() {
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
        this->setGraphicsEffect(effect);


        QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
        anim->setDuration(400);
        anim->setStartValue(1);
        anim->setEndValue(0);
        anim->setEasingCurve(QEasingCurve::InQuad);
        anim->start(QPropertyAnimation::DeleteWhenStopped);
        connect(anim, &QPropertyAnimation::finished, this, &nativeSplash::close);
    }

    void nativeSplash::play_animation() {

    }
} // view::SplashScreen
