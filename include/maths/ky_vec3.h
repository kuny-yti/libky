
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
#ifndef ky_VEC3_H
#define ky_VEC3_H

#include "ky_vec2.h"


/**
        This is a 3-dimensional Vector typename similar to vec3 of GLSL.
        All standard-operators are overloaded and go componentwisely
        similar to GLSL. For dot- and crossproduct use the functions dot and cross.
    */
template<typename T>
struct ky_vector3 {
    typedef T ElemType;

    enum {
        size = 3
    };

    union {
        struct {
            T x;
            T y;
            T z;
        };
        struct {
            T s;
            T t;
            T p;
        };
        struct {
            T r;
            T g;
            T b;
        };
        struct {
          T width;
          T height;
          T depth;
        };
        // workaround to prevent "dereferencing type-punned pointer will break strict-aliasing
        // rules" warning with gcc and -fstrict-aliasing.
        struct {
            T _xy[size-1];
            T _z;
        };
        T elem[size];
    };

    /// default constructor
    ky_vector3() {}
    /// Init all elements with the same value
    explicit ky_vector3(T v) {
        for (size_t i = 0; i < size; ++i)
            elem[i] = v;
    }
    /// Init from array with equal size
    explicit ky_vector3(const T* v) {
        for (size_t i = 0; i < size; ++i)
            elem[i] = v[i];
    }
    /// Init componentwisely
    ky_vector3(T t1, T t2, T t3) {
        elem[0] = t1;
        elem[1] = t2;
        elem[2] = t3;
    }
    /// Init x and y with v, z with _z
    ky_vector3(const ky_vector3<T>& vec, T z_) {
        elem[0] = vec.elem[0];
        elem[1] = vec.elem[1];
        elem[2] = z_;
    }
    /// Init y and z with vec2, x with _x
    ky_vector3(T _x, const ky_vector3<T>& v) {
        elem[0] = _x;
        elem[1] = v.elem[1];
        elem[2] = v.elem[2];
    }
    ky_vector3(const ky_vector2<T>& v) {
        elem[0] = v.elem[0];
        elem[1] = v.elem[1];
        elem[2] = (T)0;
    }
    ky_vector3(const ky_vector2<T>& v, T z) {
        elem[0] = v.elem[0];
        elem[1] = v.elem[1];
        elem[2] = z;
    }

    /// Init with another Vector of another type
    template<typename U>
    ky_vector3(const ky_vector3<U>& v) {
        for (size_t i = 0; i < v.size; ++i)
            elem[i] = T(v.elem[i]);
    }

    ky_vector3(const ky_vector3<T> & v){
        for (size_t i = 0; i < v.size; ++i)
            elem[i] = v.elem[i];
    }

    inline ky_vector3<T> &operator = (const ky_vector3<T> &v){
        for (size_t i = 0; i < v.size; ++i)
            elem[i] = v.elem[i];
        return *this;
    }

    /// Index operator
    inline const T& operator [] (size_t index) const {
        return elem[index];
    }
    /// Index operator
    inline T& operator [] (size_t index) {
        return elem[index];
    }

    /*
        sub-vector getters
    */
    inline const ky_vector2<T>& xy() const {
        return *((ky_vector2<T>*) _xy);
    }
    inline ky_vector2<T>& xy() {
        return *((ky_vector2<T>*) _xy);
    }
    inline const ky_vector2<T>& yz() const {
        return *((ky_vector2<T>*) (elem + 1));
    }
    inline ky_vector2<T>& yz() {
        return *((ky_vector2<T>*) (elem + 1));
    }

    /*
        statics
    */
    static ky_vector3<T> zero;
};

/// init statics
template<typename T>
ky_vector3<T> ky_vector3<T>::zero = ky_vector3<T>(T(0), T(0), T(0));

typedef ky_vector3<float>   ky_vector3f;
typedef ky_vector3<real>  ky_vector3d;

typedef ky_vector3f        vector;

/*
        types like in GLSL
    */

typedef ky_vector3<float>   vec3;
typedef ky_vector3<real>    dvec3; // currently not supported in GLSL but a reserved keyword
typedef ky_vector3<int>     ivec3;
typedef ky_vector3<size_t>  svec3;
typedef ky_vector3<bool>    bvec3;

