
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
#ifndef ky_aabb_H
#define ky_aabb_H
#include "ky_define.h"
#include "ky_mat4X4.h"
#include "ky_plane.h"
#include "tools/ky_stream.h"

// Axis-aligned bounding box
template<typename T>
class ky_aabb
{
public:
    typedef T tElemTypes;
public:
    ky_aabb() { }
    explicit ky_aabb(const ky_vector3<T>& v): min_(v),max_(v){}

    ky_aabb(const ky_vector3<T>& v1, const ky_vector3<T>& v2):
        min_(v1),max_(v1) {append(v2);}

    inline void append(const ky_vector3<T>& v)
    {
        min_ = ky_min(min(), v);
        max_ = ky_max(max(), v);
    }

    inline void append(const ky_aabb& b) {
        append(b.min());
        append(b.max());
    }

    inline ky_vector3<T> min() const { return min_; }
    inline ky_vector3<T> max() const { return max_; }

    inline T width() const{return diagonal().x;}
    inline T height() const{return diagonal().y;}
    inline T depth() const{return diagonal().z;}

    inline ky_aabb& operator |= (const ky_aabb  &rhs)
    {
        min_ = ::min(min(), rhs.min());
        max_ = ::max(max(), rhs.max());
        return *this;
    }
    inline ky_aabb& operator &= (const ky_aabb  &rhs)
    {
        min_ = ::max(min(), rhs.min());
        max_ = ::min(max(), rhs.max());
        return *this;
    }
    inline ky_aabb& operator /= (T rhs)
    {
        return this->operator *= ((T)1.0 / rhs);
    }
    inline ky_aabb& operator *= (T rhs)
    {
        min_ *= rhs;
        max_ *= rhs;
        return *this;
    }
    inline ky_aabb& operator -= (const ky_vector3<T>& rhs)
    {
        min_ -= rhs;
        max_ -= rhs;
        return *this;
    }
    inline ky_aabb& operator += (const ky_vector3<T>& rhs)
    {
        min_ += rhs;
        max_ += rhs;
        return *this;
    }
    inline ky_aabb operator - () const
    {
        return ky_aabb(-this->max(), -this->min());
    }

    ky_aabb(const ky_aabb<T> & v){
        *this = v;
    }

    inline ky_aabb<T> &operator = (const ky_aabb<T> &v){
        min_ = v.min_;
        max_ = v.max_;
        return *this;
    }
    //!
    //! \brief center 返回中心点
    //! \return
    //!
    inline ky_vector3<T> center() const {return (diagonal() * 0.5f + min());}
    //!
    //! \brief diagonal 返回对角线尺寸
    //! \return
    //!
    inline ky_vector3<T> diagonal() const {return (max_ - min_);}
    //!
    //! \brief corner 返回角顶点
    //! \param index
    //! \return
    //!
    inline ky_vector3<T> corner(const ky_vector3<T> &index) const
    {
        ky_vector3<T> _corner;
        _corner.x = (index.x >0) ? max_.x : min_.x;
        _corner.y = (index.y >0) ? max_.y : min_.y;
        _corner.z = (index.z >0) ? max_.z : min_.z;
        return _corner;
    }
    //!
    //! \brief corners 返回8个角的顶点
    //! \param corners
    //! \return
    //!
    inline bool corners(ky_vector3<T> cs[8] ) const
    {
        int n = 0;
        if (is_valid())
        {
            ky_vector3<T> P;
            int i,j,k;
            for( i = 0; i < 2; i++ )
            {
                P.x = (i) ? max().x : min().x;
                for ( j = 0; j < 2; j++ )
                {
                    P.y = (j) ? max().y : min().y;
                    for ( k = 0; k < 2; k++ )
                    {
                        P.z = (k) ? max().z : min().z;
                        cs[n++] = P;
                    }
                }
            }
        }
        return (8 == n);
    }

    //!
    //! \brief volume 返回边界框的体积
    //! \return
    //!

    inline T volume() const {return fabs((min().x - max().x) * (min().y - max().y) * (min().z - max().z));}

