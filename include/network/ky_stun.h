
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
#ifndef KY_STUN_H
#define KY_STUN_H

#include "ky_define.h"

const static short kyStunServerPort = 3478;
const static short kyStunServerTlsPort = 5349;

class ky_stun
{
protected:
    class server
    {
    public:
        server (bool is_tcp = false);
        ~server ();

        bool begin();
        void end();

    private:
        struct srv *impl;
    };
    class client
    {
    public:

    };

public:
    ky_stun(bool is_svr, bool is_tcp = false);
    virtual ~ky_stun();

    bool is_server()const{return svr_state;}
    bool is_tcp()const{return tcp_state;}

protected:
    bool svr_state;
    bool tcp_state;
    union
    {
        server *server_stun;
        client *client_stun;
    };
};

#endif
