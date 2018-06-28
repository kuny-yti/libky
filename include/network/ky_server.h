
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_setver.h
 * @brief    socket 的服务器实现
 *       1.支持UDP，TCP协议.
 *       2.支持线程检测服务器动态
 *       3.内部为非阻塞模式(linux为epoll机制)
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.1.1.0
 * @date     2013/04/12
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2013/04/12 | 1.0.0.1   | kunyang  | 创建文件
 * 2013/04/14 | 1.0.1.0   | kunyang  | 完善服务器实现
 * 2014/06/09 | 1.1.0.1   | kunyang  | 修改服务器工作模式并修改客户端连接和断开通过继承虚函数
 * 2015/01/03 | 1.1.1.0   | kunyang  | 服务器实现linux的epoll机制
 */
#ifndef ky_SERVER
#define ky_SERVER

#include "interface/isocket.h"
#include "ky_thread.h"
#include "ky_ptr.h"

namespace impl {
struct server;
}
class ky_server :public ky_thread
{
public:
    explicit ky_server(uint max_client = 10000);
    ky_server(eNetworkSockets mode, uint max_client = 10000);
    virtual ~ky_server();

    // start
    bool listen(const ky_string &ip, uint16 port, int count = 20);
    bool listen(uint16 port, bool is_ipv6 = false, int count = 20);
    bool listen(const ky_netaddr &ip, int count = 20);

    // stop
    void revert ();

    ky_netaddr address()const;

    isocket::setup *option();
    pisocket &socket();

protected:
    virtual void remote_link(const pisocket &sock, const ky_netaddr &add) = 0;
    void remote_unlink(const pisocket &sock);

protected:
    virtual void run();

private:
    impl::server *impl;
};

#endif // ky_SERVER

