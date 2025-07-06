//
// Created by gouzuang on 25-7-6.
//

#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>

namespace view::equipment {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class homepage;
    }

    QT_END_NAMESPACE

    class homepage : public QWidget {
        Q_OBJECT

    public:
        explicit homepage(QWidget *parent = nullptr);

        ~homepage() override;

    private:
        Ui::homepage *ui;
    };
} // view::equipment

#endif //HOMEPAGE_H
