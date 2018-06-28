
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_macro.h
 * @brief    基础库的全局宏定义
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.1.1
 * @date     2018/01/20
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2018/01/20 | 1.0.0.1   | kunyang  | 创建文件
 * 2018/03/06 | 1.0.1.1   | kunyang  | 加入STL模板宏kyHasSTL
 */
#ifndef KY_MACRO_H
#define KY_MACRO_H

//! 开启CPU扩展指令
#define kyHasIntrin
//! 开启智能指针
#define kyHasAutoPtr
//! 开启引用计数对象
#define kyHasReferCount
//! 开启调试和日志输出
#define kyHasLog
#define kyHasDebug
//! 开启断言
#define kyHasAssert
//! 开启内存屏障
#define kyHasMemoryBarrier
//! 如果架构支持SMP将开启
#define kyHasMemoryBarrierSMP

//! 使用原子操作实现自旋锁
#define kyAtomicSpinLock
//! 使用Posix实现自旋锁
//#define kyPosixSpinLock
//! 使用pthread_once的单例模式
#define kyHasSingletonPosix
//! 使用互斥锁模式的单例模式
//#define kyHasSingletonMutex

//! 开启STL模板库
#define kyHasSTL

//! 设置颜色位数
//#define kyColor16Bit
#define kyColor32Bit
//#define kyColor64Bit

#define kyHasClockGetTime

#define kyHasASyncIO

#define kyHasPPoll
#define kyHasPoll
#define kyHasEPoll
#define kyHasPSelect

#endif // KY_MACRO_H
