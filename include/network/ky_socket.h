
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_socket.h
 * @brief    socket 基本对象，根据协议链接服务器
 *       1.支持IPv4和IPv6.
 *       2.支持TCP和UDP协议
 *       3.可以返回socket的底层接口用于对socket进行设置
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.3.1
 * @date     2012/02/02
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2012/02/02 | 1.0.0.1   | kunyang  | 创建文件支持IPv4操作
 * 2014/04/10 | 1.0.1.0   | kunyang  | 修改socket类并建立socket虚对象
 * 2014/04/12 | 1.0.2.0   | kunyang  | 加入多协议链接和基础socket对象引出
 * 2015/03/09 | 1.0.3.1   | kunyang  | 重构类并和服务器建立关系
 */
#ifndef ky_SOCKET
#define ky_SOCKET

#include "interface/isocket.h"
#include "tools/ky_string.h"
#include "ky_ptr.h"

namespace impl {
struct socket;
}

class ky_socket
{
public:
    explicit ky_socket(const eNetworkSockets mode);
    explicit ky_socket(const pisocket &sock, const ky_netaddr &remote);
    virtual ~ky_socket();

    //!
    //! \brief local_addr 返回socket本地地址
    //! \return
    //!
    ky_netaddr local_addr() const;
    //!
    //! \brief remote_addr 返回socket远程地址
    //! \return
    //!
    ky_netaddr remote_addr()const;

    //!
    //! \brief is_valid 返回socket是否有效
    //! \return
    //!
    bool is_valid()const;

    //!
    //! \brief option 套接字的选项设置
    //! \return
    //!
    isocket::setup *option();
    //!
    //! \brief socket 套接字接口
    //! \return
    //!
    pisocket &socket();

public:
    virtual bool connect(const ky_string &host, uint16 port)
    {
        return connect (ky_netaddr(host, port));
    }
    virtual bool connect(const ky_netaddr &dst);
    virtual void disconnect();
    virtual bool is_connect()const;

    virtual size_t read(void *data, size_t maxlen)const;
    virtual size_t write(void *data, size_t len)const;
    virtual size_t read(void *data, size_t maxlen, ky_netaddr *ip );
    virtual size_t write(void *data, size_t len, const ky_netaddr &ip);

private:
    impl::socket *impl;
};

#endif // ky_SOCKET

