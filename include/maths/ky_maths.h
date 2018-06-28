
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
#ifndef MATHS_H
#define MATHS_H

#include "ky_define.h"
#include <math.h>
#include <float.h>
#include <ctime>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#ifdef PI
#undef PI
#endif

#if defined(DBL_EPSILON)
const real EPSILON = DBL_EPSILON;
#else
const real EPSILON = 2.2204460492503131e-16;
#endif
const float  FLOAT_EPS     = 1.0e-7f;
const float  FLOAT_EPS_SQ  = 1.0e-14f;
const float  FLOAT_PI      = 3.14159265358979323846f;
const real  DOUBLE_EPS    = 1.0e-14;
const real  DOUBLE_EPS_SQ = 1.0e-28;
const real  DOUBLE_PI     = 3.14159265358979323846;
const real PIdiv2 = 1.57079632679489661923;
const real PI = 3.14159265358979323846;
const float PIf = 3.14159265358979323846f;
const float PIdiv2f = 1.57079632679489661923f;
#ifdef TWO_PI
#undef TWO_PI
#endif
const real TWO_PI =  6.28318530717958647693;
const real TWO_PIf =  6.28318530717958647693f;

const real SQRT_EPSILON = 1.490116119385000000e-8;
/*
    conversion from degree to radian and vice versa
*/

/// convert a radian to degree
/// 弧度转换角度
inline float ky_rad2deg(float f) {
    static const float rdf = 57.2957795133;
    return rdf * f;
}

/// convert a radian to degree
/// 弧度转换角度
inline real ky_rad2deg(real d ) {
    static const real rdr = 57.2957795133;
    return rdr * d;
}

/// convert a degree to radian
///  角度转换弧度
inline float ky_deg2rad(float f) {
    static const float drf = 0.01745329252;
    return drf * f;
}

/// convert a degree to radian
/// 角度转换弧度
inline real ky_deg2rad(real d ) {
    static const float drr = 0.01745329252;
    return drr * d;
}

/*
    sign
*/

inline float ky_sign(float f) {
    return f == 0.f ? 0.f : f / std::fabs(f);
}

inline real ky_sign(real d) {
    return d == 0.0 ? 0.0 : d / std::fabs(d);
}

inline int ky_sign(int i) {
    return i == 0 ? 0 : i / std::fabs(i);
}

inline int ky_isign(float f) {
    return static_cast<int>(ky_sign(f));
}

inline int ky_isign(real d) {
    return static_cast<int>(ky_sign(d));
}

/*
    floor
*/

// float floor(float f) defined in std c++
// double floor(double d) defined in std c++

/// Return the largest integer not greater than \p f.
inline int ky_ifloor(float f) {
    return static_cast<int>(std::floor(f));
}

/// Return the largest integer not greater than \p d.
inline int ky_ifloor(real d) {
    return static_cast<int>(std::floor(d));
}

/*
    ceil
*/

// float ceil(float f) defined in std c++
// double ceil(double d) defined in std c++

/// Return the smallest integer not less than \p f.
inline int ky_iceil(float f) {
    return static_cast<int>(std::ceil(f));
}

/// Return the smallest integer not less than \p d.
inline int ky_iceil(real d) {
    return static_cast<int>(std::ceil(d));
}

/*
    fix
*/

/// Truncate \p f towards zero.
inline float ky_fix(float f) {
    return std::fabs(f) * ky_sign(f);
}

/// Truncate \p d towards zero.
inline double ky_fix(real d) {
    return std::fabs(d) * ky_sign(d);
}

/// Truncate \p f towards zero.
inline int ky_ifix(float f) {
    return static_cast<int>(ky_fix(f));
}

/// Truncate \p d towards zero.
inline int ky_ifix(real d) {
    return static_cast<int>(ky_fix(d));
}

/*
    round
*/

/// Return the integer nearest to \p f.
inline float ky_round(float f) {
    return std::floor(f + 0.5f);
}

/// Return the integer nearest to \p d.
inline double ky_round(real d) {
    return std::floor(d + 0.5);
}

/// Return the integer nearest to \p f.
inline int ky_iround(float f) {
    return static_cast<int>(round(f));
}

/// Return the integer nearest to \p d.
inline int ky_iround(real d) {
    return static_cast<int>(round(d));
}

/*
    clamp
*/

/// Clamps \p f to range [\p min, \p max].
inline float ky_clamp(float f, float min, float max) {
    return std::min( std::max(f,min), max );
}

/// Clamps \p f to range [\p min, \p max].
inline double ky_clamp(real f, real min, real max) {
    return std::min(std::max(f, min), max);
}

/// Clamps \p f to range [\p min, \p max].
inline int ky_clamp(int f, int min, int max) {
    return std::min(std::max(f, min), max);
}

/*
    floating point checks
 */

/// Returns false, if \p f is not a number (NaN).
inline bool ky_isNumber(float f) {
    // according to the IEEE-754 floating-point standard
    // comparisons with NaN always fail
    return f == f;
}

/// Returns false, if \p d is not a number (NaN).
inline bool ky_isNumber(real d) {
    // according to the IEEE-754 floating-point standard
    // comparisons with NaN always fail
    return d == d;
}

/// Returns true, if \p value with the type T is not a number (NaN).
template<typename T>
inline bool ky_isNaN(const T& value) {
    return (value != value);
}

/// Returns false, if \p f is NaN or infinity.
inline bool ky_isFiniteNumber(float f) {
    return (f <= FLT_MAX && f >= -FLT_MAX);
}

/// Returns false, if \p d is NaN or infinity.
inline bool ky_isFiniteNumber(real d) {
    return (d <= DBL_MAX && d >= -DBL_MAX);
}

inline float ky_fmap(float value, float inputMin, float inputMax, float outputMin,
                 float outputMax, bool clamp= false)
{
    #ifndef FLT_EPSILON
    #define FLT_EPSILON	__FLT_EPSILON__
    #endif
    if (fabs(inputMin - inputMax) < FLT_EPSILON){
        return outputMin;
    } else {
        float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);

        if( clamp ){
            if(outputMax < outputMin){
                if( outVal < outputMax )outVal = outputMax;
                else if( outVal > outputMin )outVal = outputMin;
            }else{
                if( outVal > outputMax )outVal = outputMax;
                else if( outVal < outputMin )outVal = outputMin;
            }
        }
        return outVal;
    }
}
//!
//! \brief ky_irand 取count个随机int型数组
//! \param count
//! \return
//!
template <typename T>
inline T* ky_rand(T *buf, size_t len)
{
    srand((uint)time(NULL));
    for (size_t i = 0; i < len; ++i)
    {
        if (typeid(T) == typeid(float))
            buf[i] = rand() / float(RAND_MAX);
        else if (typeid(T) == typeid(real))
            buf[i] = rand() / real(RAND_MAX);
        else
            buf[i] = rand();
    }
    return buf;
}
//!
//! \brief ky_irand 取count个以a开始的n为范围内的随机int型数组
//! \param count
//! \return
//!
template <typename T>
inline T* ky_rand(T*buf, size_t len, int a, int n)
{
    srand((uint)time(NULL));
    n = n == 0 ? 1 : n;
    for (size_t i = 0; i < len; ++i)
    {
        if (typeid(T) == typeid(float))
            buf[i] = a+ rand() %n / float(RAND_MAX);
        else if (typeid(T) == typeid(real))
            buf[i] = a+ rand() %n / real(RAND_MAX);
        else
            buf[i] = a+ rand() %n;
    }
    return buf;
}

#endif
