//
// Created by gouzuang on 25-7-6.
//

#ifndef PCH_H
#define PCH_H
#include <string>
#include <QString>
#include <QDateTime>
#include <QApplication>
#include <QPushButton>
#include <QFile>
#include <QDebug>
#include <QList>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTableView>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QWidget>
#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include "service/logger/logger.h"
#include "service/database/databaseManager.h"
#include "service/styleHelper/MicaHelper.h"
#include "service/MutiWindow/MutiWindow.h"
#include "module/data/data_UserControl.h"
#include <QRegularExpression>
#include <QLineEdit>
#include <QVariant>
#include <expected>
#include "module/data/data_Booking.h"
#include <QTableView>
#include "module/data/data_mail.h"
#include "module/model/BookingDataModel.h"
#include "view/bookingService/booking_home.h"
#include "view/bookingService/rent.h"
#include "view/homepage/teacherhomepage.h"
#include "view/loginPage/loginpage.h"
#include "view/loginPage/registerpage.h"
#include "view/messageCenter/messagewindow.h"
#include "view/equipmentManage/equipment_home.h"
#include "service/taskManager/timerTaskManager.h"
#include "module/scanner/InventoryAlert.h"

#endif //PCH_H
