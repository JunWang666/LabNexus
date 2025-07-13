//
// Created by gouzuang on 25-7-13.
//

#ifndef NATIVESPLASH_H
#define NATIVESPLASH_H

#include <QWidget>

namespace view::SplashScreen {
QT_BEGIN_NAMESPACE
namespace Ui { class nativeSplash; }
QT_END_NAMESPACE

class nativeSplash : public QWidget {
Q_OBJECT

public:
    explicit nativeSplash(QWidget *parent = nullptr);
    ~nativeSplash() override;

private:
    Ui::nativeSplash *ui;
};
} // view::SplashScreen

#endif //NATIVESPLASH_H
