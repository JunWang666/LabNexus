//
// Created by gouzuang on 25-7-7.
//

#ifndef DATA_BOOKING_H
#define DATA_BOOKING_H

namespace data::Booking {
    /**
     * @brief 删除预订数据库。
     *
     * 该函数删除预订相关的数据库表和数据。
     */
    void dropDB();

    /**
     * @brief 构建预订数据库。
     *
     * 该函数创建设备管理相关的数据库表和数据。
     */
    void buildDB();

    void createBookingInfoTable();

    void createBookingEquipmentTable();

    void createBookingTimeTable();

    void createBookingApprovalTable();
}

#endif //DATA_BOOKING_H