typedef ky_vector3<int>  isize3;
typedef ky_vector3<float> fsize3;
typedef ky_vector3<real> dsize3;

typedef ky_vector3<int>  ipoint3;
typedef ky_vector3<float> fpoint3;
typedef ky_vector3<real> dpoint3;

template<typename T>
inline ky_vector3<T> perpendicular( const ky_vector3<T>& src, const ky_vector3<T>& vec )  {
    T crossX = src.y*vec.z - src.z*vec.y;
    T crossY = src.z*vec.x - src.x*vec.z;
    T crossZ = src.x*vec.y - src.y*vec.x;

    T length = (float)sqrt(crossX*crossX +
                           crossY*crossY +
                           crossZ*crossZ);

    if( length > 0 )
        return ky_vector3<T>( crossX/length, crossY/length, crossZ/length );
    else
        return ky_vector3<T>();
}

template<typename T>
inline ky_vector3<T> crossed( const ky_vector3<T>& src ,const ky_vector3<T>& vec )  {
    return ky_vector3<T>( src.y*vec.z - src.z*vec.y,
                          src.z*vec.x - src.x*vec.z,
                          src.x*vec.y - src.y*vec.x );
}
template<typename T>
inline ky_vector3<T> rotatedRad(const ky_vector3<T> &src, T angle, const ky_vector3<T>& axis )
{
    ky_vector3<T> ax = normalize(axis);
    T a = angle;
    T sina = sin( a );
    T cosa = cos( a );
    T cosb = (T)1 - cosa;

    return ky_vector3<T>( src.x*(ax.x*ax.x*cosb + cosa)
                          + src.y*(ax.x*ax.y*cosb - ax.z*sina)
                          + src.z*(ax.x*ax.z*cosb + ax.y*sina),
                          src.x*(ax.y*ax.x*cosb + ax.z*sina)
                          + src.y*(ax.y*ax.y*cosb + cosa)
                          + src.z*(ax.y*ax.z*cosb - ax.x*sina),
                          src.x*(ax.z*ax.x*cosb - ax.y*sina)
                          + src.y*(ax.z*ax.y*cosb + ax.x*sina)
                          + src.z*(ax.z*ax.z*cosb + cosa) );
}

/*
        Prepare to implement 3 * 5 * 8 = 120 operators and dozens of functions
        the lazy way with evil voodoo macro magic
    */
template<typename T> inline ky_vector3<T> operator + (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] + v2.elem[i];
    return vRes;
}


template<typename T> inline ky_vector3<T> operator + (const ky_vector3<T>& v, T t)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] + t;
    return vRes;
}
template<typename T> inline ky_vector3<T> operator + (T t, const ky_vector3<T>& v)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t + v.elem[i];
    return vRes;
}
template<typename T> inline ky_vector3<T>& operator += (ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] += v2.elem[i];
    return v1;
}
template<typename T> inline ky_vector3<T>& operator += (ky_vector3<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] += t;
    return v;
}

template<typename T> inline ky_vector3<T> operator - (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] - v2.elem[i];
    return vRes;
}


template<typename T> inline ky_vector3<T> operator - (const ky_vector3<T>& v, T t)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] - t;
    return vRes;
}
template<typename T> inline ky_vector3<T> operator - (T t, const ky_vector3<T>& v)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t - v.elem[i];
    return vRes;
}
template<typename T> inline ky_vector3<T>& operator -= (ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] -= v2.elem[i];
    return v1;
}
template<typename T> inline ky_vector3<T>& operator -= (ky_vector3<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] -= t;
    return v;
}

template<typename T> inline ky_vector3<T> operator * (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] * v2.elem[i];
    return vRes;
}


template<typename T> inline ky_vector3<T> operator * (const ky_vector3<T>& v, T t)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] * t;
    return vRes;
}
template<typename T> inline ky_vector3<T> operator * (T t, const ky_vector3<T>& v)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t * v.elem[i];
    return vRes;
}
template<typename T> inline ky_vector3<T>& operator *= (ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] *= v2.elem[i];
    return v1;
}
template<typename T> inline ky_vector3<T>& operator *= (ky_vector3<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] *= t;
    return v;
}

