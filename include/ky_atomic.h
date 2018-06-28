
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_atomic.h
 * @brief    系统原子操作，支持x86,arm,gcc,clang
 *       1.完整gcc原子操作并兼容clang
 *       2.支持1,2,4,8bit的类型操作
 *       3.支持指针类型操作
 *       4.可在编译阶段断言系统是否支持以上类型
 *       5.同时支持arm和x86原子操作
 *       6.内存序操作保证(内存屏障)
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.3.2.2
 * @date     2012/04/01
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2012/04/01 | 1.0.0.1   | kunyang  | 创建文件
 * 2012/04/13 | 1.0.1.0   | kunyang  | 使用gcc完成简单的原子操作
 * 2013/08/10 | 1.1.0.1   | kunyang  | 修改gcc原子操作加入内存屏障
 * 2014/02/05 | 1.1.1.0   | kunyang  | 修改gcc原子操作兼容clang并简化原子运算符
 * 2014/05/25 | 1.1.2.1   | kunyang  | 加入原子指针操作和8bit原子操作
 * 2015/08/20 | 1.2.0.1   | kunyang  | 重构原子操作类加入通用原子类，定义了内存序模式
 * 2015/08/23 | 1.2.1.1   | kunyang  | 加入x86原子操作汇编
 * 2016/10/07 | 1.3.0.1   | kunyang  | 加入arm原子操作并修改了原子指针模板类
 * 2017/08/20 | 1.3.1.1   | kunyang  | 在类中加入编译器编译断言
 * 2018/02/22 | 1.3.2.1   | kunyang  | 修改比较交换引用错误
 * 2018/03/09 | 1.3.2.2   | kunyang  | 修改windows下编译时setne指令错误
 */

#ifndef KY_ATOMIC
#define KY_ATOMIC

#include "ky_barrier.h"
#include "ky_type.h"

#include <typeinfo>

typedef enum
{
    Fence_Relaxed,         ///< 宽松的内存屏障
    Fence_Acquire,         ///< 对读取有保证的内存屏障,在之前不做任何重新排序
    Fence_Release,         ///< 对写入有保证的内存屏障,在之后不做任何重新排序
    Fence_AcquireRelease,  ///< 对写入读取有保证的内存屏障
} eMemoryFences;

template<int> struct atomic_has { enum{has = 0};};

template<> struct atomic_has<1> { enum{has = 1};};
template<> struct atomic_has<2> { enum{has = 1};};
template<> struct atomic_has<4> { enum{has = 1};};

//native support for 64-bit types
#if ((kyArchitecture & kyArchBit_64) == kyArchBit_64)
template<> struct atomic_has<8> { enum{has = 1};};
#else
#endif

template <typename T>
struct atomic_add{typedef T type; static const int size = 1;};
template <typename T>
struct atomic_add <T *> {typedef intptr type; static const int size = sizeof(T*);};

extern struct ky_atomic_arch
{
    void (*load)(void *volatile _v, void *nv, eMemoryFences ) ;
    void (*store)(void *volatile _v, void * nv, eMemoryFences) ;
    bool (*increase)(void *volatile _v) ;
    bool (*reduction)(void *volatile _v) ;
    void (*fetch_store)(void *volatile _v, void * fv, void * ev) ;
    void (*fetch_add)(void *volatile _v, void * fv, void * ev) ;
    bool (*compare_exchange)(void *volatile _v, void * ev, void * nv) ;
    void (*logic_and)(void *volatile _v, void *va) ;
    void (*logic_or)(void *volatile _v, void * va) ;
    void (*logic_xor)(void *volatile _v, void * va) ;
    void (*logic_add)(void *volatile _v, void * va) ;
    void (*logic_not)(void *volatile _v) ;
    void (*logic_inc)(void *volatile _v) ;
    void (*logic_dec)(void *volatile _v) ;

    void (*pause)(void) ;
    void (*memory_fence)(eMemoryFences) ;

}* const kyArchAtomics[9];


struct atomic_base
{
    template <typename T>
    static bool increase(T &_v)
    {
        return kyArchAtomics[sizeof(T)]->increase (&_v);
    }
    template <typename T>
    static bool reduction(T &_v)
    {
        return kyArchAtomics[sizeof(T)]->reduction (&_v);
    }

    template <typename T>
    static bool compare_exchange(T &_v, T ev, T nv)
    {
        return kyArchAtomics[sizeof(T)]->compare_exchange (&_v, &ev, &nv);
    }

    template <typename T>
    static bool compare_exchange(T &_v, T ev, T nv, T *cv)
    {
        return kyArchAtomics[sizeof(T)]->compare_exchange (&_v, &ev, &nv, cv);
    }

