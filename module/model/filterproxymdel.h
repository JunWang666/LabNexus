//
// Created by Nana7mi_ on 25-7-8.
//

#ifndef FILTERPROXYMDEL_H
#define FILTERPROXYMDEL_H

#include <QSortFilterProxyModel>

namespace fliterModel {

class FilterProxyMdel : public QSortFilterProxyModel {
Q_OBJECT

public:
    explicit FilterProxyMdel(QWidget *parent = nullptr);
    ~FilterProxyMdel() override;
    // 设置用于过滤的列号
    void setUserIdColumn(int column);
    void setStatusColumn(int column);
    void setGroupColunm(int column);

public slots:
    // 用于设置过滤值的槽
    void setUserIdFilter(int userId);
    void setStatusFilter(const QString &status);
    void setGroupFilter(const QString &group);
    void clearFilters();
    // 单独清除某个过滤条件
    void clearUserIdFilter();
    void clearStatusFilter();
    void clearGroupFilter();
    // 重写核心的过滤函数
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    // -1 表示用户ID过滤器未激活
    int m_filterUserId = -1;
    // 空字符串表示状态过滤器未激活
    QString m_filterStatus;
    // 空字符串表示身份过滤器未激活
    QString m_filterGroup;
    // 用于存储列号的成员变量
    int m_userIdColumn = -1;
    int m_statusColumn = -1;
    int m_groupColumn = -1;
};
} // fliterModel

#endif //FILTERPROXYMDEL_H
