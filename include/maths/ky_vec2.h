
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
#ifndef ky_VEC2_H
#define ky_VEC2_H

#include "ky_maths.h"
#include "tools/ky_stream.h"
#include "tools/ky_variant.h"

template<typename T>
struct ky_vector2
{
    typedef T ElemType;

    enum
    {
        size = 2
    };

    union
    {
        struct
        {
            T x;
            T y;
        };
        struct {
            T s;
            T t;
        };
        struct {
            T r;
            T g;
        };
        struct {
            T width;
            T height;
        };
        T elem[size];
    };

    ky_vector2() {}
    explicit ky_vector2(T v) {
        for (size_t i = 0; i < size; ++i)
            elem[i] = v;
    }
    explicit ky_vector2(const T* v) {
        for (size_t i = 0; i < size; ++i)
            elem[i] = v[i];
    }
    ky_vector2(T t1, T t2) {
        elem[0] = t1;
        elem[1] = t2;
    }

    template<typename U>
    ky_vector2(const ky_vector2<U>& v) {
        for (size_t i = 0; i < v.size; ++i)
            elem[i] = T(v.elem[i]);
    }
    ky_vector2(const ky_vector2<T> & v){
        for (size_t i = 0; i < v.size; ++i)
            elem[i] = v.elem[i];
    }

    inline ky_vector2<T> &operator = (const ky_vector2<T> &v){
        for (size_t i = 0; i < v.size; ++i)
            elem[i] = v.elem[i];
        return *this;
    }

    inline const T& operator [] (size_t index) const {
        return elem[index];
    }

    inline T& operator [] (size_t index) {
        return elem[index];
    }

    static ky_vector2<T> zero;
};

template<typename T>
ky_vector2<T> ky_vector2<T>::zero = ky_vector2<T>(T(0), T(0));

typedef ky_vector2<float>   ky_vector2f;
typedef ky_vector2<real>  ky_vector2d;

/*
        types like in GLSL
    */
typedef ky_vector2<float>  vec2;
typedef ky_vector2<real>   dvec2; // currently not supported in GLSL but a reserved keyword
typedef ky_vector2<int>    ivec2;
typedef ky_vector2<size_t> svec2;
typedef ky_vector2<bool>   bvec2;

typedef ky_vector2<int>  isize;
typedef ky_vector2<float> fsize;
typedef ky_vector2<real> dsize;

typedef ky_vector2<int>  ipoint;
typedef ky_vector2<float> fpoint;
typedef ky_vector2<real> dpoint;

/*
        Prepare to implement 3 * 5 * 8 = 120 operators and dozens of functions
        the lazy way with evil voodoo macro magic
    */
template<typename T> inline ky_vector2<T> operator + (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] + v2.elem[i];
    return vRes;
}


template<typename T> inline ky_vector2<T> operator + (const ky_vector2<T>& v, T t)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] + t;
    return vRes;
}
template<typename T> inline ky_vector2<T> operator + (T t, const ky_vector2<T>& v)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t + v.elem[i];
    return vRes;
}
template<typename T> inline ky_vector2<T>& operator += (ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] += v2.elem[i];
    return v1;
}
template<typename T> inline ky_vector2<T>& operator += (ky_vector2<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] += t;
    return v;
}

template<typename T> inline ky_vector2<T> operator - (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] - v2.elem[i];
    return vRes;
}


template<typename T> inline ky_vector2<T> operator - (const ky_vector2<T>& v, T t)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] - t;
    return vRes;
}
template<typename T> inline ky_vector2<T> operator - (T t, const ky_vector2<T>& v)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t - v.elem[i];
    return vRes;
}
template<typename T> inline ky_vector2<T>& operator -= (ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] -= v2.elem[i];
    return v1;
}
template<typename T> inline ky_vector2<T>& operator -= (ky_vector2<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] -= t;
    return v;
}

template<typename T> inline ky_vector2<T> operator * (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] * v2.elem[i];
    return vRes;
}


