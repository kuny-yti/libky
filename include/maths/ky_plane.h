

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
#ifndef ky_plane_H
#define ky_plane_H
#include "ky_vec4.h"
#include "ky_line3.h"

/**
 * @brief a ky_plane class
 * The ky_plane is stored in hessian normal form: <br>
 * n*x + d = 0; <br>
 * n is the unit normal of this ky_plane pointing from the origin to this ky_plane <br>
 * (-d) >= 0 is the distance from the origin. <br>
 * <br>
 * Note: You can transform this ky_plane like this: <br>
 * myky_plane.toVec4() = myMatrix4 * myky_plane.toVec4(); <br>
 * <br>
 * Perhaps you want to do this: <br>
 * mat4 mv = tgt::getModelViewMatrix(); <br>
 * mat4 inv; <br>
 * mv.invert(inv); <br>
 * myky_plane.toVec4() = inv * myky_plane.toVec4(); <br>
*/
template<typename T>
struct ky_plane {
    typedef T ElemType;

    /// The normal of the ky_plane.
    ky_vector3<T> n;
    /// The negative distance from the orign.
    T d;

    /// trivial default constructor
    ky_plane() {}

    /// init with unit normal [a, b, c] and distance _d <br>
    /// BEWARE: It is your job to ensure that [a, b, c] is a unit vector and that d <= 0
    ky_plane(T a, T b, T c, T _d)
        : n(a, b, c)
        , d(_d)
    {}

    /// creates a ky_plane with a unit length normal and a distance <br>
    /// BEWARE: It is your job to ensure that [a, b, c] is a unit vector and that d <= 0
    ky_plane(const ky_vector3<T>& _n, T _d)
        : n(_n)
        , d(_d)
    {}

    /**
     * Creates a ky_plane defined by three points.
     */
    ky_plane(const ky_vector3<T>& v1, const ky_vector3<T>& v2, const ky_vector3<T>& v3) {
        n = normalize( cross(v1 - v2, v2 - v3) );
        d = -dot(v1, n);

        // correct sign of d
        if (d > T(0)) {
            n = -n;
            d = -d;
        }
    }

    /// creates a ky_plane with one from another type
    template<typename U>
    ky_plane(const ky_plane<U>& p)
        : n(p.n)
        , d(T(p.d))
    {}
    ky_plane(const ky_plane<T> &v)
    {
        n = v.n;
        d = v.d;
    }
    inline ky_plane<T> &operator = (const ky_plane<T> &c)
    {
        n = c.n;
        d = c.d;
        return *this;
    }

    inline const ky_vector4<T>& to_vec4() const {
        return *reinterpret_cast<ky_vector4<T>*>(const_cast<ky_plane<T>*>(this)->n.elem);
    }

    inline ky_vector4<T>& to_vec4() {
        return *reinterpret_cast<ky_vector4<T>*>(n.elem);
    }

    /**
     * Returns the signed distance to this ky_plane to a given point. <br>
     * If result < 0 -> v lies in the same half space of the ky_plane like the origin. <br>
     * If result > 0 -> v lies in the other half space of the ky_plane like the origin.
     * @return the signed distance from the ky_plane to the point
    */
    inline T distance(const ky_vector3<T>& v) const {
        return dot(n, v) + d;
    }

    /**
     * Returns whether this ky_plane intersects with line \p l.
     * When there is an intersection the parameter to compute
     * the point will be returned in \p f.
     * If \p l represents an edge between v1 and v2 that were given to \p l
     * via the constructor an intersection takes place when \p f is in [0, 1]
    */
    inline bool intersect(const ky_line3<T>& l, T& f) const;

    /**
     * Returns whether this ky_plane intersects with \p p.
     * When there is an intersection it will be computed in \p l
    */
    inline bool intersect(const ky_plane<T>& p, ky_line3<T>& l) const;

    /**
     * clips a given polygon to this ky_plane
     * @param vin the input polygon
     * @param vout the output polygon
    */
    inline void clipPolygon(const ky_vector< ky_vector3<T> >& vin, ky_vector< ky_vector3<T> >& vout) const;

    /**
     * Clips a given polygon to this ky_plane.
     * This is a second version with c-arrays. Can be faster, but beware:
     * Buffer overruns are likly!!!
     * @param vin the input polygon
     * @param numIn number of input vertices
     * @param vout the output polygon
     * @param numOut number of output vertices
    */
    inline void clipPolygon(const ky_vector3<T>* vin, size_t numIn, ky_vector3<T>* vout, size_t& numOut) const;

