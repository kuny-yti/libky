
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_thread.h
 * @brief    多线程对象以及锁的实现
 *       1.利用POSIX机制实现线程以及同步.
 *       2.支持自旋锁、互斥锁、临时锁对象、读写锁、条件锁
 *       3.线程支持运行优先级设置
 *       4.线程支持调度策略设置
 *       5.线程可使用TLS
 *       6.线程可支持ms、us、s睡眠机制
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.1.2.1
 * @date     2012/08/12
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2012/08/12 | 1.0.0.1   | kunyang  | 创建文件
 * 2012/08/14 | 1.0.1.0   | kunyang  | 完善线程实现
 * 2013/05/06 | 1.0.2.0   | kunyang  | 实现互斥锁和等待条件
 * 2013/05/08 | 1.0.3.0   | kunyang  | 实现自旋锁
 * 2013/05/10 | 1.0.4.0   | kunyang  | 实现读写锁
 * 2014/03/07 | 1.0.4.1   | kunyang  | 修改互斥锁和等待条件，优化线程对象
 * 2015/01/20 | 1.1.0.1   | kunyang  | 重构线程类并加入运行优先级和调度策略
 * 2015/09/07 | 1.1.1.1   | kunyang  | 修改自旋锁采用原子操作模式
 * 2016/11/18 | 1.1.2.1   | kunyang  | 修改线程实现并加入TLS和睡眠机制，内部加入全局线程管理
 */
#ifndef KY_THREADS_H
#define KY_THREADS_H
#include "ky_define.h"

#include <sched.h>
#include <errno.h>
#if kyPlatform == kyPlatform_Linux
#include <sys/prctl.h>
#endif

#define kyTimeoutIndefinite (-1)
#define kyThreadInvalid (0)

#if kyPlatform == kyPlatform_Linux && !defined(SCHED_IDLE)
// from linux/sched.h
# define SCHED_IDLE    5
#endif

#if kyPlatform == kyPlatform_Linux && defined(__GLIBC__)
#define hasThreadLocalStorage
#endif

#ifdef kyPosixSpinLock
/*!
 * Spin
 *
 */
class ky_spinlock
{
private:
    static const pthread_spinlock_t _spin_initializer;
    pthread_spinlock_t _spin;
    bool _is_init;
private:
    ky_spinlock(const ky_spinlock &) = delete;
    ky_spinlock &operator=(const ky_spinlock &) = delete;

public:
    explicit ky_spinlock();
    ~ky_spinlock();

    // Lock the mutex.
    bool lock();
    // Try to lock the mutex. Return true on success, false otherwise.
    bool trylock();
    // Unlock the mutex
    bool unlock();

};

#else
/*!
 * @brief The ky_spinlock class Spin (atomic)
 * @class ky_spinlock
 */
class ky_spinlock
{
private:
    ky_atomic<bool> slock;

private:
    ky_spinlock(const ky_spinlock &) = delete;
    ky_spinlock &operator=(const ky_spinlock &) = delete;

public:
    explicit ky_spinlock();
    ~ky_spinlock();

    // Lock the mutex.
    bool lock(size_t timeout = kyTimeoutIndefinite);
    // Try to lock the mutex. Return true on success, false otherwise.
    bool trylock(size_t timeout = kyTimeoutIndefinite);
    // Unlock the mutex
    bool unlock();

};
#endif

/*!
 * @brief The ky_rwlock class  Read Write Lock (posix)
 * @class ky_rwlock
 */
class ky_rwlock
{
private:
    static const pthread_rwlock_t _rw_initializer;
    pthread_rwlock_t _rw;
    bool _is_init;

private:
    ky_rwlock(const ky_rwlock &) = delete;
    ky_rwlock &operator=(const ky_rwlock &) = delete;

public:
    explicit ky_rwlock();
    ~ky_rwlock();

    // Lock the mutex.
    bool lockrd(size_t timeout = kyTimeoutIndefinite);
    bool lockwr(size_t timeout = kyTimeoutIndefinite);
    // Try to lock the mutex. Return true on success, false otherwise.
    bool trylockrd(size_t timeout = kyTimeoutIndefinite);
    bool trylockwr(size_t timeout = kyTimeoutIndefinite);
    // Unlock the mutex
    bool unlock();

};

/*!
 * @brief The ky_mutex class  Mutex Lock (posix)
 * @class ky_mutex
 */
class ky_mutex
{
private:
    static const pthread_mutex_t _mutex_initializer;
    pthread_mutex_t _mutex;
    bool _is_init;

private:
    ky_mutex(const ky_mutex &) = delete;
    ky_mutex &operator=(const ky_mutex &) = delete;

public:
    //!
    //! \brief ky_mutex
    //! \param is_reentrant 是否可重用
    //!
    explicit ky_mutex(bool is_reentrant = false);
    ~ky_mutex();

    // Lock the mutex.
    bool lock(size_t timeout = kyTimeoutIndefinite);
    // Try to lock the mutex. Return true on success, false otherwise.
    bool trylock(size_t timeout = kyTimeoutIndefinite);

    // Unlock the mutex
    bool unlock();

    friend class ky_condition;
    friend class ky_moment_lock;
};

/*!
 * @brief The ky_autolock class 片刻锁
 * @class ky_autolock
 */
class ky_autolock
{
public:
    explicit ky_autolock(ky_mutex *m);
    explicit ky_autolock(ky_mutex &m);
    ~ky_autolock();

    void unlock();
    void relock();

private:
    ky_mutex *mutex;
};

