//
// Created by gouzuang on 25-7-12.
//

#ifndef EQUIPMENTCLASSMANAGEHOMEPAGE_H
#define EQUIPMENTCLASSMANAGEHOMEPAGE_H

#include <QWidget>

#include "module/data/data_EquipmentManage.h"

namespace view::EquipmentClass {
    QT_BEGIN_NAMESPACE
    namespace Ui { class EquipmentClassManageHomepage; }
    QT_END_NAMESPACE

    class EquipmentClassManageHomepage : public QWidget {
        Q_OBJECT

        public:
        explicit EquipmentClassManageHomepage(QWidget *parent = nullptr);
        ~EquipmentClassManageHomepage() override;

    private:
        void initializeUI();
        void loadEquipmentClasses(int page = 1);
        void updatePaginationControls();

    private slots:
        void on_prevButton_clicked();
        void on_nextButton_clicked();

    private:
        Ui::EquipmentClassManageHomepage *ui;

        int m_currentPage;
        int m_totalPages;
        const int m_itemsPerPage = 10; // 每页显示数量，与data层保持一致
    };
} // view::EquipmentClass

#endif //EQUIPMENTCLASSMANAGEHOMEPAGE_H