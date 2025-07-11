//
// Created by gouzuang on 25-7-6.
//

#ifndef DATABASEPATHPROVIDOR_H
#define DATABASEPATHPROVIDOR_H

namespace service::Path {
    void init();
    inline QString booking();
    inline QString equipment();
    inline QString mail();
    inline QString user();
    inline QString base();
    inline QString debuglog();
    inline QString datalog();
}

#endif //DATABASEPATHPROVIDOR_H