    //!
    //! \brief distance 返回点v到边界框的最短距离
    //! \param v
    //! \return
    //!
    inline T distance (const ky_vector3<T>& v) const
    {
        return sqrt(distanceSq(v));
    }
    inline T distanceSq (const ky_vector3<T>& v)const
    {
        if (contains(v))
            return T(0);
        else
        {
            ky_vector3<T> maxDist(0,0,0);

            if (v.x < min().x)
                maxDist.x = min().x - v.x;
            else if (v.x > max().x)
                maxDist.x = v.x - max().x;

            if (v.y < min().y)
                maxDist.y = min().y - v.y;
            else if (v.y > max().y)
                maxDist.y = v.y - max().y;

            if (v.z < min().z)
                maxDist.z = min().z - v.z;
            else if (v.z > max().z)
                maxDist.z = v.z - max().z;

            return lengthSq(maxDist);
        }
    }

    //!
    //! \brief is_valid 返回边界框是否有效
    //! \return
    //!
    inline bool is_valid() const { return min() != max() && min() < max(); }

    //!
    //! \brief contains 是否包含p点
    //! \param p
    //! \return
    //!
    inline bool contains(const ky_vector3<T>& p)
    {
        return ( (p.x >= min().x) && (p.y >= min().y) && (p.z >= min().z) &&
                 (p.x <= max().x) && (p.y <= max().y) && (p.z <= max().z) );
    }

    //!
    //! \brief contains 是否包含b边界框
    //! \param b
    //! \return
    //!
    inline bool contains(const ky_aabb& b) {return ( contains(b.min()) && contains(b.max()) );}

    //!
    //! \brief intersects 返回边界框是否相交
    //! \param b
    //! \return
    //!
    inline bool intersects(const ky_aabb& b) const
    {
        if ((min().x > b.max().x) || (b.min().x > max().x))
            return false;
        if ((min().y > b.max().y) || (b.min().y > max().y))
            return false;
        if ((min().z > b.max().z) || (b.min().z > max().z))
            return false;

        return true;
    }
    //!
    //! \brief intersects 是否和平面p相交
    //! \param p
    //! \return
    //!
    inline bool intersects(const ky_plane<T>& p) const
    {
        bool pointsNeg = false;
        bool pointsPos = false;

        T d = p.distance(vec3(min().x, min().y, min().z));
        if(d < 0.0) pointsNeg = true; else if(d > 0.0) pointsPos = true;
        d = p.distance(ky_vector3<T>(max().x, min().y, min().z));
        if(d < 0.0) pointsNeg = true; else if(d > 0.0) pointsPos = true;
        d = p.distance(ky_vector3<T>(min().x, max().y, min().z));
        if(d < 0.0) pointsNeg = true; else if(d > 0.0) pointsPos = true;
        d = p.distance(ky_vector3<T>(min().x, min().y, max().z));
        if(d < 0.0) pointsNeg = true; else if(d > 0.0) pointsPos = true;
        d = p.distance(ky_vector3<T>(max().x, max().y, min().z));
        if(d < 0.0) pointsNeg = true; else if(d > 0.0) pointsPos = true;
        d = p.distance(ky_vector3<T>(min().x, max().y, max().z));
        if(d < 0.0) pointsNeg = true; else if(d > 0.0) pointsPos = true;
        d = p.distance(ky_vector3<T>(max().x, min().y, max().z));
        if(d < 0.0) pointsNeg = true; else if(d > 0.0f) pointsPos = true;
        d = p.distance(ky_vector3<T>(max().x, max().y, max().z));
        if(d < 0.0) pointsNeg = true; else if(d > 0.0) pointsPos = true;

        return (pointsNeg && pointsPos);
    }

    inline bool inside_xz(const ky_aabb& bound) const
    {
        kyASSERT(       is_valid (), "This Box ist not valid.");
        kyASSERT(bound.is_valid(), "Box b ist not valid.");

        ky_vector3<T> llfb = bound.min();
        ky_vector3<T> urbb = bound.max();

        T r0x = ky_min(min()[0], max()[0]);
        T r1x = ky_max(min()[0], max()[0]);
        T r0y = ky_min(min()[2], max()[2]);
        T r1y = ky_max(min()[2], max()[2]);
        T r2x = ky_min(llfb[0], urbb[0]);
        T r3x = ky_max(llfb[0], urbb[0]);
        T r2y = ky_min(llfb[2], urbb[2]);
        T r3y = ky_max(llfb[2], urbb[2]);

        return (r0x >= r2x) && (r0y >= r2y) && (r1x <= r3x) && (r1y <= r3y);
    }
    inline bool inside_xz(const vec3& v) const {
        kyASSERT(  is_valid(), "This Box ist not defined.");

        return (min()[0] <= v[0]) && (v[0] <= max()[0])
                && (min()[2] <= v[2]) && (v[2] <= max()[2]);
    }

