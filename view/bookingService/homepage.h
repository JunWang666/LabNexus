//
// Created by gouzuang on 25-7-6.
//

#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>

namespace view::booking {
QT_BEGIN_NAMESPACE
namespace Ui { class homepage; }
QT_END_NAMESPACE

class homepage : public QWidget {
Q_OBJECT

public:
    explicit homepage(QWidget *parent = nullptr);
    ~homepage() override;

private:
    Ui::homepage *ui;
};
} // view::booking

#endif //HOMEPAGE_H
