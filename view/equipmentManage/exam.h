#ifndef EXAM_H
#define EXAM_H

#include <QDialog>

namespace view::equipment {
QT_BEGIN_NAMESPACE
namespace Ui { class exam; }
QT_END_NAMESPACE

}

namespace view::equipment {

class exam : public QDialog {
    Q_OBJECT

public:
    explicit exam(QWidget *parent = nullptr);
    ~exam() override;

private:
    Ui::exam *ui;
};

} // view::equipment

#endif // EXAM_H
