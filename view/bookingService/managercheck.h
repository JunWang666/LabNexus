//
// Created by Nana7mi_ on 25-7-7.
//

#ifndef MANAGERCHECK_H
#define MANAGERCHECK_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
namespace view::Order {
QT_BEGIN_NAMESPACE
namespace Ui { class ManagerCheck; }
QT_END_NAMESPACE

class ManagerCheck : public QWidget {
Q_OBJECT

public:
    explicit ManagerCheck(QWidget *parent = nullptr);
    ~ManagerCheck() override;
    void loadData();
    void setUpModel();
    void setColEditable(QStandardItemModel *model,int col,bool editable);
public slots:
    void on_btnClose_clicked();
private:
    Ui::ManagerCheck *ui;
    QStandardItemModel *model;
};
} // view::Order

#endif //MANAGERCHECK_H