template<typename T> inline ky_vector2<T> operator * (const ky_vector2<T>& v, T t)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] * t;
    return vRes;
}
template<typename T> inline ky_vector2<T> operator * (T t, const ky_vector2<T>& v)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t * v.elem[i];
    return vRes;
}
template<typename T> inline ky_vector2<T>& operator *= (ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] *= v2.elem[i];
    return v1;
}
template<typename T> inline ky_vector2<T>& operator *= (ky_vector2<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] *= t;
    return v;
}

template<typename T> inline ky_vector2<T> operator / (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] / v2.elem[i];
    return vRes;
}


template<typename T> inline ky_vector2<T> operator / (const ky_vector2<T>& v, T t)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] / t;
    return vRes;
}
template<typename T> inline ky_vector2<T> operator / (T t, const ky_vector2<T>& v)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t / v.elem[i];
    return vRes;
}
template<typename T> inline ky_vector2<T>& operator /= (ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] /= v2.elem[i];
    return v1;
}
template<typename T> inline ky_vector2<T>& operator /= (ky_vector2<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] /= t;
    return v;
}

template<typename T> inline ky_vector2<T> operator % (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] % v2.elem[i];
    return vRes;
}


template<typename T> inline ky_vector2<T> operator % (const ky_vector2<T>& v, T t)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] % t;
    return vRes;
}
template<typename T> inline ky_vector2<T> operator % (T t, const ky_vector2<T>& v)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t % v.elem[i];
    return vRes;
}
template<typename T> inline ky_vector2<T>& operator %= (ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] %= v2.elem[i];
    return v1;
}
template<typename T> inline ky_vector2<T>& operator %= (ky_vector2<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] %= t;
    return v;
}

template<typename T> inline ky_vector2<T> operator & (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] & v2.elem[i];
    return vRes;
}


template<typename T> inline ky_vector2<T> operator & (const ky_vector2<T>& v, T t)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] & t;
    return vRes;
}
template<typename T> inline ky_vector2<T> operator & (T t, const ky_vector2<T>& v)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t & v.elem[i];
    return vRes;
}
template<typename T> inline ky_vector2<T>& operator &= (ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] &= v2.elem[i];
    return v1;
}
template<typename T> inline ky_vector2<T>& operator &= (ky_vector2<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] &= t;
    return v;
}

template<typename T> inline ky_vector2<T> operator | (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] | v2.elem[i];
    return vRes;
}

template<typename T> inline ky_vector2<T> operator | (const ky_vector2<T>& v, T t)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] | t;
    return vRes;
}
template<typename T> inline ky_vector2<T> operator | (T t, const ky_vector2<T>& v)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t | v.elem[i];
    return vRes;
}
template<typename T> inline ky_vector2<T>& operator |= (ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] |= v2.elem[i];
    return v1;
}
template<typename T> inline ky_vector2<T>& operator |= (ky_vector2<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] |= t;
    return v;
}

template<typename T> inline ky_vector2<T> operator ^ (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] ^ v2.elem[i];
    return vRes;
}


template<typename T> inline ky_vector2<T> operator ^ (const ky_vector2<T>& v, T t)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] ^ t;
    return vRes;
}
template<typename T> inline ky_vector2<T> operator ^ (T t, const ky_vector2<T>& v)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t ^ v.elem[i];
    return vRes;
}
template<typename T> inline ky_vector2<T>& operator ^= (ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] ^= v2.elem[i];
    return v1;
}
template<typename T> inline ky_vector2<T>& operator ^= (ky_vector2<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] ^= t;
    return v;
}

template<typename T> inline bool operator > (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    bool result = true;
    for (size_t i = 0; i < v1.size; ++i)
        result &= (v1.elem[i] > v2.elem[i]);
    return result;
}
template<typename T> inline bool operator < (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    bool result = true;
    for (size_t i = 0; i < v1.size; ++i)
        result &= (v1.elem[i] < v2.elem[i]);
    return result;
}

/*
        relational operators
    */
template<typename T>
inline ky_vector2<bool> lessThan (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    ky_vector2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] < v2.elem[i]);
    return vRes;
}

template<typename T>
inline ky_vector2<bool> lessThanEqual (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    ky_vector2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] <= v2.elem[i]);
    return vRes;
}

template<typename T>
inline ky_vector2<bool> greaterThan (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    ky_vector2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] > v2.elem[i]);
    return vRes;
}