    //!
    //! \brief inside 返回bound边界框是否在内部
    //! \param bound
    //! \return
    //!
    inline bool inside(const ky_aabb& bound) const {
        kyASSERT(       is_valid(), "This Box ist not defined.");
        kyASSERT(bound.is_valid(), "Box b ist not defined.");

        vec3 llfb = bound.min();
        vec3 urbb = bound.max();

        T r0x = ky_min(min()[0], max()[0]);
        T r1x = ky_max(min()[0], max()[0]);
        T r0y = ky_min(min()[1], max()[1]);
        T r1y = ky_max(min()[1], max()[1]);
        T r0z = ky_min(min()[2], max()[2]);
        T r1z = ky_max(min()[2], max()[2]);

        T r2x = ky_min(llfb[0], urbb[0]);
        T r3x = ky_max(llfb[0], urbb[0]);
        T r2y = ky_min(llfb[1], urbb[1]);
        T r3y = ky_max(llfb[1], urbb[1]);
        T r2z = ky_min(llfb[2], urbb[2]);
        T r3z = ky_max(llfb[2], urbb[2]);

        return (r0x >= r2x) && (r1x <= r3x) &&
                (r0y >= r2y) && (r1y <= r3y) &&
                (r0z >= r2z) && (r1z <= r3z);
    }

    //!
    //! \brief inside 返回v点是否在内部
    //! \param v
    //! \return
    //!
    inline bool inside(const ky_vector3<T>& v) const {
        kyASSERT(  is_valid(), "This Box ist not valid.");

        return (min()[0] <= v[0]) && (v[0] <= max()[0]) &&
                (min()[1] <= v[1]) && (v[1] <= max()[1]) &&
                (min()[2] <= v[2]) && (v[2] <= max()[2]);
    }

    inline ky_aabb transform(const ky_matrix4X4<T>& m) const
    {
        ky_aabb b;
        b.append(m * ky_vector3<T>(min().x, min().y, min().z));
        b.append(m * ky_vector3<T>(max().x, min().y, min().z));
        b.append(m * ky_vector3<T>(min().x, max().y, min().z));
        b.append(m * ky_vector3<T>(min().x, min().y, max().z));
        b.append(m * ky_vector3<T>(max().x, max().y, min().z));
        b.append(m * ky_vector3<T>(min().x, max().y, max().z));
        b.append(m * ky_vector3<T>(max().x, min().y, max().z));
        b.append(m * ky_vector3<T>(max().x, max().y, max().z));
        return b;
    }


    friend ky_streamb &operator << (ky_streamb &in, const ky_aabb &v)
    {
        in << v.min_;
        in << v.max_;
        return in;
    }
    friend ky_streamb &operator >> (ky_streamb &out, ky_aabb &v)
    {
        out >> v.min_;
        out >> v.max_;
        return out;
    }
    friend ky_streamt &operator << (ky_streamt &in, const ky_aabb &v)
    {
        in << v.min_;
        in << v.max_;
        return in;
    }
    friend ky_streamt &operator >> (ky_streamt &out, ky_aabb &v)
    {
        out >> v.min_;
        out >> v.max_;
        return out;
    }

private:
    ky_vector3<T> min_; //lower left front
    ky_vector3<T> max_; //upper right back
};

typedef ky_aabb<int> iaabb;
typedef ky_aabb<float> faabb;
typedef ky_aabb<real> daabb;

template<typename T>
ky_variant &operator << (ky_variant &va, const ky_aabb<T> &col)
{
    va.data.append((uint8*)&col, sizeof(ky_aabb<T>));
    va.set("aabb");
    return va;
}
template<typename T>
ky_variant &operator >> (ky_variant &va, ky_aabb<T> &col)
{
    if (va.is_set ("aabb") && va.data.size () == sizeof(ky_aabb<T>))
        col = *((ky_aabb<T>*)va.data.data());
    return va;
}
#endif // ky_aabb_H
