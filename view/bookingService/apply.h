//
// Created by Nana7mi_ on 25-7-7.
//

#ifndef APPLY_H
#define APPLY_H

#include <QWidget>

namespace view::Order {
QT_BEGIN_NAMESPACE
namespace Ui { class Apply; }
QT_END_NAMESPACE

class Apply : public QWidget {
Q_OBJECT

public:
    explicit Apply(QWidget *parent = nullptr);
    ~Apply() override;
public slots:
    void on_btnclose_clicked();
private:
    Ui::Apply *ui;
};
} // view::Order

#endif //APPLY_H
