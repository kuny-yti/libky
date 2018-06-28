
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_bitset.h
 * @brief    位操作类最大支持64位
 *       1.位操作支持bin宏实现的ky_binary对象.
 *       2.支持运算符操作
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.3.0
 * @date     2016/04/11
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2016/04/11 | 1.0.0.1   | kunyang  | 创建文件实现ky_bitset
 * 2016/04/12 | 1.0.1.0   | kunyang  | 修改内部操作位时使用BIT方法
 * 2017/01/07 | 1.0.2.0   | kunyang  | 实现基本的运算符操作
 * 2017/04/29 | 1.0.3.0   | kunyang  | 实现bin宏模式赋值
 */
#ifndef ky_BITSET_H
#define ky_BITSET_H

#include "ky_define.h"
#include "ky_string.h"
#include "ky_binary.h"
/*!
 * \brief The eBitsets enum
 */
typedef enum
{
#ifdef FALSE
#undef FALSE
#endif
    FALSE = 0,
#ifdef TRUE
#undef TRUE
#endif
    TRUE = 1,

    LOW = 0,
    HIGH = 1,

    OFF = 0,
    ON = 1,

    NO = 0,
    YES = 1
} eBitsets;

/*!
 * @brief The ky_bitset class
 * @class ky_bitset
 */
template <int tBits>
class ky_bitset
{
public:
    enum{bits = tBits, size = tBits};

    ky_bitset(uint64 v = 0);
    ky_bitset(const ky_bitset<tBits> &rhs);

    ky_bitset<tBits> &operator = (const ky_binary &v){form_string (v.bc);return *this;}
private:
    struct BIT
    {
        uint  ob:8;
        uint64 v:tBits;

        BIT &operator = (eBitsets b);
        uint64 operator ()()const{return v;}
        operator uint64()const{return v;}
        operator bool() const {return v & ((1 << ob) & ~(0));}
        bool operator == (eBitsets b)const;
        bool operator != (eBitsets b)const;
    }kyQualifyPacked;

public:
    //!
    //! \brief operator 重载按位与、或、非、异或运算
    //! \param rhs
    //! \return  返回一个运算结果
    //!
    ky_bitset<tBits> operator & (const ky_bitset<tBits> &rhs)const;
    ky_bitset<tBits> operator | (const ky_bitset<tBits> &rhs)const;
    ky_bitset<tBits> operator ~ ()const;
    ky_bitset<tBits> operator ^ (const ky_bitset<tBits> &rhs)const;

    //!
    //! \brief operator 重载按位与、或、非、异或运算,并改变自身值
    //! \param rhs
    //! \return
    //!
    ky_bitset<tBits> &operator &= (const ky_bitset<tBits> &rhs);
    ky_bitset<tBits> &operator |= (const ky_bitset<tBits> &rhs);
    ky_bitset<tBits> &operator ~ ();
    ky_bitset<tBits> &operator ^= (const ky_bitset<tBits> &rhs);

    //!
    //! \brief operator 重载左右移位操作
    //! \param rhs
    //! \return
    //!
    ky_bitset<tBits> operator << (uint b)const;
    ky_bitset<tBits> operator >> (uint b)const;
    ky_bitset<tBits> &operator <<= (uint b);
    ky_bitset<tBits> &operator >>= (uint b);

    //!
    //! \brief operator 重载逻辑运算
    //! \param rhs
    //! \return
    //!
    bool operator || (const ky_bitset<tBits> &rhs)const;
    bool operator && (const ky_bitset<tBits> &rhs)const;
    bool operator ! ()const;

    bool operator == (const ky_bitset<tBits> &rhs);
    bool operator != (const ky_bitset<tBits> &rhs);

public:
    //!
    //! \brief operator []  利用下标方法修改指定位的值
    //! \param bit
    //! \return
    //!
    BIT &operator [](uint bit);
    const BIT &operator [](uint bit)const;

    uint64 value()const{return val.v;}
    uint64 operator ()(){return value();}
    operator uint64(){return value();}

public:
    //!
    //! \brief set 所有位置为１
    //!
    void set() ;
    //!
    //! \brief set 将bit位置为１
    //!
    void set(uint bit) ;
    //!
    //! \brief set 所有位置为0
    //!
    void unset();
    //!
    //! \brief set 将bit位置为0
    //!
    void unset(uint bit);

    //!
    //! \brief any 返回是否有有效位
    //! \return
    //!
    bool any()const;
    //!
    //! \brief none 返回是否全为无效位
    //! \return
    //!
    bool none()const;
    //!
    //! \brief count 返回有效位数
    //! \return
    //!
    int count()const;

    //!
    //! \brief flip 所有位翻转
    //!
    void flip();
    //!
    //! \brief flip 将bit位翻转
    //!
    void flip(uint bit);

    //!
    //! \brief  字符串格式互相转换
    //! \return
    //!
    ky_string to_string()const;
    void form_string(const ky_string &s);

    void swap(ky_bitset &b);
    friend void ky_swap(ky_bitset &a, ky_bitset &b) {a.swap(b);}

private:
        BIT val;
};


#include "ky_bitset.inl"
#endif
