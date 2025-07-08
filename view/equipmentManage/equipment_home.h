//
// Created by gouzuang on 25-7-6.
//

#ifndef EQUIPMENT_HOME_H
#define EQUIPMENT_HOME_H

#include <QWidget>
#include <qdialog.h>

namespace view::equipment {
QT_BEGIN_NAMESPACE
namespace Ui { class equipment_home;class kaddmanage; class kchangemanage; class kdeletemanage; class applysee; class exam;}
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

    void on_kexam_clicked();

    void on_searchbtn_clicked();

    void on_kseeborrow_clicked();

    void on_seefix_clicked();

    void on_outdata_clicked();

    void on_blacklist_clicked();

private:
    Ui::equipment_home *ui;
};
// 添加设备窗口类
class kaddmanage : public QDialog {
    Q_OBJECT

public:
    explicit kaddmanage(QWidget *parent = nullptr);
    ~kaddmanage() override;

private:
    Ui::kaddmanage *ui;
    void setupUi(QWidget *widget);
};

// 修改设备信息窗口类
class kchangemanage : public QDialog {
    Q_OBJECT

public:
    explicit kchangemanage(QWidget *parent = nullptr);
    ~kchangemanage() override;

private:
    Ui::kchangemanage *ui;
    void setupUi(QWidget *widget);
};

// 删除设备窗口类
class kdeletemanage : public QDialog {
    Q_OBJECT

public:
    explicit kdeletemanage(QWidget *parent = nullptr);
    ~kdeletemanage() override;

private:
    Ui::kdeletemanage *ui;
    void setupUi(QWidget *widget);
};

// 申请界面窗口类
class applysee : public QDialog {
    Q_OBJECT

public:
    explicit applysee(QWidget *parent = nullptr);
    ~applysee() override;

private:
    Ui::applysee *ui;
    void setupUi(QWidget *widget);
};

// 考试界面窗口类
class exam : public QDialog {
    Q_OBJECT

public:
    explicit exam(QWidget *parent = nullptr);
    ~exam() override;

private:
    Ui::exam *ui;
    void setupUi(QWidget *widget);
};

} // view::equipment

#endif // EQUIPMENTMANAGE_H