template<typename T> inline ky_vector3<T> operator / (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] / v2.elem[i];
    return vRes;
}


template<typename T> inline ky_vector3<T> operator / (const ky_vector3<T>& v, T t)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] / t;
    return vRes;
}
template<typename T> inline ky_vector3<T> operator / (T t, const ky_vector3<T>& v)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t / v.elem[i];
    return vRes;
}
template<typename T> inline ky_vector3<T>& operator /= (ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] /= v2.elem[i];
    return v1;
}
template<typename T> inline ky_vector3<T>& operator /= (ky_vector3<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] /= t;
    return v;
}

template<typename T> inline ky_vector3<T> operator % (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] % v2.elem[i];
    return vRes;
}


template<typename T> inline ky_vector3<T> operator % (const ky_vector3<T>& v, T t)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] % t;
    return vRes;
}
template<typename T> inline ky_vector3<T> operator % (T t, const ky_vector3<T>& v)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t % v.elem[i];
    return vRes;
}
template<typename T> inline ky_vector3<T>& operator %= (ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] %= v2.elem[i];
    return v1;
}
template<typename T> inline ky_vector3<T>& operator %= (ky_vector3<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] %= t;
    return v;
}

template<typename T> inline ky_vector3<T> operator & (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] & v2.elem[i];
    return vRes;
}


template<typename T> inline ky_vector3<T> operator & (const ky_vector3<T>& v, T t)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] & t;
    return vRes;
}
template<typename T> inline ky_vector3<T> operator & (T t, const ky_vector3<T>& v)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t & v.elem[i];
    return vRes;
}
template<typename T> inline ky_vector3<T>& operator &= (ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] &= v2.elem[i];
    return v1;
}
template<typename T> inline ky_vector3<T>& operator &= (ky_vector3<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] &= t;
    return v;
}

template<typename T> inline ky_vector3<T> operator | (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] | v2.elem[i];
    return vRes;
}

template<typename T> inline ky_vector3<T> operator | (const ky_vector3<T>& v, T t)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] | t;
    return vRes;
}
template<typename T> inline ky_vector3<T> operator | (T t, const ky_vector3<T>& v)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t | v.elem[i];
    return vRes;
}
template<typename T> inline ky_vector3<T>& operator |= (ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] |= v2.elem[i];
    return v1;
}
template<typename T> inline ky_vector3<T>& operator |= (ky_vector3<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] |= t;
    return v;
}

template<typename T> inline ky_vector3<T> operator ^ (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] ^ v2.elem[i];
    return vRes;
}


template<typename T> inline ky_vector3<T> operator ^ (const ky_vector3<T>& v, T t)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] ^ t;
    return vRes;
}
template<typename T> inline ky_vector3<T> operator ^ (T t, const ky_vector3<T>& v)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t ^ v.elem[i];
    return vRes;
}
template<typename T> inline ky_vector3<T>& operator ^= (ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] ^= v2.elem[i];
    return v1;
}
template<typename T> inline ky_vector3<T>& operator ^= (ky_vector3<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] ^= t;
    return v;
}

/*
        relational operators
    */
template<typename T>
inline ky_vector3<bool> lessThan (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] < v2.elem[i]);
    return vRes;
}

template<typename T>
inline ky_vector3<bool> lessThanEqual (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] <= v2.elem[i]);
    return vRes;
}

template<typename T>
inline ky_vector3<bool> greaterThan (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] > v2.elem[i]);
    return vRes;
}

template<typename T>
inline ky_vector3<bool> greaterThanEqual (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] >= v2.elem[i]);
    return vRes;
}
template<typename T>
inline ky_vector3<bool> equal (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] == v2.elem[i]);
    return vRes;
}

template<typename T>
inline ky_vector3<bool> notEqual (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] != v2.elem[i]);
    return vRes;
}

template<typename T>
inline bool operator == (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    bool result = true;
    for (size_t i = 0; i < v1.size; ++i)
        result &= (v1.elem[i] == v2.elem[i]);
    return result;
}

