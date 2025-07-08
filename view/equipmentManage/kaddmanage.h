#ifndef KADDMANAGE_H
#define KADDMANAGE_H

#include <QDialog>

namespace view::equipment {
QT_BEGIN_NAMESPACE
namespace Ui { class kaddmanage; }
QT_END_NAMESPACE

}

namespace view::equipment {

class kaddmanage : public QDialog {
    Q_OBJECT

public:
    explicit kaddmanage(QWidget *parent = nullptr);
    ~kaddmanage() override;

private slots:
    void on_addButton_clicked();

private:
    Ui::kaddmanage *ui;
};

} // view::equipment

#endif // KADDMANAGE_H
