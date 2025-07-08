#ifndef APPLYSEE_H
#define APPLYSEE_H

#include <QDialog>

namespace view::equipment {
QT_BEGIN_NAMESPACE
namespace Ui { class applysee; }
QT_END_NAMESPACE
}

namespace view::equipment {

class applysee : public QDialog {
    Q_OBJECT

public:
    explicit applysee(QWidget *parent = nullptr);
    ~applysee() override;

private:
    Ui::applysee *ui;
};

} // view::equipment

#endif // APPLYSEE_H
