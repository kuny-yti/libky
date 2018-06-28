
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
#ifndef ky_URL
#define ky_URL

#include "ky_string.h"
#include "ky_stream.h"

typedef enum
{
    Protocol_File,
    Protocol_Ftp,
    Protocol_Gopher ,
    Protocol_Http ,
    Protocol_Https ,
    Protocol_Mailto ,
    Protocol_MMS ,
    Protocol_Telnet ,
    Protocol_News ,
    Protocol_Socket,
    Protocol_UrlNot
}eUrlProtocols;

class ky_url
{
public:
    ky_url();
    explicit ky_url(const ky_string& url);
    ~ky_url();

    bool is_empty()const;
    bool is_valid()const;

    eUrlProtocols protocol()const;

    bool resolve (const ky_string& url)const;
    ky_string package()const;

    ky_string scheme()const;
    ky_string host()const;
    ky_string port()const;
    ky_string path()const;
    ky_string query()const;
    ky_string fragment()const;
    ky_string username()const;
    ky_string password()const;
    ky_string url() const;

    bool operator == (const ky_url &rhs)
    {
        return url_ == rhs.url_;
    }
    bool operator != (const ky_url &rhs)
    {
        return url_ != rhs.url_;
    }

private:
    ky_string url_;

    ky_string scheme_;
    ky_string host_;
    ky_string port_;
    ky_string path_;
    ky_string query_;
    ky_string fragment_;
    ky_string username_;
    ky_string password_;
};

ky_streamb &operator << (ky_streamb &in, const ky_url &v);
ky_streamb &operator >> (ky_streamb &out, ky_url &v);
#endif // ky_URL

