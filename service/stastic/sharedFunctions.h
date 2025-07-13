//
// Created by gouzuang on 25-7-11.
//

#ifndef SHAREDFUNCTIONS_H
#define SHAREDFUNCTIONS_H
namespace service {
    void initDB();
    void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
}
#endif //SHAREDFUNCTIONS_H
