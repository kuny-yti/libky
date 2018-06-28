
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_variant.h
 * @brief    数据类型的变种类型
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.1.0.1
 * @date     2015/04/08
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2015/04/08 | 1.0.0.1   | kunyang  | 创建文件
 * 2015/07/23 | 1.0.1.1   | kunyang  | 修改字符串存储
 * 2016/03/08 | 1.0.2.1   | kunyang  | 修改>>、<<符号操作
 * 2018/02/18 | 1.1.0.1   | kunyang  | 重构类并独立
 */
#ifndef KY_VARIANT_H
#define KY_VARIANT_H

#include "ky_type.h"
#include "ky_array.h"

/*!
 * @brief The ky_variant struct 解析器获取的数据
 * @struct ky_variant
 */
typedef struct ky_variant
{
    ~ky_variant(){}
    ky_variant(){}
    ky_variant(const ky_variant &pd)
    {
        type = pd.type;
        data = pd.data;
    }
    ky_variant &operator = (const ky_variant &pd)
    {
        data = pd.data;
        type = pd.type;
        return *this;
    }

    void swap(ky_variant &b){type.swap (b.type);data.swap (b.data);}
    friend void ky_swap(ky_variant &a, ky_variant &b) {a.swap(b);}

    friend ky_variant & operator << (ky_variant &, bool);
    friend ky_variant & operator << (ky_variant &, bool_t);
    friend ky_variant & operator << (ky_variant &, char);
    friend ky_variant & operator << (ky_variant &, uchar);
    friend ky_variant & operator << (ky_variant &, short);
    friend ky_variant & operator << (ky_variant &, ushort);
    friend ky_variant & operator << (ky_variant &, int);
    friend ky_variant & operator << (ky_variant &, uint);
    friend ky_variant & operator << (ky_variant &, long);
    friend ky_variant & operator << (ky_variant &, ulong);
    friend ky_variant & operator << (ky_variant &, longlong);
    friend ky_variant & operator << (ky_variant &, ulonglong);
    friend ky_variant & operator << (ky_variant &, half_float);
    friend ky_variant & operator << (ky_variant &, float);
    friend ky_variant & operator << (ky_variant &, real);

    friend ky_variant & operator >> (ky_variant &, bool & );
    friend ky_variant & operator << (ky_variant &, bool_t &);
    friend ky_variant & operator >> (ky_variant &, char & );
    friend ky_variant & operator >> (ky_variant &, uchar & );
    friend ky_variant & operator >> (ky_variant &, short & );
    friend ky_variant & operator >> (ky_variant &, ushort & );
    friend ky_variant & operator >> (ky_variant &, int & );
    friend ky_variant & operator >> (ky_variant &, uint & );
    friend ky_variant & operator >> (ky_variant &, long & );
    friend ky_variant & operator >> (ky_variant &, ulong & );
    friend ky_variant & operator >> (ky_variant &, longlong & );
    friend ky_variant & operator >> (ky_variant &, ulonglong & );
    friend ky_variant & operator >> (ky_variant &, float & );
    friend ky_variant & operator << (ky_variant &, half_float &);
    friend ky_variant & operator >> (ky_variant &, real & );

    void set(const char* t);
    bool is_set(const char* t);
    bool is_valid() const;

    ky_array<char>  type;
    ky_array<uint8> data;
}ky_variant;

#endif