template<typename T>
inline ky_vector2<bool> greaterThanEqual (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    ky_vector2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] >= v2.elem[i]);
    return vRes;
}
template<typename T>
inline ky_vector2<bool> equal (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    ky_vector2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] == v2.elem[i]);
    return vRes;
}

template<typename T>
inline ky_vector2<bool> notEqual (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    ky_vector2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] != v2.elem[i]);
    return vRes;
}

template<typename T>
inline bool operator == (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    bool result = true;
    for (size_t i = 0; i < v1.size; ++i)
        result &= (v1.elem[i] == v2.elem[i]);
    return result;
}

template<typename T>
inline bool operator != (const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    return !(v1 == v2);
}


/*
        further vector functions
    */
template<typename T> inline T dot(const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    T result = 0;
    for (size_t i = 0; i < v1.size; ++i)
        result += v1.elem[i] * v2.elem[i];
    return result;
}

template<typename T> inline T lengthSq(const ky_vector2<T>& v)
{
    T result = 0;
    for (size_t i = 0; i < v.size; ++i)
        result += (v.elem[i] * v.elem[i]);
    return result;
}

template<typename T> inline T length(const ky_vector2<T>& v)
{
    return std::sqrt(lengthSq(v));
}

template<typename T> inline ky_vector2<T> normalize(const ky_vector2<T>& v)
{
    ky_vector2<T> vRes(v);
    T recipLength = 1 / length(v);
    vRes *= recipLength;
    return vRes;
}

template<typename T> inline T distance(const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    return length(v2 - v1);
}

template<typename T> inline T distanceSq(const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    return lengthSq(v2 - v1);
}

template<typename T> inline ky_vector2<T> operator - (const ky_vector2<T>& v)
{
    ky_vector2<T> vRes(v);
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = -v.elem[i];
    return vRes;
}

template<typename T> inline ky_vector2<T> min(const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = ky_min(v1.elem[i], v2.elem[i]);
    return vRes;
}

template<typename T> inline ky_vector2<T> max(const ky_vector2<T>& v1, const ky_vector2<T>& v2)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = ky_max(v1.elem[i], v2.elem[i]);
    return vRes;
}

template<typename T> inline ky_vector2<T> min(const ky_vector2<T>& v, T t)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = ky_min(v.elem[i], t);
    return vRes;
}

template<typename T> inline ky_vector2<T> max(const ky_vector2<T>& v, T t)
{
    ky_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = ky_max(v.elem[i], t);
    return vRes;
}

template<typename T> inline T min(const ky_vector2<T>& v)
{
    T res = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        res = ky_min(v.elem[i], res);
    return res;
}

template<typename T> inline T max(const ky_vector2<T>& v)
{
    T res = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        res = ky_max(v.elem[i], res);
    return res;
}

template<typename T> inline ky_vector2<T> clamp(const ky_vector2<T>& v, const ky_vector2<T>& minVal, const ky_vector2<T>& maxVal)
{
    return min( max(v, minVal), maxVal );
}

template<typename T> inline ky_vector2<T> clamp(const ky_vector2<T>& v, T minVal, T maxVal)
{
    return min( max(v, minVal), maxVal );
}

