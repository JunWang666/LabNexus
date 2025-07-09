#ifndef KCHANGEMANAGE_H
#define KCHANGEMANAGE_H

#include "pch.h"

namespace view::equipment {
QT_BEGIN_NAMESPACE
namespace Ui { class kchangemanage; }
QT_END_NAMESPACE

}

namespace view::equipment {

class kchangemanage : public QDialog {
    Q_OBJECT

public:
    explicit kchangemanage(QWidget *parent = nullptr);
    ~kchangemanage() override;

private slots:
    void on_find_clicked();
    void on_change_clicked();
    void on_deleteButton_clicked();
    void on_next_clicked();
    void on_before_clicked();
    void on_cancelButton_clicked();

private:
    Ui::kchangemanage *ui;
};

} // view::equipment

#endif // KCHANGEMANAGE_H
