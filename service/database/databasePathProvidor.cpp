//
// Created by gouzuang on 25-7-6.
//

#include "databasePathProvidor.h"

#include <QDir>

namespace service::Path {
    void init() {
        QDir dir;
        QStringList paths = { booking(), equipment(), mail(), user(), base(), debuglog(), datalog() };
        for (const QString& path : paths) {
            if (!dir.exists(path)) {
                if (dir.mkpath(path)) {
                    qDebug() << "已创建路径:" << path;
                } else {
                    qDebug() << "创建路径失败:" << path;
                }
            }
        }
    }
}

