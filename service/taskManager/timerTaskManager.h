//
// Created by gouzuang on 25-7-10.
//

#ifndef TIMERTASKMANAGER_H
#define TIMERTASKMANAGER_H

#include <QObject>
#include <functional>
#include <QTimer>
#include <QVector>

namespace service::taskManager {

    class TimerTaskManager : public QObject {
        Q_OBJECT

    public:
        // 获取单例实例
        static TimerTaskManager &instance();

        // 安排一个定时任务，intervalMs 为间隔毫秒，singleShot 指定是否为单次执行
        void scheduleTask(int intervalMs, const std::function<void()> &task, bool singleShot = false);

        // 启动所有已安排的任务
        void startAll();

        // 停止所有任务
        void stopAll();

    private:
        explicit TimerTaskManager(QObject *parent = nullptr);

        ~TimerTaskManager() override;

        QVector<QTimer *> m_timers; // 存储所有定时器
    };

    TimerTaskManager& getTimer();
} // namespace service::taskManager

#endif //TIMERTASKMANAGER_H
