//
// Created by gouzuang on 25-7-7.
//

#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QWidget>
#include <QString>

namespace view::login {
QT_BEGIN_NAMESPACE
namespace Ui { class registerPage; }
QT_END_NAMESPACE

class registerPage : public QWidget {
Q_OBJECT

public:
    explicit registerPage(QWidget *parent = nullptr);
    ~registerPage() override;

private slots:
    void on_submitButton_clicked();
    void on_studentRadioButton_clicked();
    void on_teacherRadioButton_clicked();
    void on_pushButton_cancel_clicked();
private:
    Ui::registerPage *ui;
    QString name;
    QString ID;
    QString password;
    QPoint mouseOffset;
    int identity; // 1=学生, 2=老师
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool isPasswordValid(const QString &password);
};
} // view::login

#endif //REGISTERPAGE_H
