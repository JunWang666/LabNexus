//
// Created by gouzuang on 25-7-8.
//

#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H

#include "pch.h"
#include "messageblock.h"

namespace view::messageCenter {
    namespace Ui {
        class MessageWindow;
    }

    class MessageWindow : public QWidget {
        Q_OBJECT

    public:
        explicit MessageWindow(QWidget *parent = nullptr);
        explicit MessageWindow(int userId, QWidget *parent = nullptr);
        ~MessageWindow() override;

    private:
        void initializeUI();
        void loadMailsFromDatabase(int userId, int page = 1);

        /**
         * @brief 根据当前页码和总邮件数更新分页控件（按钮可用性、页码标签）的状态。
         */
        void updatePaginationControls();

    private slots:
        /**
         * @brief “上一页”按钮的槽函数
         */
        void on_prevButton_clicked();

        /**
         * @brief “下一页”按钮的槽函数
         */
        void on_nextButton_clicked();


    private:
        Ui::MessageWindow *ui;

        int m_currentUserId;
        int m_currentPage;
        int m_totalMails;
        const int m_mailsPerPage = 10;
    };
}

#endif //MESSAGEWINDOW_H