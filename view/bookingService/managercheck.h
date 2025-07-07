//
// Created by Nana7mi_ on 25-7-7.
//

#ifndef MANAGERCHECK_H
#define MANAGERCHECK_H

#include <QWidget>

namespace view::Order {
QT_BEGIN_NAMESPACE
namespace Ui { class ManagerCheck; }
QT_END_NAMESPACE

class ManagerCheck : public QWidget {
Q_OBJECT

public:
    explicit ManagerCheck(QWidget *parent = nullptr);
    ~ManagerCheck() override;
public slots:
    void on_btnclose_clicked();
private:
    Ui::ManagerCheck *ui;
};
} // view::Order

#endif //MANAGERCHECK_H
