//
// Created by gouzuang on 25-7-10.
//

#include "timerTaskManager.h"
#include <QTimer>

namespace service::taskManager {
    TimerTaskManager &TimerTaskManager::instance() {
        static TimerTaskManager inst;
        return inst;
    }

    TimerTaskManager::TimerTaskManager(QObject *parent)
        : QObject(parent) {
    }

    TimerTaskManager::~TimerTaskManager() {
        stopAll();
        qDeleteAll(m_timers);
        m_timers.clear();
    }

    TimerTaskManager & getTimer() {
        return TimerTaskManager::instance();
    }

    void TimerTaskManager::scheduleTask(int intervalMs, const std::function<void()> &task, bool singleShot) {
        QTimer *timer = new QTimer(this);
        timer->setInterval(intervalMs);
        timer->setSingleShot(singleShot);
        connect(timer, &QTimer::timeout, this, [task]() { task(); });
        m_timers.append(timer);
    }

    void TimerTaskManager::startAll() {
        for (QTimer *timer: m_timers) {
            if (!timer->isActive()) {
                timer->start();
            }
        }
    }

    void TimerTaskManager::stopAll() {
        for (QTimer *timer: m_timers) {
            if (timer->isActive()) {
                timer->stop();
            }
        }
    }
} // namespace service::taskManager
