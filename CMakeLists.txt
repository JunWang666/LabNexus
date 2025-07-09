cmake_minimum_required(VERSION 3.30)

project(LabNexus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Sql
        REQUIRED)

add_executable(LabNexus main.cpp
        service/logger/logger.cpp
        service/logger/logger.h
        pch.h
        service/database/databasePathProvidor.cpp
        service/database/databasePathProvidor.h
        view/bookingService/booking_home.cpp
        view/bookingService/booking_home.h
        view/bookingService/booking_home.ui
        view/inventoryManage/inventory_home.cpp
        view/inventoryManage/inventory_home.h
        view/inventoryManage/inventory_home.ui
        view/equipmentManage/equipment_home.cpp
        view/equipmentManage/equipment_home.h
        view/equipmentManage/equipment_home.ui
        view/equipmentManage/kaddmanage.cpp
        view/equipmentManage/kaddmanage.h
        view/equipmentManage/kaddmanage.ui
        view/equipmentManage/kchangemanage.cpp
        view/equipmentManage/kchangemanage.h
        view/equipmentManage/kchangemanage.ui
        view/equipmentManage/kdeletemanage.cpp
        view/equipmentManage/kdeletemanage.h
        view/equipmentManage/kdeletemanage.ui
        view/equipmentManage/applysee.cpp
        view/equipmentManage/applysee.h
        view/equipmentManage/applysee.ui
        view/equipmentManage/exam.cpp
        view/equipmentManage/exam.h
        view/equipmentManage/exam.ui
        view/bookingService/rent.cpp
        view/bookingService/rent.h
        view/bookingService/rent.ui
        view/bookingService/sendrent.cpp
        view/bookingService/sendrent.h
        view/bookingService/sendrent.ui
        view/bookingService/apply.cpp
        view/bookingService/apply.h
        view/bookingService/apply.ui
        view/bookingService/rentteacher.cpp
        view/bookingService/rentteacher.h
        view/bookingService/rentteacher.ui
        view/bookingService/managercheck.cpp
        view/bookingService/managercheck.h
        view/bookingService/managercheck.ui
        service/database/databaseManager.cpp
        service/database/databaseManager.h
        module/data/data_UserControl.cpp
        module/data/data_UserControl.h
        module/data/data_EquipmentManage.cpp
        module/data/data_EquipmentManage.h
        module/data/data_Booking.cpp
        module/data/data_Booking.h
        module/model/BookingDataModel.cpp
        module/model/BookingDataModel.h
        module/model/EquipmentDataModel.cpp
        module/model/EquipmentDataModel.h
        view/loginPage/loginpage.cpp
        view/loginPage/loginpage.h
        view/loginPage/loginpage.ui
        view/loginPage/registerpage.cpp
        view/loginPage/registerpage.h
        view/loginPage/registerpage.ui
        view/homepage/teacherhomepage.cpp
        view/homepage/teacherhomepage.h
        view/homepage/teacherhomepage.ui
        view/homepage/studenthomepage.cpp
        view/homepage/studenthomepage.h
        view/homepage/studenthomepage.ui
        view/homepage/administratorhomepage.cpp
        view/homepage/administratorhomepage.h
        view/homepage/administratorhomepage.ui
        view/loginPage/changepassword.cpp
        view/loginPage/changepassword.h
        view/loginPage/changepassword.ui
        module/model/filterproxymdel.cpp
        module/model/filterproxymdel.h
        module/model/ApprovalStatusDelegate.cpp
        module/model/ApprovalStatusDelegate.h
        module/model/RepairStatusDelegate.cpp
        module/model/RepairStatusDelegate.h
        view/loginPage/style.qrc
        assets/style/fluentDesign.qss
        assets/style/style.qrc
        assets/style/style_res.qrc
        assets/style/blurredTranslucent.h
        module/data/data_mail.h
        module/data/data_mail.cpp
        view/messageCenter/messagewindow.ui
        view/messageCenter/messagewindow.cpp
        view/messageCenter/messagewindow.h
        view/messageCenter/messageblock.ui
        view/messageCenter/messageblock.cpp
        view/messageCenter/messageblock.h
        service/styleHelper/StyleHelper.cpp
        service/styleHelper/StyleHelper.h
        service/MutiWindow/MutiWindow.cpp
        service/MutiWindow/MutiWindow.h
        view/messageCenter/messagedetailpage.cpp
        view/messageCenter/messagedetailpage.h
        view/messageCenter/messagedetailpage.ui
)
target_link_libraries(LabNexus
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Sql
        dwmapi
)
target_include_directories(LabNexus PRIVATE ${PROJECT_SOURCE_DIR})
target_precompile_headers(LabNexus PRIVATE pch.h)

