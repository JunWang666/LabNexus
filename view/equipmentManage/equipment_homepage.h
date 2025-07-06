//
// Created by gouzuang on 25-7-6.
//

#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>

namespace view::equipment {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class equipment_homepage;
    }

    QT_END_NAMESPACE

    class equipment_homepage : public QWidget {
        Q_OBJECT

    public:
        explicit equipment_homepage(QWidget *parent = nullptr);

        ~equipment_homepage() override;

    private:
        Ui::equipment_homepage *ui;
    };
} // view::equipment

#endif //HOMEPAGE_H