    static void createCubeVertices(const ky_vector3<T>& llf, const ky_vector3<T>& urb, ky_vector3<T> cubeVertices[8]);

    static void createCubeky_planes(const ky_vector3<T> cv[8], ky_plane<T> cubeky_planes[6]);

    /**
     * Creates ky_planes in the aabky_planes array from a axis aligned box given via llf and urb. The normal points to inside.
     * @param llf lower left front of the input axis aligned box
     * @param urb upper right back of the input axis aligned box
     * @param cubeky_planes the result is written to this array which must be able to hold at least 6 ky_planes
     */
    static void createAABky_planes(const ky_vector3<T>& llf, const ky_vector3<T>& urb, ky_plane<T> aabky_planes[6]) {
        ky_vector3<T> cv[8];// cube vertices
        createCubeVertices(llf, urb, cv);

        // cube ky_planes
        createCubeky_planes(cv, aabky_planes);
    }

    /**
     * This method creates a polygon from this ky_plane.
     * It is ensured that this polygon is huge enough
     * so it may be clipped later against the axis aligned box
     * defined by llf and urb. <br>
     * Note that you can use this method with Vector, too: <br>
     * Vector\<vec3\> vout(4);
     * createQuad(llf, urb, &vout[0]);
     * @param llf lower left front of the input axis aligned box
     * @param urb upper right back of the input axis aligned box
     * @param v the result is written to this array which must be able to hold at least 4 vertices
     */
    inline void createQuad(ky_vector3<T> llf, ky_vector3<T> urb, ky_vector3<T>* v) const;

    /**
     * This routine clips a polygon against 6 ky_planes
     * @param aabky_planes against these 6 ky_planes is clipped
     * @param vin Points to the input vertices. <br>
     *      BEWARE: vin is changed after this call. <br>
     *      AND EVEN WORSE: The size should be enough. Buffer overruns are likely!!!
     * @param numIn number of vertices in vin
     * @param vout Points to the output vertices. This is the result. <br>
     *      BEWARE: The size should be enough. Buffer overruns are likely!!!
     * @param numOut Number of output vertices. This a result, too.
     */
    static void clipAAB(ky_plane<T> aabky_planes[6], ky_vector3<T>* vin, size_t numIn, ky_vector3<T>* vout, size_t& numOut);

    /**
     * This routine clips a polygon against 6 ky_planes
     * @param aabky_planes against these 6 ky_planes is clipped
     * @param vin The input vertices. <br>
     *      BEWARE: vin is changed after this call.
     * @param vout Points to the output vertices. This is the result,
     */
    static void clipAAB(ky_plane<T> aabky_planes[6], ky_vector< ky_vector3<T> >& vin, ky_vector< ky_vector3<T> >& vout);

    /**
     * This method cilps this ky_plane against an axis aligned box defined by llf and urb.
     * The output is written to vout. <br>
     * BEWARE: It is likely that you can gain performance by precomputing the ky_planes of the AAB once,
     * then calculate a proper polygon with createQuad and finally call a static clipAAB version
     * which takes the ky_planes an the polygon as input.
     * @param llf lower left front of the input axis aligned box
     * @param urb upper right back of the input axis aligned box
     * @param vout the result polygon
     */
    inline void clipAAB(const ky_vector3<T>& llf, const ky_vector3<T>& urb, ky_vector< ky_vector3<T> >& vout) const;
};

// non inline implementation

template<typename T>
bool ky_plane<T>::intersect(const ky_line3<T> &l, T& f) const {
    T f0 = distance(l.start());
    T f1 = -dot(n, l.direction());

    if (std::fabs(f1) < 0.00001f)
        return false;
    else {
        f = f0 / f1;
        return true;
    }
}

template<typename T>
bool ky_plane<T>::intersect(const ky_plane<T>& p, ky_line3<T> &l) const {
    ky_vector3<T> pn = p.n;
    T n00 = dot( n,  n);
    T n01 = dot( n, pn);
    T n11 = dot(pn, pn);
    T det = n00 * n11 - n01 * n01;

    if (std::fabs(det) < 0.00001f)
        return false;
    else {
        T inv_det = 1.0f/det;
        T c0 = (-n11 *   -d + n01 * -p.d) * inv_det;
        T c1 = (-n00 * -p.d + n01 *   -d) * inv_det;
        l.start(n * c0 + pn * c1);
        l.direction(cross(n, pn));
        return true;
    }
}