    template <typename T>
    static T fetch_store(T &_v, T nv)
    {
        T fv;
        kyArchAtomics[sizeof(T)]->fetch_store (&_v, &fv, &nv);

        return fv;
    }
    template <typename T>
    static T fetch_add(T &_v, T va)
    {
         T fv;
         kyArchAtomics[sizeof(T)]->fetch_add (&_v, &fv, &va);
        return fv;
    }

    template <typename T>
    static T logic_and(T &_v, T va)
    {
        kyArchAtomics[sizeof(T)]->logic_and (&_v,  &va);
        return _v;
    }
    template <typename T>
    static T logic_or(T &_v, T va)
    {
        kyArchAtomics[sizeof(T)]->logic_or (&_v,  &va);
        return _v;
    }
    template <typename T>
    static T logic_xor(T &_v, T va)
    {
        kyArchAtomics[sizeof(T)]->logic_xor (&_v,  &va);
        return _v;
    }
    template <typename T>
    static T logic_add(T &_v, T va)
    {
        kyArchAtomics[sizeof(T)]->logic_add (&_v,  &va);
        return _v;
    }

    template <typename T>
    static T logic_not(T &_v)
    {
        kyArchAtomics[sizeof(T)]->logic_not (&_v);
        return _v;
    }
    template <typename T>
    static T logic_inc(T &_v)
    {
        kyArchAtomics[sizeof(T)]->logic_inc (&_v);
        return _v;
    }
    template <typename T>
    static T logic_dec(T &_v)
    {
        kyArchAtomics[sizeof(T)]->logic_dec (&_v);
        return _v;
    }

    template <typename T>
    static T load(T _v, eMemoryFences mf= Fence_Relaxed)
    {
        T lv;
        kyArchAtomics[sizeof(T)]->load (&_v, &lv, mf);
        return lv;
    }

    template <typename T>
    static void store(T &_v, T nv, eMemoryFences mf= Fence_Relaxed)
    {
        kyArchAtomics[sizeof(T)]->store (&_v, &nv, mf);
    }

    static void pause(void) {
        return kyArchAtomics[0]->pause ();
    }

    static void memory_fence(eMemoryFences fence = Fence_Acquire){
        return kyArchAtomics[0]->memory_fence (fence);
    }
};

template <typename T>
class ky_atomic : public atomic_base
{
    kyCompilerAssert(is_int<T>::value);
    kyCompilerAssert(atomic_has<sizeof(T)>::has);

public:
    ky_atomic() {ope_value = T(0);}
    ky_atomic(T v) {atomic_base::store(ope_value, v, Fence_Release);}

    inline T operator = (T value)
    {
        atomic_base::store (ope_value, value, Fence_Release);
        return value;
    }

    inline T value()const {return atomic_base::load(ope_value, Fence_Acquire);}
    inline operator T () {return atomic_base::load(ope_value, Fence_Acquire);}
    inline operator const T ()const {return atomic_base::load(ope_value, Fence_Acquire);}

    inline ky_atomic<T> & operator = (const ky_atomic<T> &rhs)
    {
        atomic_base::store (ope_value, rhs.value (), Fence_Release);
        return *this;
    }

public:
    // 后置
    inline T operator++(int) {return atomic_base::fetch_add(ope_value, 1); }
    inline T operator--(int) {return atomic_base::fetch_add(ope_value, -1); }

    // 前置
    inline T operator++() {return atomic_base::logic_add(ope_value, (T)1);}
    inline T operator--() {return atomic_base::logic_add(ope_value, (T)-1);}

    inline T operator+=(T v) { return atomic_base::logic_add(ope_value, v); }
    inline T operator-=(T v) { return atomic_base::logic_add(ope_value, -v); }
    inline T operator&=(T v) { return atomic_base::logic_and(ope_value, v); }
    inline T operator|=(T v) { return atomic_base::logic_or(ope_value, v); }
    inline T operator^=(T v) { return atomic_base::logic_xor(ope_value, v); }

public:
    inline T load(eMemoryFences mo = Fence_Relaxed){return atomic_base::load(ope_value, mo);}
    inline void store(T nv, eMemoryFences mo = Fence_Relaxed){atomic_base::store(ope_value, nv, mo);}

    inline bool increase(){return atomic_base::increase(ope_value);}
    inline bool reduction(){return atomic_base::reduction(ope_value);}

    inline bool compare_exchange (T ev, T nv)
    {
        return atomic_base::compare_exchange(ope_value, ev, nv);
    }

    inline bool compare_exchange (T ev, T nv, T &cv)
    {
        return atomic_base::compare_exchange(ope_value, ev, nv, &cv);
    }

    inline T fetch_store(T nv)
    {
        return atomic_base::fetch_store(ope_value, nv);
    }

    inline T fetch_add(T va)
    {
        return atomic_base::fetch_add(ope_value, va);
    }

