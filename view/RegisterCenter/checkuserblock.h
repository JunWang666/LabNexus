//
// Created by gouzuang on 25-7-13.
//

#ifndef CHECKUSERBLOCK_H
#define CHECKUSERBLOCK_H

#include <QWidget>

namespace view::RegisterCenter {
QT_BEGIN_NAMESPACE
namespace Ui { class CheckUserBlock; }
QT_END_NAMESPACE

class CheckUserBlock : public QWidget {
Q_OBJECT

public:
    explicit CheckUserBlock(QWidget *parent = nullptr);
    ~CheckUserBlock() override;



private:
    Ui::CheckUserBlock *ui;
    int m_currentPage;
    int m_totalPages;
    const int m_itemsPerPage = 10;
};
} // view::RegisterCenter

#endif //CHECKUSERBLOCK_H