/*!
 * @brief The ky_condition class Wait condition
 * @class ky_condition
 */
class ky_condition
{
private:
    static const pthread_cond_t _cond_initializer;
    ky_mutex* _mutex;
    pthread_cond_t _cond;
    bool _is_init;

private:
    ky_condition(const ky_condition &) = delete;
    ky_condition &operator=(const ky_condition &) = delete;

public:
    // Constructor / Destructor
    explicit ky_condition();
    ~ky_condition();

    // ms Wait for the condition. The calling thread must have the mutex locked.
    bool wait(ky_mutex& m, size_t timeout = kyTimeoutIndefinite);
    // Wake one thread that waits on the condition.
    bool wake_one(); //- signal
    // Wake all threads that wait on the condition.
    bool wake_all(); //- broadcast

};

//! 线程调度的策略
typedef enum
{
    Policy_Default = 0,    //!< 系统默认
    Policy_FIFO = 1,       //!< 先进先出
    Policy_RoundRobin = 2  //!< 轮询
}eThreadPolicys;

//! 线程优先权
typedef enum
{
    Priority_Idle,
    Priority_Lowest,
    Priority_Low,
    Priority_Normal,
    Priority_High,
    Priority_Highest,

    Priority_TimeCritical,

    Priority_Inherit
}eThreadPrioritys;

typedef pthread_t thread_id;
typedef pid_t     process_id;
struct tThreadLocalStorages :public ky_ref
{
    virtual void destroy() = 0;
};

/*!
 * @brief The ky_thread class Thread
 * @class ky_thread
 * Implement the run() function in a subclass.
 */
class ky_thread
{
protected:
    thread_id __thread_id;
    ky_atomic<bool> __joinable;
    ky_atomic<bool> __running;
    ky_mutex __wait_mutex;
    ky_condition __wait_cond;

    static void* __run(void* p);

public:
    static const int priority_default = Priority_Idle;

    explicit ky_thread();
    virtual ~ky_thread();

    //!
    //! \brief policy 线程的策略
    //! \param tp
    //! \return
    //!
    bool policy(eThreadPolicys tp);
    eThreadPolicys policy()const;

    //!
    //! \brief priority 线程的优先权
    //! \param prio
    //! \return
    //!
    bool priority(eThreadPrioritys prio);
    eThreadPrioritys priority() const;

    //!
    //! \brief run 需要执行的线程，继承
    //!
    virtual void run() = 0;

    //!
    //! \brief start 启动一个线程来执行run函数，若在运行则什么都不做
    //! \param tp
    //! \return
    //!
    bool start(eThreadPolicys tp = Policy_Default);

    //!
    //! \brief is_running 返回线程是否在运行
    //! \return
    //!
    bool is_running() {return __running;}

    //!
    //! \brief wait 等待线程运行结束，若没有运行则直接返回
    //! \return
    //!
    bool wait(size_t timeout = kyTimeoutIndefinite);

    //!
    //! \brief finish  等待线程运行结束
    //! \return
    //!
    bool finish();

    //!
    //! \brief cancel  取消线程，不安全请勿使用
    //! \return
    //!
    bool cancel(); //terminate

    //!
    //! \brief id 回线程id
    //! \return
    //!
    thread_id id()const;

    //!
    //! \brief operator 比较两个线程是否相同
    //! \param rhs
    //! \return
    //!
    bool operator != (const ky_thread &rhs)const;
    bool operator == (const ky_thread &rhs)const;

public:
    //!
    //! \brief sleep 睡眠s秒
    //! \param s
    //!
    static void sleep(uint s);//s
    //!
    //! \brief msleep 睡眠ms毫秒
    //! \param ms
    //!
    static void msleep(ulong ms);//ms
    //!
    //! \brief usleep 睡眠um微妙
    //! \param um
    //!
    static void usleep(size_t um);//us
    //!
    //! \brief yield 出让执行权
    //!
    static void yield();
    //!
    //! \brief current 返回当前线程
    //! \return
    //!
    static ky_thread *current();
    //!
    //! \brief current_id 返回当前线程id
    //! \return
    //!
    static thread_id current_id ();
    //!
    //! \brief pid 返回当前进程id
    //! \return
    //!
    static process_id pid();

    #ifdef hasThreadLocalStorage
    //!
    //! \brief tls_set 设置线程TLS的数据
    //! \param td
    //!
    static void local_storage_set(tThreadLocalStorages * td);
    //!
    //! \brief tls_get 获取线程TLS的数据
    //! \return
    //!
    static tThreadLocalStorages *local_storage_get();
    //!
    //! \brief tls_clear 清除线程TLS数据
    //!
    static void local_storage_clear();
    #endif

private:
    eThreadPrioritys __priority;
    int __priority_min;
    int __priority_max;
    eThreadPolicys __policy;
};

//!
//! \brief The ky_nonthreadsafe class
//! @class ky_nonthreadsafe
//! 线程非安全指示
//!
class ky_nonthreadsafe
{
public:
    ky_nonthreadsafe(){}
    virtual ~ky_nonthreadsafe(){}

    bool is_safe(){return true;}
protected:
    void detach(){}
    void ensure_id_valid(){}
};
//!
//! \brief The ky_threadsafe class
//! @class ky_threadsafe
//! 线程安全检测
//!
class ky_threadsafe
{
public:
    ky_threadsafe();
    virtual ~ky_threadsafe(){}

    bool is_safe();

protected:
    void detach();
    void ensure_id_valid();

private:
    thread_id valid_id;
    ky_mutex mutex;
};

#endif
