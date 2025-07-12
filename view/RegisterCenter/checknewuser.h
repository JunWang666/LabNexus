//
// Created by gouzuang on 25-7-11.
//

#ifndef CHECKNEWUSER_H
#define CHECKNEWUSER_H

#include <QWidget>

namespace view::RegisterCenter {
QT_BEGIN_NAMESPACE
namespace Ui { class CheckNewUser; }
QT_END_NAMESPACE

class CheckNewUser : public QWidget {
Q_OBJECT

public:
    explicit CheckNewUser(QWidget *parent = nullptr);
    ~CheckNewUser() override;
    void loadDataFromDatabase(int page=1);

private:
    void updatePaginationControls();

private slots:
    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_addButton_clicked();
    void on_refreshButton_clicked();
private:
    Ui::CheckNewUser *ui;
    int m_currentPage;
    int m_totalPages;
    const int m_itemsPerPage = 10;
};
} // view::RegisterCenter

#endif //CHECKNEWUSER_H
