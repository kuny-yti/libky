
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_object.h
 * @brief    支持托管及信号槽模式的连接继承对象，并管理子父对象
 *       1.支持类的RTTI.
 *       2.可完成信号槽模式的对象间调用机制
 *       3.支持子父对象的管理及遍历
 *       4.支持对象间的引用或指针间的引用机制
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.3.2.1
 * @date     2011/10/11
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2011/10/11 | 1.0.0.1   | kunyang  | 创建文件实现空对象类
 * 2011/10/20 | 1.0.1.0   | kunyang  | 完善对象类实现
 * 2012/03/13 | 1.1.0.1   | kunyang  | 添加类的RTTI机制
 * 2012/07/27 | 1.1.1.1   | kunyang  | 修改对象类的子父对象管理
 * 2014/05/08 | 1.2.0.1   | kunyang  | 添加指针间引用计数机制
 * 2015/02/28 | 1.2.1.0   | kunyang  | 修改指针间引用机制为原子操作模式
 * 2016/03/08 | 1.3.0.0   | kunyang  | 实现信号槽调用机制
 * 2016/05/14 | 1.3.1.0   | kunyang  | 修改引用计数机制并兼容对象类
 * 2017/04/12 | 1.3.2.0   | kunyang  | 修改字符对象管理机制并重构信号槽机制
 * 2018/03/26 | 1.3.2.1   | kunyang  | 将模板对象是否为继承ky_object进行枚举定义
 */
#ifndef KY_OBJECT_H
#define KY_OBJECT_H

#include "tools/ky_typeinfo.h"
#include "tools/ky_string.h"
#include "tools/ky_list.h"
#include "tools/ky_signal.inl"
#include "ky_define.h"
#include "ky_thread.h"

namespace impl {
class object;
}

/// 当前此类不是线程安全的
///
/// 1.类的RTTI
///   a.可继承后用name可以查看类型继承关系
///   b.继承is可判断是否兼容类型
/// 2.关联类的调用
///   a.connect可以将被本类调用的函数链接到被调用池
///   b.disconnect断开连接
///   c.emit执行调用池内的对象
///   d.被调用者需要继承callback
/// 3.子父对象的结构
///   用于关系操作
/// 4.对象自动管理
///
//! 类的RTTI
#define kyObject(type) public:\
    virtual ky_string name() const{return ky_typeinfo<type>();} \
    virtual bool is(ky_object *rhs)const \
    {\
        type *tmps = kyAsCast(type *)(rhs);\
        if (tmps != NULL)\
            return true;\
        return false; \
    }

class ky_object : public sigslot
{
    kyObject(ky_object)
    template<typename PtrT>
    friend class ky_ptrref;
protected:
    void lessref();
    void addref();

public:
    explicit ky_object(ky_object *parent = NULL);

    template <typename T>
    T* as() {return kyAsCast(T *)(this);}

    ky_thread *thread() const;

public:
    typedef ky_list <ky_object*>::iterator child;

    uint child_count()const;
    ky_object *parent()const;
    void parent(const ky_object *p);

protected:
    virtual ~ky_object(){}

protected:
    //!
    //! \brief append 添加孩子
    //! \param obj
    //!
    void append(ky_object *obj);
    //!
    //! \brief remove 删除孩子
    //! \param it
    //!
    void remove(const child &it);
    void remove(ky_object *obj);
    //!
    //! \brief begin 孩子的开始节点
    //! \return
    //!
    child begin();
    const child begin()const;
    //!
    //! \brief end 孩子的结束节点
    //! \return
    //!
    child end();
    const child end()const;

private:
    impl::object *impl;
};



template<typename PtrT>
class ky_ptrref
{
public:
    ky_ptrref():ptr(0), ref(0){}
    ky_ptrref (PtrT *p, ky_ref::eRefFlags c = ky_ref::refShareable)
    {
        if (typeid(PtrT) != typeid(ky_object))
            ref = kyNew (ky_ref(c));
        else
            ref = 0;
        ptr = (p);
    }