template<typename T>
inline bool operator != (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    return !(v1 == v2);
}
template<typename T> inline bool operator > (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    bool result = true;
    for (size_t i = 0; i < v1.size; ++i)
        result &= (v1.elem[i] > v2.elem[i]);
    return result;
}
template<typename T> inline bool operator < (const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    bool result = true;
    for (size_t i = 0; i < v1.size; ++i)
        result &= (v1.elem[i] < v2.elem[i]);
    return result;
}


/*
        further vector functions
    */
template<typename T> inline T dot(const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    T result = 0;
    for (size_t i = 0; i < v1.size; ++i)
        result += v1.elem[i] * v2.elem[i];
    return result;
}

template<typename T> inline T lengthSq(const ky_vector3<T>& v)
{
    T result = 0;
    for (size_t i = 0; i < v.size; ++i)
        result += (v.elem[i] * v.elem[i]);
    return result;
}

template<typename T> inline T length(const ky_vector3<T>& v)
{
    return std::sqrt(lengthSq(v));
}

template<typename T> inline ky_vector3<T> normalize(const ky_vector3<T>& v)
{
    ky_vector3<T> vRes(v);
    T recipLength = 1 / length(v);
    vRes *= recipLength;
    return vRes;
}

template<typename T> inline T distance(const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    return length(v2 - v1);
}

template<typename T> inline T distanceSq(const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    return lengthSq(v2 - v1);
}

template<typename T> inline ky_vector3<T> operator - (const ky_vector3<T>& v)
{
    ky_vector3<T> vRes(v);
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = -v.elem[i];
    return vRes;
}

template<typename T> inline ky_vector3<T> min(const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = ky_min(v1.elem[i], v2.elem[i]);
    return vRes;
}

template<typename T> inline ky_vector3<T> max(const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = ky_max(v1.elem[i], v2.elem[i]);
    return vRes;
}

template<typename T> inline ky_vector3<T> min(const ky_vector3<T>& v, T t)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = ky_min(v.elem[i], t);
    return vRes;
}

template<typename T> inline ky_vector3<T> max(const ky_vector3<T>& v, T t)
{
    ky_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = ky_max(v.elem[i], t);
    return vRes;
}

template<typename T> inline T min(const ky_vector3<T>& v)
{
    T res = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        res = ky_min(v.elem[i], res);
    return res;
}

template<typename T> inline T max(const ky_vector3<T>& v)
{
    T res = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        res = ky_max(v.elem[i], res);
    return res;
}

template<typename T> inline ky_vector3<T> clamp(const ky_vector3<T>& v, const ky_vector3<T>& minVal, const ky_vector3<T>& maxVal)
{
    return min( max(v, minVal), maxVal );
}

template<typename T> inline ky_vector3<T> clamp(const ky_vector3<T>& v, T minVal, T maxVal)
{
    return min( max(v, minVal), maxVal );
}

