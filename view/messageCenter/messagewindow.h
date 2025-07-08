//
// Created by gouzuang on 25-7-8.
//

#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H

#include <QWidget>
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

        /**
         * 从数据库加载邮件数据并显示在消息窗口中。
         *
         * @param userId 用户ID，用于查询该用户的邮件
         * @param page 分页参数，默认为1，表示从第一页开始加载邮件
         */
        void loadMailsFromDatabase(int userId, int page = 1);

        Ui::MessageWindow *ui;
    };
}

#endif //MESSAGEWINDOW_H
