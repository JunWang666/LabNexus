//
// Created by gouzuang on 25-7-9.
//

#include "StyleHelper.h"

// 仅在 Windows 平台下编译此文件的内容
#if defined(Q_OS_WIN)
#include <windows.h>
#include <dwmapi.h>

// 链接 dwmapi.lib 库，用于调用 DWM API
#pragma comment(lib, "dwmapi.lib")

// --- 关键部分：手动定义缺失的宏 ---
// 检查宏是否已定义，避免在较新的 SDK 中产生重定义错误

// 定义 DWMWA_SYSTEMBACKDROP_TYPE 属性，其值为 38
#ifndef DWMWA_SYSTEMBACKDROP_TYPE
#define DWMWA_SYSTEMBACKDROP_TYPE 38
#endif

// 定义 Mica 效果对应的值
#ifndef DWMSBT_MAINWINDOW
#define DWMSBT_MAINWINDOW 2
#endif

#endif // Q_OS_WIN

namespace service::style {
    void setMica(QWidget *window) {
        // 确保只在 Windows 平台上执行
#if defined(Q_OS_WIN)
        if (!window) {
            return;
        }

        window->setAttribute(Qt::WA_TranslucentBackground);

        HWND hwnd = reinterpret_cast<HWND>(window->winId());
        if (hwnd) {
            // 使用我们手动定义的宏来设置 Mica 效果
            int backdropType = DWMSBT_MAINWINDOW;
            DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));

            MARGINS margins = {-1};
            DwmExtendFrameIntoClientArea(hwnd, &margins);
        }
#else
        Q_UNUSED(window);
        // window->setStyleSheet("background-color: #F0F0F0;");
#endif
    }
}
