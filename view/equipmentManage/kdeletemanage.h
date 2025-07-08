#ifndef KDELETEMANAGE_H
#define KDELETEMANAGE_H

#include <QDialog>

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
    void on_deleteButton_clicked();
    void on_batchDeleteButton_clicked();
    void on_deleteRecordButton_clicked();
    void on_undoButton_clicked();

private:
    Ui::kdeletemanage *ui;
};

} // view::equipment

#endif // KDELETEMANAGE_H