    inline T fetch_less(T va)
    {
        return atomic_base::fetch_add(ope_value, -va);
    }

    inline T logic_and(T a)
    {
        return atomic_base::logic_and(ope_value, a);
    }
    inline T logic_or( T a)
    {
        return atomic_base::logic_or(ope_value, a);
    }
    inline T logic_xor(T a)
    {
        return atomic_base::logic_xor(ope_value, a);
    }
    inline T logic_add(T a)
    {
        return atomic_base::logic_add(ope_value, a);
    }

    inline T logic_not()
    {
        return atomic_base::logic_not(ope_value);
    }
    inline T logic_inc()
    {
        return atomic_base::logic_inc(ope_value);
    }
    inline T logic_dec()
    {
        return atomic_base::logic_dec(ope_value);
    }
private:
    T ope_value;
};

template <typename T>
class ky_atomic <T*>: public atomic_base
{
    typedef T* PT;
    kyCompilerAssert(is_pointer<PT>::value);
    kyCompilerAssert(atomic_has<sizeof(PT)>::has);
public:
    ky_atomic() {ope_value = PT(0);}
    ky_atomic(PT v) {atomic_base::store(ope_value, v, Fence_Release);}

    inline PT operator = (PT value)
    {
        atomic_base::store (ope_value, value, Fence_Release);
        return value;
    }

    inline PT value()const {return atomic_base::load(ope_value, Fence_Acquire);}
    inline operator PT () {return atomic_base::load(ope_value, Fence_Acquire);}
    inline operator PT ()const {return atomic_base::load(ope_value, Fence_Acquire);}
    inline T *operator ->()  { return ope_value;}
    inline T *operator ->() const { return ope_value;}

    inline ky_atomic & operator = (const ky_atomic &rhs)
    {
        atomic_base::store (ope_value, rhs.value (), Fence_Release);
        return *this;
    }

public:
    // 后置
    inline T operator++(int) {return atomic_base::fetch_add(ope_value, 1);}
    inline T operator--(int) {return atomic_base::fetch_add(ope_value, -1);}

    // 前置
    inline T operator++() {return atomic_base::logic_add(ope_value, 1) ;}
    inline T operator--() {return atomic_base::logic_add(ope_value, -1) ;}

    inline PT operator+=(intptr v) { return atomic_base::logic_add(ope_value, v) ; }
    inline PT operator-=(intptr v) { return atomic_base::logic_add(ope_value, -v) ; }
    inline PT operator&=(intptr v) { return atomic_base::logic_and(ope_value, v) ; }
    inline PT operator|=(intptr v) { return atomic_base::logic_or(ope_value, v); }
    inline PT operator^=(intptr v) { return atomic_base::logic_xor(ope_value, v); }

public:
    inline PT load(eMemoryFences mo = Fence_Relaxed){return atomic_base::load(ope_value, mo);}
    inline void store(PT nv, eMemoryFences mo = Fence_Relaxed){atomic_base::store(ope_value, nv, mo);}

    inline bool increase(){return atomic_base::increase(ope_value);}
    inline bool reduction(){return atomic_base::reduction(ope_value);}

    inline bool compare_exchange (PT ev, PT nv)
    {
        return atomic_base::compare_exchange(ope_value, ev, nv);
    }
    inline bool compare_exchange (PT ev, PT nv, PT &cv)
    {
        return atomic_base::compare_exchange(ope_value, ev, nv, &cv);
    }

    inline PT fetch_store(PT nv)
    {
        return atomic_base::fetch_store(ope_value, nv);
    }

    inline PT fetch_add(typename atomic_add<PT>::type va)
    {
        return atomic_base::fetch_add(ope_value, va);
    }
    inline PT fetch_less(typename atomic_add<PT>::type va)
    {
        return atomic_base::fetch_add(ope_value, -va);
    }

    inline PT logic_and(typename enif_t <is_pointer<PT>::value, PT>::type ov)
    {
        return atomic_base::logic_and(ope_value, ov);
    }
    inline PT logic_or(typename enif_t <is_pointer<PT>::value, PT>::type ov)
    {
        return atomic_base::logic_or(ope_value, ov);
    }
    inline PT logic_xor(typename enif_t <is_pointer<PT>::value, PT>::type ov)
    {
        return atomic_base::logic_xor(ope_value, ov);
    }
    inline PT logic_add(typename enif_t <is_pointer<PT>::value, PT>::type ov)
    {
        return atomic_base::logic_add(ope_value, ov);
    }

    inline PT logic_not()
    {
        return atomic_base::logic_not(ope_value);
    }
    inline PT logic_inc()
    {
        return atomic_base::logic_inc(ope_value);
    }
    inline PT logic_dec()
    {
        return atomic_base::logic_dec(ope_value);
    }

private:
    PT ope_value;
};

#endif // KY_ATOMIC

