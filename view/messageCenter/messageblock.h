#include "pch.h"
//
// Created by gouzuang on 25-7-8.
//

#ifndef MESSAGEBLOCK_H
#define MESSAGEBLOCK_H

namespace view::messageCenter {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class MessageBlock;
    }

    QT_END_NAMESPACE

    struct MessageData {
        QString id;
        QString title;
        QString sender;
        QString content;
        QString timestamp;
    };

    class MessageBlock : public QWidget {
        Q_OBJECT

    public:
        explicit MessageBlock(QWidget *parent = nullptr);

        /**
     * 构造一个带有给定消息ID和父窗口部件的MessageBlock。
     *
     * @param messageId 要显示的消息的ID。用于从数据库中获取消息数据。
     * @param parent 该MessageBlock的父窗口部件。
     *
     * @return 不返回值；构造函数初始化一个新的MessageBlock实例。
     */
        explicit MessageBlock(const QString &messageId, QWidget *parent = nullptr);

        /**
         * @brief 构造一个新的MessageBlock，带有给定的消息数据和父窗口部件。
         *
         * 该构造函数初始化一个新的MessageBlock类实例，设置用户界面并用提供的消息数据进行初始化。
         *
         * @param messageData 要在块中显示的消息的数据。
         * @param parent 该MessageBlock的父窗口部件。可以是nullptr。
         * @return 无
         */
        explicit MessageBlock(const MessageData &messageData, QWidget *parent = nullptr);

        ~MessageBlock() override;

    private:
        void init(const MessageData &messageData);

    private:
        Ui::MessageBlock *ui;
        QString m_messageId;

    public slots:
        void on_frame_clicked();
    };
} // view::MessageCenter

#endif //MESSAGEBLOCK_H
