#ifndef KDELETEMANAGE_H
#define KDELETEMANAGE_H

#include "pch.h"

namespace view::equipment {
QT_BEGIN_NAMESPACE
namespace Ui { class kdeletemanage; }
QT_END_NAMESPACE

}

namespace view::equipment {

class kdeletemanage : public QDialog {
    Q_OBJECT

public:
    explicit kdeletemanage(QWidget *parent = nullptr);
    ~kdeletemanage() override;

private slots:


    void on_addrecord_clicked();

    void on_changerecord_clicked();

    void on_deleterecord_clicked();

    void on_close_clicked();

private:
    Ui::kdeletemanage *ui;
};

} // view::equipment

#endif // KDELETEMANAGE_H
