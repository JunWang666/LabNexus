//
// Created by gouzuang on 25-7-6.
//

#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>

namespace view::maintenance {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class maintenance_homepage;
    }

    QT_END_NAMESPACE

    class maintenance_homepage : public QWidget {
        Q_OBJECT

    public:
        explicit maintenance_homepage(QWidget *parent = nullptr);

        ~maintenance_homepage() override;

    private:
        Ui::maintenance_homepage *ui;
    };
} // view::maintenance

#endif //HOMEPAGE_H
