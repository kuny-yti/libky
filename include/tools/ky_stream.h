
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
#ifndef ky_streamb_H
#define ky_streamb_H

#include "ky_vector.h"
#include "ky_memory.h"
#include "ky_array.h"

typedef int eByteOrders;
struct tStreamDatas
{
    eByteOrders border;
    ky_byte *array;

    virtual void put(char* val, size_t len) = 0;
    virtual char *take(int *len = NULL) = 0;

    virtual void dump() {}
    virtual char split()const{return 0;}

    template <typename T>
    T swap_endian(T &v);
};

struct tStreamBinarys : tStreamDatas
{
    virtual void put(char* val, size_t len);
    virtual char *take(int *len = NULL);

    virtual void dump();
};
struct tStreamTexts : tStreamDatas
{
    virtual char *take(int *len = NULL);
    virtual void put(char* val, size_t len);
    virtual void dump() ;
    virtual char split()const;
};

/*!
 * @brief The ky_stream class
 * @class ky_stream
 */
template <typename OpeT>
class ky_stream : public OpeT
{
public :
    explicit ky_stream(const ky_byte &);
    explicit ky_stream(ky_byte *);

    virtual ~ky_stream();
    size_t size()const;

public:
    friend ky_stream & operator << (ky_stream &, const ky_stream &);
    friend ky_stream & operator >> (ky_stream &, ky_stream &);
    friend ky_stream & operator << (ky_stream &, const ky_byte &);
    friend ky_stream & operator >> (ky_stream &, ky_byte &);

protected:
    tStreamDatas *impl;
};

#include "ky_stream.inl"
class ky_streamb : public ky_stream<tStreamBinarys>
{
public:
    ky_streamb();
    explicit ky_streamb(const ky_byte &);
    explicit ky_streamb(ky_byte *);

public:
    friend ky_streamb & operator << (ky_streamb &, bool);
    friend ky_streamb & operator << (ky_streamb &, bool_t);
    friend ky_streamb & operator << (ky_streamb &, char);
    friend ky_streamb & operator << (ky_streamb &, uchar);
    friend ky_streamb & operator << (ky_streamb &, short);
    friend ky_streamb & operator << (ky_streamb &, ushort);
    friend ky_streamb & operator << (ky_streamb &, int);
    friend ky_streamb & operator << (ky_streamb &, uint);
    friend ky_streamb & operator << (ky_streamb &, long);
    friend ky_streamb & operator << (ky_streamb &, ulong);
    friend ky_streamb & operator << (ky_streamb &, longlong);
    friend ky_streamb & operator << (ky_streamb &, ulonglong);
    friend ky_streamb & operator << (ky_streamb &, float);
    friend ky_streamb & operator << (ky_streamb &, half_float);
    friend ky_streamb & operator << (ky_streamb &, real);

    friend ky_streamb & operator >> (ky_streamb &, bool & );
    friend ky_streamb & operator >> (ky_streamb &, bool_t & );
    friend ky_streamb & operator >> (ky_streamb &, char & );
    friend ky_streamb & operator >> (ky_streamb &, uchar & );
    friend ky_streamb & operator >> (ky_streamb &, short & );
    friend ky_streamb & operator >> (ky_streamb &, ushort & );
    friend ky_streamb & operator >> (ky_streamb &, int & );
    friend ky_streamb & operator >> (ky_streamb &, uint & );
    friend ky_streamb & operator >> (ky_streamb &, long & );
    friend ky_streamb & operator >> (ky_streamb &, ulong & );
    friend ky_streamb & operator >> (ky_streamb &, longlong & );
    friend ky_streamb & operator >> (ky_streamb &, ulonglong & );
    friend ky_streamb & operator >> (ky_streamb &, float & );
    friend ky_streamb & operator >> (ky_streamb &, half_float & );
    friend ky_streamb & operator >> (ky_streamb &, real & );

    friend ky_streamb & operator << (ky_streamb &in, const ky_streamb &v);
    friend ky_streamb & operator >> (ky_streamb &out, ky_streamb &v);
    friend ky_streamb & operator << (ky_streamb &, const ky_byte &);
    friend ky_streamb & operator >> (ky_streamb &, ky_byte &);

};

class ky_streamt : public ky_stream<tStreamTexts>
{
public:
    explicit ky_streamt(const ky_byte &);
    explicit ky_streamt(ky_byte *);
public:
    friend ky_streamt & operator << (ky_streamt &, bool);
    friend ky_streamt & operator << (ky_streamt &, bool_t);
    friend ky_streamt & operator << (ky_streamt &, char);
    friend ky_streamt & operator << (ky_streamt &, uchar);
    friend ky_streamt & operator << (ky_streamt &, short);
    friend ky_streamt & operator << (ky_streamt &, ushort);
    friend ky_streamt & operator << (ky_streamt &, int);
    friend ky_streamt & operator << (ky_streamt &, uint);
    friend ky_streamt & operator << (ky_streamt &, long);
    friend ky_streamt & operator << (ky_streamt &, ulong);
    friend ky_streamt & operator << (ky_streamt &, longlong);
    friend ky_streamt & operator << (ky_streamt &, ulonglong);
    friend ky_streamt & operator << (ky_streamt &, float);
    friend ky_streamt & operator << (ky_streamt &, half_float);
    friend ky_streamt & operator << (ky_streamt &, real);

    friend ky_streamt & operator << (ky_streamt &, char *);
    friend ky_streamt & operator << (ky_streamt &, const char *);

    friend ky_streamt & operator >> (ky_streamt &, bool & );
    friend ky_streamt & operator >> (ky_streamt &, bool_t & );
    friend ky_streamt & operator >> (ky_streamt &, char & );
    friend ky_streamt & operator >> (ky_streamt &, uchar & );
    friend ky_streamt & operator >> (ky_streamt &, short & );
    friend ky_streamt & operator >> (ky_streamt &, ushort & );
    friend ky_streamt & operator >> (ky_streamt &, int & );
    friend ky_streamt & operator >> (ky_streamt &, uint & );
    friend ky_streamt & operator >> (ky_streamt &, long & );
    friend ky_streamt & operator >> (ky_streamt &, ulong & );
    friend ky_streamt & operator >> (ky_streamt &, longlong & );
    friend ky_streamt & operator >> (ky_streamt &, ulonglong & );
    friend ky_streamt & operator >> (ky_streamt &, float & );
    friend ky_streamt & operator >> (ky_streamt &, half_float & );
    friend ky_streamt & operator >> (ky_streamt &, real & );

    friend ky_streamt & operator << (ky_streamt &in, const ky_streamt &v);
    friend ky_streamt & operator >> (ky_streamt &out, ky_streamt &v);
    friend ky_streamt & operator << (ky_streamt &, const ky_byte &);
    friend ky_streamt & operator >> (ky_streamt &, ky_byte &);
private:
    void numput(longlong val, int bs = 10);
    void unumput(ulonglong val, int bs = 10);
    void fltput(real val, int prec = 4);

    template<typename T>
    T unumtake(int bs = 10);
    template<typename T>
    T numtake(int bs = 10);
};

#endif // ky_streamb_H
