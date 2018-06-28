
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_define.h
 * @brief    基础定义
 *       1.定义了使用的类型.
 *       2.ky_ref 引用计数器，对象间的赋值及使用的基本管理
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.0.1
 * @date     2012/01/02
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2012/01/02 | 1.0.0.1   | kunyang  | 创建文件
 * 2012/01/10 | 1.0.1.0   | kunyang  | 加入引用的可共享属性
 */
#ifndef ky_TIMER
#define ky_TIMER

#include "ky_datetime.h"
#include "ky_thread.h"
#include "ky_object.h"

typedef enum
{
    TimerState_Pause,
    TimerState_Stop,
    TimerState_Run,
}TimerState;
typedef enum
{
    TimerMode_Time, //! 计时器
    TimerMode_Timing//! 定时器
}TimerMode;


class ky_timer :public ky_thread, public ky_object
{
    friend class ky_datetime;
public:
    //! 模式 检测间隔时间(ms)
    explicit ky_timer(TimerMode mode = TimerMode_Time, size_t _interval = 10);

    //! 启动，参数为超时时间(ms)。注：若为0则跟线程运行没什么区别
    void start(size_t msec = 0);
    //! 停止
    void stop(bool is_wait = false);
    //! 暂停
    void pause();
    //! 继续
    void resume();
    //! 状态
    TimerState state() const;
    //! 检测间隔时间
    size_t interval()const;
    TimerMode mode()const;

    //! 计时器功能
    //! 复位计时器时间
    int restart();
    //! 计时器过去的时间
    int elapsed() const;

public:
    Signal<void (size_t ts)> timeout;

protected:
    virtual void run();

private:
    //! 定时器
    size_t last_time;
    size_t timer_timeout;
    TimerState current_state;
    size_t thread_interval;
    TimerMode timer_mode;

    ky_datetime tally;

};

#endif // ky_TIMER