template<typename T>
void ky_plane<T>::clipPolygon(const ky_vector< ky_vector3<T> >& vin, ky_vector< ky_vector3<T> >& vout) const {
    // temporary array for distances
    ky_vector<T> vd;
    vd.reserve(vin.size());

    for (size_t i = 0; i < vin.size(); ++i)
        vd.push_back( distance(vin[i]) );

    for (size_t i1 = 0, i2 = 1; i1 < vin.size(); ++i1, ++i2) {
        // wrap i2 around
        i2 = ( i2 == vin.size() ) ? 0 : i2;

        // inside-inside
        if (vd[i1] < T(0) && vd[i2] < T(0))
            vout.push_back( vin[i2] );
        // inside-outside OR outside-inside
        else if (vd[i1] * vd[i2] < 0.f) {
            T scale = vd[i1] / (vd[i1] - vd[i2]);
            // interpolate vertex
            ky_vector3<T> newv = vin[i1] + (vin[i2] - vin[i1]) * scale;
            vout.push_back(newv);

            // outside-inside
            if (vd[i1] > T(0) && vd[i2] < T(0))
                vout.push_back( vin[i2] );
        }
        // do nothing for outside-outside
    }
}

template<typename T>
void ky_plane<T>::clipPolygon(const ky_vector3<T>* vin, size_t numIn, ky_vector3<T>* vout, size_t& numOut) const {
    // temporary array for distances
    T* vd = new T[numIn];
    numOut = 0;

    for (size_t i = 0; i < numIn; ++i)
        vd[i] = distance(vin[i]);

    for (size_t i1 = 0, i2 = 1; i1 < numIn; ++i1, ++i2) {
        // wrap i2 around
        i2 = ( i2 == numIn ) ? 0 : i2;

        // inside-inside
        if (vd[i1] < T(0) && vd[i2] < T(0)) {
            vout[numOut] = vin[i2];
            ++numOut;
        }
        // inside-outside OR outside-inside
        else if (vd[i1] * vd[i2] < 0.f) {
            T scale = vd[i1] / (vd[i1] - vd[i2]);
            // interpolate vertex
            vout[numOut] = vin[i1] + (vin[i2] - vin[i1]) * scale;
            ++numOut;

            // outside-inside
            if (vd[i1] > T(0) && vd[i2] < T(0)) {
                vout[numOut] = vin[i2];
                ++numOut;
            }
        }
        // do nothing for outside-outside
    }

    // clean up
    delete[] vd;
}

template<typename T>
void ky_plane<T>::createCubeVertices(const ky_vector3<T>& llf, const ky_vector3<T>& urb, ky_vector3<T> cubeVertices[8]) {
    cubeVertices[0] = ky_vector3<T>(llf.x, llf.y, urb.z);// llb 0
    cubeVertices[1] = ky_vector3<T>(urb.x, llf.y, urb.z);// lrb 1
    cubeVertices[2] = ky_vector3<T>(urb.x, urb.y, urb.z);// urb 2
    cubeVertices[3] = ky_vector3<T>(llf.x, urb.y, urb.z);// ulb 3

    cubeVertices[4] = ky_vector3<T>(llf.x, llf.y, llf.z);// llf 4
    cubeVertices[5] = ky_vector3<T>(urb.x, llf.y, llf.z);// lrf 5
    cubeVertices[6] = ky_vector3<T>(urb.x, urb.y, llf.z);// urf 6
    cubeVertices[7] = ky_vector3<T>(llf.x, urb.y, llf.z);// ulf 7
}

template<typename T>
void ky_plane<T>::createCubeky_planes(const ky_vector3<T> cv[8], ky_plane<T> cubeky_planes[6]) {
    cubeky_planes[0] = ky_plane<T>( cv[2], cv[1], cv[0] );// front
    cubeky_planes[1] = ky_plane<T>( cv[0], cv[3], cv[7] );// left
    cubeky_planes[2] = ky_plane<T>( cv[4], cv[5], cv[6] );// back
    cubeky_planes[3] = ky_plane<T>( cv[6], cv[2], cv[1] );// right
    cubeky_planes[4] = ky_plane<T>( cv[5], cv[1], cv[0] );// bottom
    cubeky_planes[5] = ky_plane<T>( cv[7], cv[3], cv[2] );// top
}

