# 1. 项目基础设置
cmake_minimum_required(VERSION 3.25)
project(LabNexus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. 启用Qt的自动化功能
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 3. 查找所有需要的Qt模块
find_package(Qt6 COMPONENTS Core Gui Widgets Sql Test REQUIRED)

# 4. 定义共享源码文件列表 (除 main.cpp 和 tests/*.cpp 之外的所有文件)
set(SHARED_SOURCES
        pch.h
        service/logger/logger.cpp
        service/logger/logger.h
        service/database/databasePathProvidor.cpp
        service/database/databasePathProvidor.h
        view/bookingService/booking_home.cpp
        view/bookingService/booking_home.h
        view/bookingService/booking_home.ui
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
        assets/style/style.qrc
        assets/style/style_res.qrc
        module/data/data_mail.h
        module/data/data_mail.cpp
        view/messageCenter/messagewindow.ui
        view/messageCenter/messagewindow.cpp
        view/messageCenter/messagewindow.h
        view/messageCenter/messageblock.ui
        view/messageCenter/messageblock.cpp
        view/messageCenter/messageblock.h
        service/styleHelper/MicaHelper.cpp
        service/styleHelper/MicaHelper.h
        service/MutiWindow/MutiWindow.h
        service/MutiWindow/MutiWindow.cpp
        view/messageCenter/messagedetailpage.cpp
        view/messageCenter/messagedetailpage.h
        view/messageCenter/messagedetailpage.ui
        view/bookingService/setapprover.cpp
        view/bookingService/setapprover.h
        view/bookingService/setapprover.ui
        module/scanner/InventoryAlert.cpp
        module/scanner/InventoryAlert.h
        service/taskManager/timerTaskManager.cpp
        service/taskManager/timerTaskManager.h
        view/RegisterCenter/checknewuser.cpp
        view/RegisterCenter/checknewuser.h
        view/RegisterCenter/checknewuser.ui
        service/stastic/staticdata.h
        service/stastic/sharedFunctions.cpp
        service/stastic/sharedFunctions.h
)
list(REMOVE_DUPLICATES SHARED_SOURCES)

# 5. 定义库的集合，方便重用
set(CORE_LIBS Qt::Core Qt::Gui Qt::Widgets Qt::Sql dwmapi dbghelp)
set(TEST_LIBS ${CORE_LIBS} Qt::Test)
set(PCH_FILE "pch.h")

# 6. 配置共享代码的OBJECT库 (这是解决问题的核心)
add_library(LabNexus_Objects OBJECT ${SHARED_SOURCES})

# 为对象库自身配置编译选项 (PRIVATE 表示仅自己使用，不传递给消费者)
target_link_libraries(LabNexus_Objects PRIVATE ${CORE_LIBS})
target_include_directories(LabNexus_Objects PRIVATE ${PROJECT_SOURCE_DIR})
target_precompile_headers(LabNexus_Objects PRIVATE ${PCH_FILE})

# 7. 配置主程序 (明确指定所有配置)
add_executable(LabNexus main.cpp $<TARGET_OBJECTS:LabNexus_Objects>)

target_link_libraries(LabNexus PRIVATE ${CORE_LIBS})
target_include_directories(LabNexus PRIVATE ${PROJECT_SOURCE_DIR})
target_precompile_headers(LabNexus PRIVATE ${PCH_FILE})

# 8. 使用函数添加所有测试
enable_testing()

function(add_labnexus_test TARGET_NAME)
    # 1. 创建可执行文件，使用传入的 TARGET_NAME 拼接源文件名
    add_executable(${TARGET_NAME}
            "tests/${TARGET_NAME}.cpp"
            $<TARGET_OBJECTS:LabNexus_Objects>
    )
    # 2. 链接测试需要的库
    target_link_libraries(${TARGET_NAME} PRIVATE ${TEST_LIBS})

    # 3. 添加头文件目录 (这是之前函数版缺失的关键步骤)
    target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR})

    # 4. 启用预编译头 (这也是之前函数版缺失的关键步骤)
    target_precompile_headers(${TARGET_NAME} PRIVATE ${PCH_FILE})

    # 5. 添加到CTest测试集
    add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
endfunction()

# 依次调用函数来创建所有测试目标
add_labnexus_test(test_logs)
add_labnexus_test(test_db)
add_labnexus_test(test_booking)
add_labnexus_test(test_mail)
add_labnexus_test(test_equipment_manage)


# 9. 配置Windows平台运行所需的DLL自动部署 (最佳实践：调用windeployqt)
if(WIN32)
    # --- 定义所有需要部署的可执行文件 ---
    set(ALL_EXECUTABLES
            LabNexus
            test_logs
            test_db
            test_booking
            test_mail
            test_equipment_manage
    )

    # --- 循环为每个可执行文件添加自动部署步骤 ---
    foreach(TARGET_EXE IN LISTS ALL_EXECUTABLES)
        message(STATUS "Configuring automatic deployment for ${TARGET_EXE} using windeployqt.")

        add_custom_command(TARGET ${TARGET_EXE} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "Running windeployqt for ${TARGET_EXE}..."
                COMMAND Qt6::windeployqt # <-- 使用Qt6提供的windeployqt工具
                # -- 基本选项 --
                --dir "$<TARGET_FILE_DIR:${TARGET_EXE}>" # 指定输出目录为exe所在目录
                --pdb # 同时复制调试符号（如果有）

                # -- 精简输出，移除不常用的组件 --
                --no-translations
                --no-opengl-sw
                --no-svg
                --no-system-d3d-compiler

                # 指定要分析的exe文件
                "$<TARGET_FILE:${TARGET_EXE}>"
                VERBATIM
        )
    endforeach()
endif()