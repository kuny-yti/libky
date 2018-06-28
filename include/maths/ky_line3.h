
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
#ifndef ky_line3_H
#define ky_line3_H

#include "ky_vec3.h"

///This implements a 3D line(segment).
template <typename T>
class ky_line3 {
    ky_vector3<T> p_; /// position
    ky_vector3<T> d_; /// direction

public:
    /// trivial standard constructor
    ky_line3() {}

    /// construct a line from two points on a line
    ky_line3(const ky_vector3<T>& start, const ky_vector3<T>& end)
        : p_(start)
        , d_(end - start)
    {}

    ky_line3(const ky_line3<T> &v)
    {
        p_ = v.p_;
        d_ = v.d_;
    }
    inline ky_line3<T> &operator = (const ky_line3<T> &c)
    {
        p_ = c.p_;
        d_ = c.d_;
        return *this;
    }
    /**
        * Interpolates between start and end
        * @param t Should be betweeen 0 and 1.
        * @return (start*(1-t) + end*t)
        */
    inline ky_vector3<T> interpolates(T t) const {
        return p_ + t * d_;
    }

    inline ky_vector3<T> start() const { return p_; }
    inline ky_vector3<T> end() const { return p_ + d_; }
    ///Set startingpoint to start, leaves end unchanged.
    inline void start(const ky_vector3<T>& s) { d_ = (p_ + d_) - s; p_ = s; }
    ///Set endpoint to end, leaves start unchanged.
    inline void end(const ky_vector3<T>& e) { d_ = (e - p_); }

    ///@return Vector from start to end
    inline ky_vector3<T> direction() const { return d_; }
    ///Set direction, changing endpoint, leaves start unchanged.
    inline void direction(const ky_vector3<T>& dir) { d_ = dir; }

    /**
         * intersect a linesegment with another linesegment
         * See RTR p618
         *
         * @param l the line to intersect this object with
         * @param v holds the result if an intersection took place,
         * otherwise it remains unchanged
         * @return true if these lines intersect
        */
    inline bool intersectSegments(const ky_line3<T>& l, ky_vector3<T>& v) const;

    /**
         * intersect a line with another line
         * See RTR p618
         *
         * @param l the line to intersect this object with
         * @param v holds the result if an intersection took place,
         * otherwise it remains unchanged
         * @return true if these lines intersect
        */
    inline bool intersect(const ky_line3<T>& l, ky_vector3<T>& v) const;
};

//------------------------------------------------------------------------------
//  non inline implementation
//------------------------------------------------------------------------------

template <typename T>
bool ky_line3<T>::intersectSegments(const ky_line3<T>& l, ky_vector3<T>& v) const {
    ky_vector3<T> c = cross(d_, l.d_);
    T q = lengthSq(c);

    if (std::fabs(q) < 0.00001f)
        return false;
    else {
        ky_vector3<T> o = l.p_ - p_;
        ky_vector3<T> d = l.d_;

        T t = o.x*d_.y*c.z + d_.x*c.y*o.z + c.x*o.y*d_.z
                - c.x*d_.y*o.z - d_.x*o.y*c.z - o.x*c.y*d_.z;

        T s = o.x*l.d_.y*c.z + l.d_.x*c.y*o.z + c.x*o.y*l.d_.z
                - c.x*l.d_.y*o.z - l.d_.x*o.y*c.z - o.x*c.y*l.d_.z;

        t /= q;
        s /= q;

        v = p_ + t*d_;
        if( (s >= 0.0) && (s <= 1.0)
                &&(t >= 0.0) && (t <= 1.0) )
            return true;
        else
            return false;
    }
}

template <typename T>
bool ky_line3<T>::intersect(const ky_line3<T>& l, ky_vector3<T>& v) const {
    ky_vector3<T> c = cross(d_, l.d_);
    T q = lengthSq(c);

    if (std::fabs(q) < 0.00001f)
        return false;
    else {
        ky_vector3<T> o = l.p_ - p_;
        ky_vector3<T> d = l.d_;

        T t = o.x*d_.y*c.z + d_.x*c.y*o.z + c.x*o.y*d_.z
                - c.x*d_.y*o.z - d_.x*o.y*c.z - o.x*c.y*d_.z;

        t /= q;
        v = p_ + t*d_;

        return true;
    }
}

template <typename T>
std::ostream& operator << (std::ostream& s, const ky_line3<T>& l) {
    return (s << "x = " << l.start() << " + t * " << l.direction());
}

typedef ky_line3<int>     iline3;
typedef ky_line3<float>   fline3;
typedef ky_line3<real>    dline3;


template<typename T>
ky_streamb &operator << (ky_streamb &in, const ky_line3<T> &v)
{
    in << v.start();
    in << v.direction();
    return in;
}

template<typename T>
ky_streamb &operator >> (ky_streamb &out, ky_line3<T> &v)
{
    ky_vector3<T> tmp;
    out >> tmp; v.start(tmp);
    out >> tmp; v.direction(tmp);
    return out;
}
template<typename T>
ky_streamt &operator << (ky_streamt &in, const ky_line3<T> &v)
{
    in << v.start();
    in << v.direction();
    return in;
}
template<typename T>
ky_streamt &operator >> (ky_streamt &out, ky_line3<T> &v)
{
    ky_vector3<T> tmp;
    out >> tmp; v.start(tmp);
    out >> tmp; v.direction(tmp);
    return out;
}
#endif // ky_line3_H