template<typename T>
void ky_plane<T>::createQuad(ky_vector3<T> llf, ky_vector3<T> urb, ky_vector3<T>* v) const {
// copy of llf and urb is needed here
    T param;
    ky_line3<T> l;

    // scale to ensure that a polygon is created that is huge enough
    urb.x += T(5);
    urb.y += T(5);
    urb.z -= T(5);
    llf.x -= T(5);
    llf.y -= T(5);
    llf.z += T(5);

    // check whether the ky_plane's normal lies in the x-y ky_plane
    if (abs(n).z < T(0.000001)) {
        ky_vector3<T> llb(llf.x, llf.y, urb.z);
        ky_vector3<T> urf(urb.x, urb.y, llf.z);

        // each found vector must be scaled hence a overlapping polygon is guaranteed
        l = ky_line3<T>(urb, urb + ky_vector3<T>(T(1), T(0), T(0)));
        intersect(l, param);
        v[0] = l.interpolates (param);

        l = ky_line3<T>(urf, urf + ky_vector3<T>(T(1), T(0), T(0)));
        intersect(l, param);
        v[1] = l.interpolates(param);

        l = ky_line3<T>(llf, llf + ky_vector3<T>(T(1), T(0), T(0)));
        intersect(l, param);
        v[2] = l.interpolates(param);

        l = ky_line3<T>(llb, llb + ky_vector3<T>(T(1), T(0), T(0)));
        intersect(l, param);
        v[3] = l.interpolates(param);
    }
    else {
        ky_vector3<T> lrf(urb.x, llf.y, llf.z);
        ky_vector3<T> ulb(llf.x, urb.y, urb.z);

        // each found vector must be scaled hence a overlapping polygon is guaranteed
        l = ky_line3<T>(ulb, ulb + ky_vector3<T>(T(0), T(0), T(1)));
        intersect(l, param);
        v[0] = l.interpolates(param);

        l = ky_line3<T>(urb, urb + ky_vector3<T>(T(0), T(0), T(1)));
        intersect(l, param);
        v[1] = l.interpolates(param);

        l = ky_line3<T>(lrf, lrf + ky_vector3<T>(T(0), T(0), T(1)));
        intersect(l, param);
        v[2] = l.interpolates(param);

        l = ky_line3<T>(llf, llf + ky_vector3<T>(T(0), T(0), T(1)));
        intersect(l, param);
        v[3] = l.interpolates(param);
    }
}

template<typename T>
void ky_plane<T>::clipAAB(ky_plane<T> aabky_planes[6], ky_vector3<T>* vin, size_t numIn, ky_vector3<T>* vout, size_t& numOut) {
    for (size_t i = 0; i < 6; ++i) {
        aabky_planes[i].clipPolygon(vin, numIn, vout, numOut);

        if (i == 5)
            return;

        // swap
        ky_vector3<T>* tmp = vin;
        vin = vout;
        vout = tmp;
        numIn = numOut;
    }
}

template<typename T>
void ky_plane<T>::clipAAB(ky_plane<T> aabky_planes[6], ky_vector<ky_vector3<T> > &vin, ky_vector<ky_vector3<T> > &vout) {
    for (size_t i = 0; i < 6; ++i) {
        aabky_planes[i].clipPolygon(vin, vout);

        if (i == 5)
            return;

        ky_swap(vin, vout);
        vout.clear();
    }
}

template<typename T>
void ky_plane<T>::clipAAB(const ky_vector3<T>& llf, const ky_vector3<T>& urb, ky_vector<ky_vector3<T> > &vout) const {
    // 1. compute AAB ky_planes
    ky_plane<T> aabky_planes[6];
    createAABky_planes(llf, urb, aabky_planes);

    // 2. calculate a polygon which is huge enough
    ky_vector< ky_vector3<T> > vin(4);
    createQuad(llf, urb, &vin[0]);

    // 3. clip against all 6 ky_planes of the AAB
    clipAAB(aabky_planes, vin, vout);
}

/// ostream-operator
template <typename T>
std::ostream& operator << (std::ostream& s, const ky_plane<T>& p) {
    return (s << p.n << " * x + " << p.d << " = 0");
}

// typedefs for easy usage
// defined types with a small 'p' like other absolute math typenamees in tgt
typedef ky_plane<int>      iplane;
typedef ky_plane<float>    fplane;
typedef ky_plane<real>     dplane;


template<typename T>
ky_streamb &operator << (ky_streamb &in, const ky_plane<T> &v)
{
    in << v.d;
    in << v.n;
    return in;
}

template<typename T>
ky_streamb &operator >> (ky_streamb &out, ky_plane<T> &v)
{
    out >> v.d;
    out >> v.n;
    return out;
}
template<typename T>
ky_streamt &operator << (ky_streamt &in, const ky_plane<T> &v)
{
    in << v.d;
    in << v.n;
    return in;
}

template<typename T>
ky_streamt &operator >> (ky_streamt &out, ky_plane<T> &v)
{
    out >> v.d;
    out >> v.n;
    return out;
}
#endif // ky_plane_H
