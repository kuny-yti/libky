
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_algorlthm.h
 * @brief    基本算法定义和实现
 *       1.数据类型的大小判断
 *       2.常用排序算法
 *       3.基本数据类型的交换
 *       3.一些哈希算法实现(散列)
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.2.1.1
 * @date     2015/04/15
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2015/04/15 | 1.0.0.1   | kunyang  | 创建文件加入数据类型的交换
 * 2015/08/22 | 1.0.1.0   | kunyang  | 修改根据编译器来交换64位整形类型
 * 2016/02/11 | 1.1.0.1   | kunyang  | 加入散列计算算法
 * 2016/06/04 | 1.2.0.1   | kunyang  | 加入排序算法
 * 2016/11/22 | 1.2.1.1   | kunyang  | 修改排序算法并加入归并排序算法
 */
#ifndef ky_ALGORLTHM_H
#define ky_ALGORLTHM_H

#include "ky_define.h"

template <typename T>
inline const T _ref ky_min(_in T _ref a, _in T _ref b) { return (a < b) ? a : b; }
template <typename T>
inline const T _ref ky_max(_in T _ref a, _in T _ref b) { return (a < b) ? b : a; }
template <typename T>
inline const T _ref ky_bound(_in T _ref min, _in T _ref val, _in T _ref max)
{return ky_max(min, ky_min(max, val));}

#define _SWAP_(T) inline void ky_swap(T _ref a, T _ref b){T tmp = a; a = b; b = tmp;}
_SWAP_(bool)
_SWAP_(bool_t)
_SWAP_(char)
_SWAP_(short)
_SWAP_(int)
_SWAP_(long)
_SWAP_(float)
_SWAP_(uchar)
_SWAP_(ushort)
_SWAP_(uint)
_SWAP_(ulong)
_SWAP_(real)
_SWAP_(half_float)
_SWAP_(wchar_t)
#if kyCompiler != kyCompiler_GNUC && kyCompiler != kyCompiler_CLANG
_SWAP_(int64)
#endif
#if kyCompiler != kyCompiler_GNUC && kyCompiler != kyCompiler_CLANG
_SWAP_(uint64)
#endif
#undef _SWAP_

namespace __sort__
{
    //!
    //! @brief insert 数组型插入排序
    //!
    //! @param a 是数组的开始地址
    //! @param length 是数组的长度
    //!
    template<typename T>
    void insert( T *a, size_t length);

    //!
    //! \brief insert 容器型插入排序
    //! \param begin
    //! \param end
    //!
    template<typename container_iterator>
    void insert (container_iterator begin, container_iterator end);


    //!
    //! @brief insert2 数组型2路插入排序
    //!
    //! @param a 是数组的开始地址
    //! @param length 是数组的长度
    //!
    template<typename T>
    void insert2(T* a, size_t alength);

    //!
    //! @brief quick 数组型快速排序
    //!
    //! @param begin 是数组的开始地址
    //! @param end 是数组的结束地址
    //!
    template<typename T>
    void quick(T *begin, T *end);

    //!
    //! @brief quick 容器型快速排序
    //!
    //! @param begin 是容器开始迭代
    //! @param end 是容器结束迭代
    //!
    template<typename container_iterator>
    void quick(container_iterator begin, container_iterator end);

    //!
    //! @brief heap 堆排序算法
    //!
    //! @param H是待调整的堆数组
    //! @param length是数组的长度
    //!
    template <typename T>
    void heap(T *H, size_t length);

    //!
    //! @brief bubble 冒泡排序算法
    //!
    //! @param H是待调整的堆数组
    //! @param length是数组的长度
    //!
    template <typename T>
    void bubble (T *r, size_t length);
    //!
    //! @brief merge 归并排序算法
    //!
    //! @param arr 是待调整的堆数组
    //! @param length是数组的长度
    //!
    template <typename T>
    void merge(T *arr, size_t length);
}
template<typename T>
inline void ky_isort( T *a, size_t length){__sort__::insert(a, length);}
template<typename container_iterator>
inline void ky_isort(container_iterator begin, container_iterator end){__sort__::insert(begin, end);}
template<typename T>
inline void ky_isort2(T* a, size_t alength){__sort__::insert2(a, alength);}
template<typename T>
inline void ky_qsort(T *begin, T *end){__sort__::quick(begin, end);}
template<typename container_iterator>
inline void ky_qsort(container_iterator begin, container_iterator end){__sort__::quick(begin, end);}
template<typename T>
inline void ky_hsort(T *H, size_t length){__sort__::heap(H, length);}
template<typename T>
inline void ky_bsort(T *H, size_t length){__sort__::bubble(H, length);}
template<typename T>
inline void ky_msort(T *H, size_t length){__sort__::merge(H, length);}

namespace __hash_
{
    // RS Hash Function
    uint32 RS (char *str, size_t len);
    // JS Hash Function
    uint32 JS(char *str, size_t len);
    // P. J. Weinberger Hash Function
    uint32 PJW(char *str, size_t len);
    // ELF Hash Function
    uint32 ELF(char *str, size_t len);
    // BKDR Hash Function
    uint32 BKDR(char *str, size_t len, uint seed = 131);
    // SDBM Hash Function
    uint32 SDBM(char *str, size_t len);
    // DJB Hash Function
    uint32 DJB(char *str, size_t len);
    // AP Hash Function
    uint32 AP(char *str, size_t len);
    /*DEKHash*/
    uint32 DEK(char *str, size_t len);
    /*BPHash*/
    uint32 BP(char *str, size_t len);
    /*FNVHash*/
    uint32 FNV(char *str, size_t len);
    uint32 H64(uint64 v);
}

inline uint32 ky_hash(_in char _ref v){return (uint32)v;}
inline uint32 ky_hash(_in uchar _ref v){return (uint32)v;}
inline uint32 ky_hash(_in short _ref v){return (uint32)v;}
inline uint32 ky_hash(_in ushort _ref v){return (uint32)v;}
inline uint32 ky_hash(_in int _ref v){return (uint32)v;}
inline uint32 ky_hash(_in uint _ref v){return (uint32)v;}
inline uint32 ky_hash(_in wchar_t _ref v){return (uint32)v;}
inline uint32 ky_hash(_in long _ref v){return __hash_::H64 (v);}
inline uint32 ky_hash(_in ulong _ref v){return __hash_::H64 (v);}
inline uint32 ky_hash(_in longlong _ref v){return __hash_::H64 (v);}
inline uint32 ky_hash(_in ulonglong _ref v){return __hash_::H64 (v);}
inline uint32 ky_hash(float v){union{float tv;uint64 v64;};tv = v; return __hash_::H64(v64);}
inline uint32 ky_hash(real v){union{real tv;uint64 v64;};tv = v; return __hash_::H64(v64);}
inline uint32 ky_hash(char *v, int len = -1){if (len < 0) len = strlen (v);return __hash_::DJB(v, len);}

#include "ky_algorlthm.inl"
#endif //ky_ALGORLTHM_H
