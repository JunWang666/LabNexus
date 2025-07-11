//
// Created by Nana7mi_ on 25-7-8.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FilterProxyMdel.h" resolved

#include "filterproxymdel.h"

namespace fliterModel {
FilterProxyMdel::FilterProxyMdel(QWidget *parent) :
    QSortFilterProxyModel(parent) {
}

FilterProxyMdel::~FilterProxyMdel() {
}

void FilterProxyMdel::setUserIdColumn(int column) {
    m_userIdColumn = column;
}

void FilterProxyMdel::setStatusColumn(int column) {
    m_statusColumn = column;
}

void FilterProxyMdel::setGroupColunm(int column) {
    m_groupColumn = column;
}

void FilterProxyMdel::setUserIdFilter(int userId) {
    m_filterUserId = userId;
    invalidateFilter(); // 应用新的过滤规则
}

void FilterProxyMdel::setStatusFilter(const QString &status) {
    m_filterStatus = status;
    invalidateFilter(); // 应用新的过滤规则
}

void FilterProxyMdel::setGroupFilter(const QString &group) {
    m_filterGroup = group;
    invalidateFilter();
}

void FilterProxyMdel::clearFilters() {
    m_filterUserId = -1;
    m_filterStatus.clear();
    m_filterGroup.clear();
    invalidateFilter();
}

void FilterProxyMdel::clearUserIdFilter() {
    m_filterUserId = -1; // 将用户ID设置为“未激活”状态
    invalidateFilter();
}

void FilterProxyMdel::clearStatusFilter() {
    m_filterStatus.clear(); // 将状态设置为空，即“未激活”
    invalidateFilter();
}

void FilterProxyMdel::clearGroupFilter() {
    m_filterGroup.clear();  // 将身份设置为空，即“未激活”
    invalidateFilter();
}

bool FilterProxyMdel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const {
    // 默认认为所有检查都通过
    bool userIdMatch = true;
    bool statusMatch = true;
    bool groupMatch = true;

    // 检查用户ID过滤器是否激活
    if (m_filterUserId != -1 && m_userIdColumn != -1) {
        QModelIndex index = sourceModel()->index(source_row, m_userIdColumn, source_parent);
        userIdMatch = (sourceModel()->data(index).toInt() == m_filterUserId);
    }

    // 检查状态过滤器是否激活
    if (!m_filterStatus.isEmpty() && m_statusColumn != -1) {
        QModelIndex index = sourceModel()->index(source_row, m_statusColumn, source_parent);
        statusMatch = (sourceModel()->data(index).toString() == m_filterStatus);
    }
    if (!m_filterGroup.isEmpty() && m_groupColumn != -1) {
        QModelIndex index = sourceModel()->index(source_row, m_groupColumn, source_parent);
        groupMatch = (sourceModel()->data(index).toString() == m_filterGroup);
    }

    // 只有所有激活的过滤器条件都满足的行才会被显示（AND 逻辑）
    return userIdMatch && statusMatch && groupMatch;
}
} // fliterModel
