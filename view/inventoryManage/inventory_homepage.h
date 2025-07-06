//
// Created by gouzuang on 25-7-6.
//

#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>

namespace view::inventory {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class inventory_homepage;
    }

    QT_END_NAMESPACE

    class inventory_homepage : public QWidget {
        Q_OBJECT

    public:
        explicit inventory_homepage(QWidget *parent = nullptr);

        ~inventory_homepage() override;

    private:
        Ui::inventory_homepage *ui;
    };
} // view::inventory

#endif //HOMEPAGE_H