# 添加 Qt Test 支持
find_package(Qt6 COMPONENTS Test REQUIRED)

# 日志测试
add_executable(TestLogs
        tests/test_logs.cpp
        service/logger/logger.cpp
)
target_link_libraries(TestLogs PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Test
)
target_include_directories(TestLogs PRIVATE ${PROJECT_SOURCE_DIR})
target_precompile_headers(TestLogs PRIVATE pch.h)
add_test(NAME TestLogs COMMAND TestLogs)

# 数据库测试
add_executable(TestDB
        tests/test_db.cpp
        service/logger/logger.cpp
        service/database/databasePathProvidor.cpp
        service/database/databaseManager.cpp
        module/data/data_UserControl.cpp
)
target_link_libraries(TestDB PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Test
        Qt::Sql
)
target_include_directories(TestDB PRIVATE ${PROJECT_SOURCE_DIR})
target_precompile_headers(TestDB PRIVATE pch.h)
add_test(NAME TestDB COMMAND TestDB)

# 预订模块测试
add_executable(TestBooking
        tests/test_booking.cpp
        service/logger/logger.cpp
        service/database/databasePathProvidor.cpp
        service/database/databaseManager.cpp
        module/data/data_UserControl.cpp
        module/data/data_Booking.cpp
        tests/test_booking.cpp
)
target_link_libraries(TestBooking PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Test
        Qt::Sql
)
target_include_directories(TestBooking PRIVATE ${PROJECT_SOURCE_DIR})
target_precompile_headers(TestBooking PRIVATE pch.h)
add_test(NAME TestBooking COMMAND TestBooking)

# 邮件模块测试
add_executable(TestMail
        tests/test_mail.cpp
        service/logger/logger.cpp
        service/database/databasePathProvidor.cpp
        service/database/databaseManager.cpp
        module/data/data_UserControl.cpp
        module/data/data_mail.cpp
)
target_link_libraries(TestMail PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Test
        Qt::Sql
)
target_include_directories(TestMail PRIVATE ${PROJECT_SOURCE_DIR})
target_precompile_headers(TestMail PRIVATE pch.h)
add_test(NAME TestMail COMMAND TestMail)

add_executable(TestEquipmentManage tests/test_equipment_manage.cpp
        service/logger/logger.cpp
        service/database/databaseManager.cpp
        module/data/data_EquipmentManage.cpp
)

target_link_libraries(TestEquipmentManage PRIVATE Qt6::Core Qt6::Sql Qt6::Test Qt6::Widgets)
target_include_directories(TestEquipmentManage PRIVATE ${PROJECT_SOURCE_DIR})
target_precompile_headers(TestEquipmentManage PRIVATE pch.h)
add_test(NAME TestEquipmentManage COMMAND TestEquipmentManage)

enable_testing()

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

# 为测试目标复制 Qt6 测试库 DLL
if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    foreach (target TestLogs TestDB TestBooking TestMail TestEquipmentManage)
        add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6Test${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${target}>")
    endforeach ()
    add_custom_command(TARGET TestDB POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${QT_INSTALL_PATH}/bin/Qt6Sql${DEBUG_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:TestDB>")
    add_custom_command(TARGET TestBooking POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${QT_INSTALL_PATH}/bin/Qt6Sql${DEBUG_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:TestBooking>")
    add_custom_command(TARGET TestMail POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${QT_INSTALL_PATH}/bin/Qt6Sql${DEBUG_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:TestMail>")
    add_custom_command(TARGET TestEquipmentManage POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${QT_INSTALL_PATH}/bin/Qt6Sql${DEBUG_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:TestEquipmentManage>")
    # 复制 Qt 平台插件供测试使用
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        foreach (target TestLogs TestDB TestBooking TestMail TestEquipmentManage)
            add_custom_command(TARGET ${target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                    "$<TARGET_FILE_DIR:${target}>/plugins/platforms")
            add_custom_command(TARGET ${target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                    "$<TARGET_FILE_DIR:${target}>/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        endforeach ()
    endif ()
    # 复制 SQLite 驱动插件
    if (EXISTS "${QT_INSTALL_PATH}/plugins/sqldrivers/qsqlite${DEBUG_SUFFIX}.dll")
        foreach (target TestLogs TestDB TestBooking TestMail TestEquipmentManage)
            add_custom_command(TARGET ${target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                    "$<TARGET_FILE_DIR:${target}>/plugins/sqldrivers")
            add_custom_command(TARGET ${target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_INSTALL_PATH}/plugins/sqldrivers/qsqlite${DEBUG_SUFFIX}.dll"
                    "$<TARGET_FILE_DIR:${target}>/plugins/sqldrivers/qsqlite.dll")
        endforeach ()
    endif ()
endif ()
