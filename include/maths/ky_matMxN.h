
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
#ifndef MATMXN_H
#define MATMXN_H
#include "ky_vec4.h"
#include "tools/ky_memory.h"
#include "ky_debug.h"

template<typename T>
struct ky_matrixMxN
{
    T *elem;
    size_t cols;
    size_t rows;

    ky_matrixMxN()
    {
        elem == NULL;
        cols = 0;
        rows = 0;
    }
    ky_matrixMxN(size_t m, size_t n)
    {
        elem == NULL;
        cols = 0;
        rows = 0;
        create(m,n);
    }
    ky_matrixMxN(const ky_matrixMxN<T> &mn)
    {
        elem == NULL;
        cols = 0;
        rows = 0;
        *this = mn;
    }
    template<typename U>
    ky_matrixMxN(const ky_matrixMxN<U> &mn) {
        elem == NULL;
        cols = 0;
        rows = 0;
        *this = mn;
    }
    ~ky_matrixMxN(){
        destroy();
    }

    inline size_t size(){return cols * rows;}
    inline size_t bytecount(){return cols * rows * sizeof(T);}
    inline void create(size_t m, size_t n)
    {
        cols = m;
        rows = n;
        elem = (T*)kyMalloc(sizeof(T) *m*n);
        ky_mem()->zero(elem, sizeof(T) *m*n);
    }
    inline void destroy()
    {
        if (elem && cols)
            kyFree(elem);
        cols = 0;
        rows = 0;
        elem = NULL;
    }

    inline void resize(size_t m, size_t n)
    {
        destroy();
        create(m, n);
    }
    inline void clean()
    {
        ky_mem()->zero (elem, sizeof(T) *cols*rows);
    }

    inline ky_matrixMxN<T> &operator =(const T** mn)
    {
        for (size_t i = 0; i < cols; ++i)
            for (size_t j = 0; j < rows; ++j)
                elem[i*rows+j] = mn[i][j];
        return *this;
    }
    inline ky_matrixMxN<T> &operator =(const ky_matrixMxN<T> &mn)
    {
        destroy();
        create(mn.cols, mn.rows);
        ky_mem()->copy (elem, mn.elem, sizeof(T) *mn.cols*mn.rows);

        return *this;
    }
    template<typename U>
    inline ky_matrixMxN<T> &operator =(const ky_matrixMxN<U> &mn)
    {
        destroy();
        create(mn.cols, mn.rows);
        for (size_t i = 0; i < cols *rows; ++i)
                elem[i] = mn.elem[i];

        return *this;
    }

    inline T* operator [](size_t mi)
    {
        kyASSERT((mi >= cols), "i must be less than size");
        return &elem[mi*rows];
    }
    inline const T* operator [](size_t mi)const
    {
        kyASSERT((mi >= cols), "i must be less than size");
        return &elem[mi*rows];
    }
    inline T& pos(size_t mi, size_t ni)
    {
        kyASSERT((mi >= cols) && (ni >= rows), "i must be less than size");
        return elem[mi*rows+ni];
    }
    inline const T& pos(size_t mi, size_t ni)const
    {
        kyASSERT((mi >= cols) && (ni >= rows), "i must be less than size");
        return elem[mi*rows+ni];
    }
    inline T* pos(size_t mi)
    {
        return *this[mi];
    }
    inline const T* pos(size_t mi)const
    {
        return *this[mi];
    }

};

typedef ky_matrixMxN<float> ky_matMxNi;
typedef ky_matrixMxN<float> ky_matMxNf;
typedef ky_matrixMxN<real> ky_matMxNd;

typedef ky_matrixMxN<int>  imatMxN;
typedef ky_matrixMxN<float> fmatMxN;
typedef ky_matrixMxN<real> dmatMxN;

template<typename T>
ky_streamb &operator << (ky_streamb &in, const ky_matrixMxN<T> &v)
{
    in << v.cols;
    in << v.rows;
    in << ky_byte((uint8*)v.elem, sizeof(T)*v.cols*v.rows);
    return in;
}

template<typename T>
ky_streamb &operator >> (ky_streamb &out, ky_matrixMxN<T> &v)
{
    ky_byte tmp;
    out >> v.cols;
    out >> v.rows;
    out >> tmp;
    v.resize(v.cols, v.rows);
    ky_mem()->copy(v.elem, tmp.data(), tmp.size());

    return out;
}
template<typename T>
ky_streamt &operator << (ky_streamt &in, const ky_matrixMxN<T> &v)
{
    in << v.cols;
    in << v.rows;
    in << '\n';
    for (size_t m = 0; m < v.cols; ++m)
    {
        for (size_t n = 0; n < v.rows; ++n)
            in <<  v[m][n];
        in << '\n';
    }
    return in;
}
template<typename T>
ky_streamt &operator >> (ky_streamt &out, ky_matrixMxN<T> &v)
{
    size_t col = 0, row = 0;
    char tm;
    out >> col >> row >> tm;
    v.resize(col, row);
    for (size_t m = 0; m < col; ++m)
    {
        for (size_t n = 0; n < row; ++n)
            out >> v[m][n];
        out >> tm;
    }
    return out;
}
#endif // MAT4X4_H
