
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
#ifndef KY_DEFINE_H
#define KY_DEFINE_H

#include "ky_type.h"

#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cfloat>
#include <stdint.h>
#include <limits.h>
#include <typeinfo>


#ifndef kyVersionNumber
#define kyVersionNumber(major, minor, path)\
    ((major << 16 & 0xffffff) | (minor << 8 & 0xffff) | path & 0xff)
#endif

#ifndef kyVersionStr
#define kyVersionStr(major, minor, path) \
    (#major"."#minor"."#path)
#endif

#ifndef kyFloatBit
#define kyFloatBit 4
#endif

#ifndef kyFloatAcc
#define kyFloatAcc 0.0000001
#endif

#ifndef kyEqualf
#define kyEqualf(v1 ,v2) (((v1 + kyFloatAcc) > v2) && ((v1 - kyFloatAcc) < v2))
#endif

#ifndef kyUnEqualf
#define kyUnEqualf(v1 ,v2) (!kyEqualf(v1 ,v2))
#endif

#ifndef kyEmptyf
#define kyEmptyf(v1) ((v1 < kyFloatAcc) && (v1 > -kyFloatAcc))
#endif

#ifndef kyHasUTF8
#define kyHasUTF8 1
#endif

#ifndef kyHasUTF16
#define kyHasUTF16 1
#endif

#ifndef kyHasUTF32
#define kyHasUTF32 (sizeof(wchar_t) != sizeof(unsigned short))
#endif

#ifndef kyPackage
#define kyPackage struct
#endif

#ifdef kyPackage
#define kyAsCast(type) dynamic_cast<type>
#else
#define kyAsCast(type) (type)
#endif

#ifndef kyArraySizeof
///< 计算固定数组的数量
#define kyArraySizeof(array) (sizeof(array) / sizeof(array[0]))
#endif

///< 修饰变量，类型，函数
/**
  * 1.变量修饰: int a __attribute__((aligned(8))) = 0;
  * 2.类型修饰: struct test {int a;} __attribute__((aligned(8)));
  * 3.函数修饰: void test(void) __attribute__((aligned(2)));
***/
#define kyQualifyAligned(byte) __attribute__ ((aligned(byte)))

/// <将函数强制为内联函数
/**
  * static inline void test2(void) __attribute__((always_inline));
***/
#define kyQualifyAlwaysInline __attribute__((always_inline))

///< 修饰结构体，枚举，联合体的字节数为紧凑结构
#define kyQualifyPacked __attribute__ ((packed))
#define kyQualifyNoReture __attribute__((noreturn))

///< 在main之前执行此函数
#define kyQualifyMainBefore(level) __attribute__((constructor(100+level)))
///< 在main退出后执行此函数
#define kyQualifyMainAfter(level) __attribute__((destructor(100+level)))

#if (kyCompiler == kyCompiler_GNUC) || (kyCompiler == kyCompiler_CLANG)

#define kyLikely(expr) (__builtin_expect (!!(expr), 1))
#define kyUnLikely(expr) (__builtin_expect (!!(expr), 0))

#else
#define kyLikely(expr)  (!!(expr) ? 1 : 0)
#define kyUnLikely(expr) (!!(expr) ? 0 : 1)

#endif

#ifdef kyLanguage
#define _in const
#define _out &
#define _inout &
#define _fix const
#define _ref &
#else
#define _in const
#define _out *
#define _inout *
#define _fix const
#define _ref *
#endif

///< 执行特定次数
#define repeat(num) for (size_t __i___ = 0; __i___ < (size_t)num; ++__i___)

///< 执行条件型 (加入条件为了防止死循环)
#define forever(cond) for (;cond;)

///< 容器操作型 (c++98 不能操作数组类型)
#if kyLanguage >= kyLanguage11
#define foreach(var, container) for (var : container)
#else

struct kyContainerBase{
    virtual bool condition()const = 0;
    virtual void operator ++()const = 0;
    virtual void operator --()const = 0;
    virtual void operator ++(int)const = 0;
    virtual void operator --(int)const = 0;

    virtual bool cond_less()const =0;
    virtual bool cond_add()const =0;
};
template <typename T>
class kyContainerOpe :public kyContainerBase
{
    kyContainerOpe & operator=(const kyContainerOpe &) = delete;
public:
    inline kyContainerOpe(const T & t):
        cond(0),
        container(t),
        b(t.begin()),
        e(t.end()){}
    kyContainerOpe(const kyContainerOpe & rhs):
        cond(rhs.cond),
        container(rhs.container),
        b(rhs.b),
        e(rhs.e) {}

    bool condition()const {return b != e && !cond_add();}
    void operator ++()const {++b;}
    void operator --()const  {--b;}
    void operator ++(int)const {++b;}
    void operator --(int)const {--b;}

    bool cond_less()const {kyContainerOpe* co = (kyContainerOpe*)this; return co->cond > 0 ? co->cond--: false;}
    bool cond_add()const {kyContainerOpe* co = (kyContainerOpe*)this; return co->cond++;}

    int cond;
    const T container;
    mutable typename T::iterator b, e;
};
template <typename T>
T *kyContainerAffirm(const T &) {return NULL;}
template <typename T>
kyContainerOpe<T> kyContainerNew(const T & conta) {return kyContainerOpe<T>(conta);}
template <typename T>
const kyContainerOpe<T> *kyContainerAs(const kyContainerBase * fc, const T *) {return (const kyContainerOpe<T>*)fc;}
#define foreach(val, container) \
    for (const kyContainerBase &_conta_ = kyContainerNew(container); _conta_.condition(); ++_conta_) \
    for (val = *kyContainerAs(&_conta_, kyContainerAffirm(container))->b; _conta_.cond_less() ; )

#endif


#ifndef kyHasReferCount
struct ky_ref{};
#else
#include "ky_atomic.h"
/*!
 * \brief The ky_ref struct
 */
struct ky_ref
{
    /*!
     * \brief 引用标志定义
     */
    enum eRefFlags
    {
        refStatic = -1,                                ///< 静态模式，不支持引用
        refShareable = 0,                              ///< 可共享模式，用于对象间引用
        refOne = 1,                                    ///< 将当前引用置为1，
        refDetach = 0x40000000,                        ///< 支持分离
        refMask = 0xbfffffff,                          ///< 标志提取掩码
        refShareableDetach = refShareable | refDetach, ///< 可共享且可分离
        refOneDetach = refDetach | refOne,             ///< 可共享、可分离并且将引用置为1
    };

    ky_ref(eRefFlags flag = refOneDetach):ref((int)flag){}
    ~ky_ref(){}

    inline ky_ref &operator = (const ky_ref&rhs)
    {
        ref = rhs.ref;
        return *this;
    }

    //!
    //! \brief count 返回引用的次数
    //! \return
    //!
    inline int count()const {return (ref & int(refMask));}
    inline operator int()const{return (ref & int(refMask));}
    inline operator int(){return (ref & int(refMask));}
    //!
    //! \brief set 设置引用标志
    //! \param flag
    //!
    inline void set(eRefFlags flag = refStatic){ref.store ((int)flag, Fence_Release);}
    inline ky_ref & operator = (eRefFlags flags){ref.store ((int)flags, Fence_Release);return *this;}
    //!
    //! \brief is_shared 返回是否有引用共享
    //! \return
    //!
    inline bool is_shared()const
    {
        return ((ref & int(refMask)) > (int)refShareable);
    }
    //!
    //! \brief is_static 返回是否为静态引用
    //! \return
    //!
    inline bool is_static()const{return ref == (int)refStatic;}
    //!
    //! \brief has_shareable 返回是否启用引用共享
    //! \return
    //!
    inline bool has_shareable()const
    {
        return (ref & int(refMask)) >= (int)refShareable;
    }

    //!
    //! \brief has_detach 返回是否支持分离
    //! \return
    //!
    inline bool has_detach()const{return ref & (int)refDetach;}

    //!
    //! \brief addref 添加引用计数
    //!
    inline void addref() {if (has_shareable()) ++ref;}
    //!
    //! \brief lessref 释放引用计数
    //! \return
    //!
    inline bool lessref()
    {
        if (!(ref & int(refMask)) || (--ref) < 0)
            return true;
        return false;
    }
    //!
    //! \brief is_destroy 返回是否需要释放对象
    //! \return
    //!
    inline bool is_destroy()
    {
        if (!(ref & int(refMask)) || ref < 0)
            return true;
        return false;
    }
    inline ky_ref &operator ++(){++ref;return *this;}
    inline ky_ref operator ++(int){ky_ref tret = *this; ref++;return tret;}
    inline ky_ref &operator --(){--ref; return *this;}
    inline ky_ref operator --(int){ky_ref tret = *this;ref--; return tret;}

    ky_atomic<int> ref;
};

#endif


#endif // KY_DEFINE_H
