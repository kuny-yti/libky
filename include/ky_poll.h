/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_poll.h
 * @brief    系统fd事件轮询对象
 *       1.
 *       2.
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.1.1
 * @date     2018/06/27
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2018/06/27 | 1.0.0.1   | kunyang  | 创建文件
 */
#ifndef KY_POLL_H
#define KY_POLL_H

#include "ky_define.h"

typedef int poll_fd_t;
namespace impl {
struct poll;
}
class ky_poll
{
public:
    //!
    //! \brief ky_poll 构造可用于调度可取消超时的轮询POLL
    //! \param controllable 是否控制等待。(多个超时可以从不同的线程执行。)
    //! \note 可以分别使用restart（）和set_flushing（）重新启动或清除对wait（）的调用。
    //!
    explicit ky_poll(bool controllable);
    virtual ~ky_poll();

    //!
    //! \brief append 将文件描述符添加到POLL
    //! \param fd
    //! \return
    //!
    bool append (poll_fd_t fd);
    //!
    //! \brief remove
    //! \param id
    //! \return
    //!
    bool remove (poll_fd_t fd);

    //!
    //! \brief ctl_write 控制poll拥有可写性
    //! \param fd
    //! \param active
    //! \return
    //!
    bool ctl_write (poll_fd_t fd, bool active);
    //!
    //! \brief ctl_read 控制poll拥有可读性
    //! \param fd
    //! \param active
    //! \return
    //!
    bool ctl_read (poll_fd_t fd, bool active);

    //!
    //! \brief has_close 是否已关闭连接
    //! \param fd
    //! \return
    //!
    bool is_close (poll_fd_t fd);

    //!
    //! \brief has_error 是否有错误
    //! \param fd
    //! \return
    //!
    bool is_error (poll_fd_t fd);

    //!
    //! \brief can_write 是否可以写入
    //! \param fd
    //! \return
    //!
    bool can_write (poll_fd_t fd);
    //!
    //! \brief can_read 是否有要读取的数据
    //! \param fd
    //! \return
    //!
    bool can_read (poll_fd_t fd);

    //!
    //! \brief ignored
    //! \param fd
    //!
    void ignored (poll_fd_t fd);
    //!
    //! \brief set_controllable
    //! \param controllable 新的可控状态。
    //! \note 当@controllable为true时，此函数确保未来对wait（）的调用将受到restart（）
    //!       和flushing（）的影响。
    //! \note 此函数仅适用于使用创建的非定时器
    //! \return
    //!
    bool set_controllable (bool controllable);

    //!
    //! \brief wait 等待FD中产生活动,直至超时或退出
    //! \param timeout 以纳秒为单位的超时。
    //! \note 此函数只能是一次从一个线程调用
    //! \return -1 error, -2 从多个线程调用. 返回活动的数量
    //!
    int wait (int64_t timeout = - 1);

    //!
    //! \brief restart 重新启动正在进行的任何wait（）。这个函数通常在添加或删除描述符后使用。
    //!
    void restart ();

    //!
    //! \brief set_flushing 当@flushing为TRUE时，此函数确保当前和未来对wait（）的调用将返回-1，
    //!                     并将errno设置为EBUSY。
    //! \param flushing
    //!
    void set_flushing (bool flushing);

    //!
    //! \brief write_control 将一个字节写入可控的控制套接字。
    //! \return
    //!
    bool write_control ();

    //!
    //! \brief read_control 从控制套接字中读取一个字节。
    //! \return
    //!
    bool read_control ();

protected:
    bool add_fd_unlocked (poll_fd_t fd);
    bool ctl_read_unlocked (poll_fd_t fd, bool active);
    bool can_read_unlocked (poll_fd_t fd);

private:
    impl::poll *impl;
};

#endif // KY_POLL_H
