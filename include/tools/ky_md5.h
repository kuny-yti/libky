
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
#ifndef ky_MD5
#define ky_MD5

#include "ky_define.h"
#include "ky_string.h"

class ky_md5
{
public:
    ky_md5();
    ky_md5(const void *input, size_t length);
    ky_md5(const ky_string &str);

    void update(const void *input, size_t length);
    void update(const ky_string &str);

    const uchar *digest();
    ky_string to_string();

    void reset();

private:
    void update(const uchar *input, size_t length);

    void final();
    void transform(const uchar block[64]);

    void encode(const uint32_t *input, uchar *output, size_t length);
    void decode(const uchar *input, uint32_t *output, size_t length);

    ky_string byte_hex(const uchar *input, size_t length);

    /* class uncopyable */
    ky_md5(const ky_md5&);
    ky_md5& operator=(const ky_md5&);

private:
    uint32_t _state[4];	/* state (ABCD) */
    uint32_t _count[2];	/* number of bits, modulo 2^64 (low-order word first) */
    uchar _buffer[64];	/* input buffer */
    uchar _digest[16];	/* message digest */
    bool _finished;		/* calculate finished ? */

    static const uchar PADDING[64];	/* padding for calculate */
    static const char HEX[16];
    static const size_t BUFFER_SIZE = 1024;

};
#endif // ky_MD5