    ky_ptrref(const ky_ptrref<PtrT> &rhs):
        ref(rhs.ref)
    {
        ptr = (rhs.ptr);
        addref();
    }

#if kyLanguage >= kyLanguage11
    ky_ptrref(ky_ptrref<PtrT> &&rhs) :
        ref(rhs.ref)
    {
        ptr = (rhs.ptr);
        rhs.ptr = 0;
        rhs.ref = 0;
    }
    inline ky_ptrref<PtrT>& operator =(ky_ptrref<PtrT>&& rhs)
    {
        if (*this != rhs)
        {
            lessref();
            ptr = (rhs.ptr);
            ref = rhs.ref;
            rhs.ptr = NULL;
            rhs.ref = NULL;
        }
        return *this;
    }
#endif

    //! 其他类到本类转换
    template <class PtrU>
    ky_ptrref(const ky_ptrref <PtrU> &rhs)
    {
        ptr = kyAsCast(PtrT *)(rhs.ptr);
        if (ptr)
        {
            ref = rhs.ref;
            addref();
        }
        else
            ref = 0;
    }

    //! 作为其他类使用
    template <class PtrU>
    inline ky_ptrref <PtrU> as()const
    {
        return ky_ptrref <PtrU>(*this);
    }
    template <class PtrU>
    inline ky_ptrref <PtrU> as()
    {
        return ky_ptrref <PtrU>(*this);
    }

    virtual ~ky_ptrref() {lessref();}
    //! 其他类到本类转换
    template <class PtrU>
    inline ky_ptrref<PtrT>& operator =(const ky_ptrref<PtrU>& rhs)
    {
        lessref();
        ptr = (kyAsCast(PtrT *)(rhs.ptr));
        if (ptr)
        {
            ref = rhs.ref;
            addref();
        }
        else
            ref = 0;
        return *this;
    }

    inline ky_ptrref<PtrT>& operator =(const ky_ptrref<PtrT>& rhs)
    {
        if (ptr != rhs.ptr && ref != rhs.ref)
        {
            lessref();
            ptr = (rhs.ptr);
            ref = rhs.ref;
            addref();
        }
        return *this;
    }

    inline ky_ptrref<PtrT>& operator = (PtrT *otp)
    {
        if (otp != ptr)
        {
            lessref();
            if (typeid(PtrT) != typeid(ky_object))
                ref = kyNew (ky_ref());
            else
                ref = 0;
            ptr = (otp);
        }
        return *this;
    }

    inline bool operator ==(const ky_ptrref<PtrT> &rhs)const{return ptr ==rhs.ptr && ref ==rhs.ref;}
    inline bool operator !=(const ky_ptrref<PtrT> &rhs)const{ return !(this->operator ==(rhs));}
    inline bool operator ==(const PtrT* rhs) const { return ptr == rhs; }
    inline bool operator !=(const PtrT* rhs) const { return !(this->operator ==(rhs)); }

    inline PtrT* operator->() { return ptr; }
    inline PtrT* operator->() const { return ptr; }
    inline PtrT* get() { return ptr; }
    inline const PtrT* get() const { return ptr; }
    inline PtrT& operator*() { return *ptr; }
    inline const PtrT& operator*() const { return *ptr; }
    inline operator PtrT* () { return ptr; }
    inline operator bool() const { return ptr != 0; }

    inline void reset(){ lessref(); ptr = 0;ref = 0;}

public:
    PtrT*    ptr;
    ky_ref * ref;

    inline void lessref()
    {
        ky_object *toj = kyAsCast(ky_object*)(ptr);
        if (typeid(PtrT) == typeid(ky_object))
            toj->lessref();
        else if (ref && ref->lessref())
        {
            PtrT* t = ptr;
            ptr = (0);
            kyDelete (t);
            kyDelete (ref);
            ptr = 0;
            ref = 0;
        }
    }
    inline void addref()
    {
        ky_object *toj = kyAsCast(ky_object*)(ptr);
        if (typeid(PtrT) == typeid(ky_object))
            toj->addref();
        else if (ref != 0)
            ref->addref();
    }
};
#endif
