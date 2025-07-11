// databasePathProvidor.h

#ifndef DATABASEPATHPROVIDOR_H
#define DATABASEPATHPROVIDOR_H

#include <QString>
#include <QDir> // 需要包含QDir

namespace service::Path {
    void init();

    inline QString base() {
        return "./data/";
    }

    inline QString booking() {
        return base() + "booking.db";
    }

    inline QString equipment() {
        return base() + "equipment.db";
    }

    inline QString mail() {
        return base() + "mail.db";
    }

    inline QString user() {
        return base() + "user.db";
    }

    inline QString debuglog() {
        return "./log/";
    }

    inline QString datalog() {
        return "./log/data.log";
    }
}

#endif //DATABASEPATHPROVIDOR_H