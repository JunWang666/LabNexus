//
// Created by Nana7mi_ on 25-7-10.
//

#ifndef SETAPPROVER_H
#define SETAPPROVER_H

#include <QDialog>

namespace view::Order {
QT_BEGIN_NAMESPACE
namespace Ui { class SetApprover; }
QT_END_NAMESPACE

class SetApprover : public QDialog {
Q_OBJECT

public:
    explicit SetApprover(QString& teacherId,QWidget *parent = nullptr);
    ~SetApprover() override;
    QString getTeacherId();
public slots:
    void on_btnSet_clicked();
    void on_btnCancel_clicked();
private:
    Ui::SetApprover *ui;
    QString &teacherId;
};
} // view::Order

#endif //SETAPPROVER_H