template<typename T> inline ky_vector2<T> ceil(const ky_vector2<T>& v)
{
    using std::ceil;
    ky_vector2<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ceil(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector2<T> floor(const ky_vector2<T>& v)
{
    using std::floor;
    ky_vector2<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = floor(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector2<T> fix(const ky_vector2<T>& v)
{
    ky_vector2<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = fix(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector2<T> round(const ky_vector2<T>& v)
{
    ky_vector2<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = round(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector2<T> sign(const ky_vector2<T>& v) {
    ky_vector2<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = sign(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector2<int> iceil(const ky_vector2<T>& v)
{
    ky_vector2<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = iceil(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector2<int> ifloor(const ky_vector2<T>& v)
{
    ky_vector2<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ifloor(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector2<int> iround(const ky_vector2<T>& v)
{
    ky_vector2<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = iround(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector2<int> ifix(const ky_vector2<T>& v)
{
    ky_vector2<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ifix(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector2<int> isign(const ky_vector2<T>& v)
{
    ky_vector2<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = isign(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector2<T> mix(const ky_vector2<T>& v, const ky_vector2<T>& v2, T t)
{
    ky_vector2<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result.elem[i] = (v.elem[i]*(1-t)) + (v2.elem[i]*t);
    return result;
}

/*
        horizontally working functions
    */
template<typename T> inline T hadd(const ky_vector2<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result += v.elem[i];
    return result;
}

template<typename T> inline T hsub(const ky_vector2<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result -= v.elem[i];
    return result;
}

template<typename T> inline T hmul(const ky_vector2<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result *= v.elem[i];
    return result;
}

template<typename T> inline T hdiv(const ky_vector2<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result /= v.elem[i];
    return result;
}

template<typename T> inline T hmod(const ky_vector2<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result %= v.elem[i];
    return result;
}

template<typename T> inline bool hand(const ky_vector2<T>& v)
{
    bool result = v.elem[0] && v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result &= v.elem[i];
    return result;
}

template<typename T> inline bool hor(const ky_vector2<T>& v)
{
    T result = v.elem[0] || v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result |= v.elem[i];
    return result;
}

template<typename T> inline bool hxor(const ky_vector2<T>& v)
{
    T result = v.elem[0] ^ v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result ^= v.elem[i];
    return result;
}


/**
        cross product
    */
/*
        Component-wise absolute value
    */
template<typename T>
inline ky_vector2<T> abs(const ky_vector2<T>& v) {
    using std::abs;
    return ky_vector2<T>(fabs(v.x), fabs(v.y));
}

/*
        specialization for length(VectorX<int>)
    */

inline float length(const ky_vector2<int>& v) {
    return std::sqrt( float(lengthSq(v)) );
}
template<typename T>
inline void ky_swap (ky_vector2<T> &v1, ky_vector2<T> &v2)
{
    ky_vector2<T> tmp = v1;
    v1 = v2;
    v2 = tmp;
}
/*
         stream operators
    */

/// output ky_vector2d to stream
template<typename T>
std::ostream& operator << (std::ostream& s, const ky_vector2<T>& v) {
    return (s << "[" << v.elem[0] << " " << v.elem[1] << "]");
}

// specialication for (unsigned) int, to make it output numbers and not characters

template<>
inline std::ostream& operator << <uchar>(std::ostream& s, const ky_vector2<uchar>& v) {
    return (s << ky_vector2<int>(v));
}

template<>
inline std::ostream& operator << <char>(std::ostream& s, const ky_vector2<char>& v) {
    return (s << ky_vector2<int>(v));
}

/// input ky_vector2d from stream
template<typename T>
std::istream& operator >> (std::istream& s, ky_vector2<T>& v) {
    char ch;

    s >> ch;
    if (ch == '[') {  // format "[ x y ]"
        s >> v[0] >> v[1] >> ch;
        if (ch != ']') {
            s.clear(std::ios::badbit);
        }
    } else { // format "x y"
        s.putback(ch);
        s >> v[0] >> v[1];
    }

    return s;
}


template<typename T>
ky_streamb &operator << (ky_streamb &in, const ky_vector2<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        in << v.elem[i];
    return in;
}

template<typename T>
ky_streamb &operator >> (ky_streamb &out, ky_vector2<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
template<typename T>
ky_streamt &operator << (ky_streamt &in, const ky_vector2<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        in << v.elem[i];
    return in;
}

template<typename T>
ky_streamt &operator >> (ky_streamt &out, ky_vector2<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
template<typename T>
ky_variant &operator << (ky_variant &va, const ky_vector2<T> &col)
{
    va.data.append((uint8*)&col, sizeof(ky_vector2<T>));
    va.set("vec2");
    return va;
}
template<typename T>
ky_variant &operator >> (ky_variant &va, ky_vector2<T> &col)
{
    if (va.is_set ("vec2") && va.data.size () == sizeof(ky_vector2<T>))
        col = *((ky_vector2<T>*)va.data.data());
    return va;
}
#endif // VEC2_H