template<typename T> inline ky_vector3<T> ceil(const ky_vector3<T>& v)
{
    using std::ceil;
    ky_vector3<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ceil(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector3<T> floor(const ky_vector3<T>& v)
{
    using std::floor;
    ky_vector3<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = floor(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector3<T> fix(const ky_vector3<T>& v)
{
    ky_vector3<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = fix(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector3<T> round(const ky_vector3<T>& v)
{
    ky_vector3<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = round(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector3<T> sign(const ky_vector3<T>& v) {
    ky_vector3<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = sign(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector3<int> iceil(const ky_vector3<T>& v)
{
    ky_vector3<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = iceil(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector3<int> ifloor(const ky_vector3<T>& v)
{
    ky_vector3<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ifloor(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector3<int> iround(const ky_vector3<T>& v)
{
    ky_vector3<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = iround(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector3<int> ifix(const ky_vector3<T>& v)
{
    ky_vector3<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ifix(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector3<int> isign(const ky_vector3<T>& v)
{
    ky_vector3<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = isign(v.elem[i]);
    return result;
}

template<typename T> inline ky_vector3<T> mix(const ky_vector3<T>& v, const ky_vector3<T>& v2, T t)
{
    ky_vector3<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result.elem[i] = (v.elem[i]*(1-t)) + (v2.elem[i]*t);
    return result;
}

/*
        horizontally working functions
    */
template<typename T> inline T hadd(const ky_vector3<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result += v.elem[i];
    return result;
}

template<typename T> inline T hsub(const ky_vector3<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result -= v.elem[i];
    return result;
}

template<typename T> inline T hmul(const ky_vector3<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result *= v.elem[i];
    return result;
}

template<typename T> inline T hdiv(const ky_vector3<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result /= v.elem[i];
    return result;
}

template<typename T> inline T hmod(const ky_vector3<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result %= v.elem[i];
    return result;
}

template<typename T> inline bool hand(const ky_vector3<T>& v)
{
    bool result = v.elem[0] && v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result &= v.elem[i];
    return result;
}

template<typename T> inline bool hor(const ky_vector3<T>& v)
{
    T result = v.elem[0] || v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result |= v.elem[i];
    return result;
}

template<typename T> inline bool hxor(const ky_vector3<T>& v)
{
    T result = v.elem[0] ^ v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result ^= v.elem[i];
    return result;
}


/**
        cross product
    */
template<typename T>
inline ky_vector3<T> cross(const ky_vector3<T>& v1, const ky_vector3<T>& v2)
{
    ky_vector3<T> vResult;
    vResult.elem[0] = v1.elem[1] * v2.elem[2] - v1.elem[2] * v2.elem[1];
    vResult.elem[1] = v1.elem[2] * v2.elem[0] - v1.elem[0] * v2.elem[2];
    vResult.elem[2] = v1.elem[0] * v2.elem[1] - v1.elem[1] * v2.elem[0];
    return vResult;
}

/*
        Component-wise absolute value
    */

template<typename T>
inline ky_vector3<T> abs(const ky_vector3<T>& v) {
    using std::abs;
    return ky_vector3<T>(fabs(v.x), fabs(v.y), fabs(v.z));
}


/*
        specialization for length(VectorX<int>)
    */

inline float length(const ky_vector3<int>& v) {
    return std::sqrt( float(lengthSq(v)) );
}

template<typename T>
inline void ky_swap (ky_vector3<T> &v1, ky_vector3<T> &v2)
{
    ky_vector3<T> tmp = v1;
    v1 = v2;
    v2 = tmp;
}

/*
         stream operators
    */

/// output ky_vector3d to stream
template<typename T>
std::ostream& operator << (std::ostream& s, const ky_vector3<T>& v) {
    return (s << "[" << v.elem[0] << " " << v.elem[1] << " " << v.elem[2] << "]");
}

// specialication for (unsigned) int, to make it output numbers and not characters

template<>
inline std::ostream& operator << <unsigned char>(std::ostream& s, const ky_vector3<uchar>& v) {
    return (s << ky_vector3<int>(v));
}

template<>
inline std::ostream& operator << <char>(std::ostream& s, const ky_vector3<char>& v) {
    return (s << ky_vector3<int>(v));
}

/// input ky_vector3d from stream
template<typename T>
std::istream& operator >> (std::istream& s, ky_vector3<T>& v) {
    char ch;

    s >> ch;
    if (ch == '[') {  // format "[ x y z ]"
        s >> v[0] >> v[1] >> v[2] >> ch;
        if (ch != ']') {
            s.clear(std::ios::badbit);
        }
    } else { // format "x y z"
        s.putback(ch);
        s >> v[0] >> v[1] >> v[2];
    }

    return s;
}



template<typename T>
ky_streamb &operator << (ky_streamb &in, const ky_vector3<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        in << v.elem[i];
    return in;
}

template<typename T>
ky_streamb &operator >> (ky_streamb &out, ky_vector3<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
template<typename T>
ky_streamt &operator << (ky_streamt &in, const ky_vector3<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        in << v.elem[i];
    return in;
}

template<typename T>
ky_streamt &operator >> (ky_streamt &out, ky_vector3<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
template<typename T>
ky_variant &operator << (ky_variant &va, const ky_vector3<T> &col)
{
    va.data.append((uint8*)&col, sizeof(ky_vector3<T>));
    va.set("vec3");
    return va;
}
template<typename T>
ky_variant &operator >> (ky_variant &va, ky_vector3<T> &col)
{
    if (va.is_set ("vec3") && va.data.size () == sizeof(ky_vector3<T>))
        col = *((ky_vector3<T>*)va.data.data());
    return va;
}
#endif // VEC3_H
