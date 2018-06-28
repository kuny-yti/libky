
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
#ifndef ky_DATATIME_H
#define ky_DATATIME_H

#include "ky_define.h"
#include "ky_string.h"
#include "ky_stream.h"
#include "ky_variant.h"


class ky_datetime
{
    friend class ky_timer;
public:
    typedef enum Type
    {
        RealTime,   //! 系统实时时间
        Monotonic,  //! 从系统启动到现在的时间
        Process,    //! 从进程启动到现在的时间
        Thread      //! 从线程启动到现在的时间
    }eTypes;
public:
    explicit ky_datetime(ulonglong time_micro = 0);
    ky_datetime(int y, int mh, int d, int h, int me, int s, int ms = 0);
    ky_datetime(int h, int me, int s, int ms = 0);
    ky_datetime(int me, int s, int ms = 0);
    ~ky_datetime();

    ky_datetime(const ky_datetime & rhs);
    ky_datetime & operator =(const ky_datetime & rhs);

public:
    int year() const;
    int month() const;
    int day() const;

    int hour() const;
    int minute() const;
    int second() const;
    int msec() const;

    ky_string to_string(const ky_string &form = ky_string("%d-%d-%d %d:%d:%d:%d"))const;
    void from_string(const ky_string &s,
                     const ky_string &form = ky_string("%d-%d-%d %d:%d:%d:%d"));
    uint64 to_millisec()const;

public:
    bool operator !=(const ky_datetime &);
    bool operator ==(const ky_datetime &);

    bool operator >(const ky_datetime &);
    bool operator <(const ky_datetime &);
    bool operator >=(const ky_datetime &);
    bool operator <=(const ky_datetime &);

    ky_datetime operator +(const ky_datetime &);
    ky_datetime operator -(const ky_datetime &);
    ky_datetime &operator +=(const ky_datetime &);
    ky_datetime &operator -=(const ky_datetime &);

    void swap(ky_datetime &b)
    {
        ky_datetime tmp = b;
        b = *this;
        *this = tmp;
    }
    friend void ky_swap(ky_datetime &a, ky_datetime &b) {a.swap(b);}

public:
    //! 当前系统时间
    static ky_datetime convert_time(uint64 time_micro); //微妙转时间
    static ky_datetime current_time(); // 当前系统时间

    static timespec convert_timespec(uint64 milliSeconds);
    static timespec convert_timespec();

    static uint64 millisec(); // 毫秒
    static uint64 microsec();// 微妙
    static uint64 nanosec(eTypes t= RealTime);// 纳秒

private:
    //////////////////////
    //! 时间
    uint time_msec;
    uint time_second;
    uint time_minute;
    uint time_hour;

    uint date_year ;
    uint date_month ;
    uint date_day ;

    uint64 _microsecond_;
};

ky_streamb &operator << (ky_streamb &in, const ky_datetime &v);
ky_streamb &operator >> (ky_streamb &out, ky_datetime &v);
ky_streamt &operator << (ky_streamt &in, const ky_datetime &v);
ky_streamt &operator >> (ky_streamt &out, ky_datetime &v);

ky_variant &operator << (ky_variant &va, const ky_datetime &col);
ky_variant &operator >> (ky_variant &va, ky_datetime &col);
#endif // UTIME_H
