//
// Created by gouzuang on 25-7-6.
//

#ifndef EQUIPMENT_HOME_H
#define EQUIPMENT_HOME_H

#include <QWidget>

namespace view::equipment {
QT_BEGIN_NAMESPACE
namespace Ui { class equipment_home; }
QT_END_NAMESPACE

class equipment_home : public QWidget {
Q_OBJECT

public:
    explicit equipment_home(QWidget *parent = nullptr);
    ~equipment_home() override;

private slots:
    void on_toapply_clicked();

    void on_stuexam_clicked();

    void on_adfix_clicked();

    void on_zreturn_clicked();

    void on_zclose_clicked();

    void on_kadd_clicked();

    void on_kreall_clicked();

    void on_kchange_clicked();

    void on_kdel_clicked();

    void on_exam_clicked();

    void on_searchbtn_clicked();

private:
    Ui::equipment_home *ui;
};
} // view::equipment

#endif //EQUIPMENT_HOME_H
