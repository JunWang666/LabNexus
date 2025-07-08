//
// Created by gouzuang on 25-7-8.
//

#ifndef DATA_MAIL_H
#define DATA_MAIL_H
#include <QList>
#include <QString>
#include <QDateTime>

namespace data::mail {
    inline static const QString path = "mail.db";

    struct Mail {
        int id;
        int sender_id;
        int receiver_id;
        QString subject;
        QString content;
        QDateTime send_date;
        int status;
        QString extra_data;
    };


    void buildDB();

    void dropDB();

    void createMailTable();

    void send_mail(int senderId, int receiverId, const QString &subject, const QString &content,
                   const QString &extra_data);

    QList<Mail> getAllMails(int receiverId, int page, int pageSize);

    QList<Mail> getUnreadMails(int receiverId, int page, int pageSize);

    int getMailPageCount(int receiverId, int pageSize);

    int getUnreadMailCount(int receiverId, int pageSize);
}


#endif //DATA_MAIL_H
