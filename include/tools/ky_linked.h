/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_linked.h
 * @brief    模板链式链表数据空间容器类(支持C++11)
 *       ky_linked 类主要完成一个链式链表型模板类，内部使用ky_alloc来完成内存分配，
 * 有ky_ref完成对类的引用管理，具备共享后修改会进行单独复制使用。
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.0.1
 * @date     2018/03/09
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2018/03/09 | 1.0.0.1   | kunyang  | 创建文件
 */

#ifndef KY_LINKED_H
#define KY_LINKED_H

#include "../ky_define.h"
#include "ky_algorlthm.h"
#include "ky_memory.h"
#include <list>

struct _ky_linked_
{
    _ky_linked_  *prev;
    _ky_linked_  *next;
    ky_ref ref;
    size_t count;

    static const _ky_linked_ shared_null;
    static bool is_null (intptr p);
    static intptr null();

    void addref(){ref.addref();}
    bool lessref(){return ref.lessref();}
};

template <typename T, typename Alloc = ky_alloc<T> >
class ky_linked : public Alloc
{
public:
    class opelist_node;
    typedef opelist_node  iterator;
    typedef const iterator    const_iterator;

    // STL
public:
    ky_linked();
    ky_linked(const ky_linked<T, Alloc> &rhs);
    ~ky_linked();

    //!
    //! \brief operator = 链表赋值
    //! \param rhs
    //! \return
    //!
    ky_linked& operator = (const ky_linked<T, Alloc> &rhs);

    bool empty() const {return is_empty();}
    size_t size() const {return count();}
    //!
    //! \brief swap 交换链表
    //! \param rhs
    //!
    void swap(ky_linked<T, Alloc> &rhs);
    friend void ky_swap(ky_linked<T, Alloc>  &a, ky_linked<T, Alloc>  &b){a.swap(b);}
    //!
    //! \brief sort 排序链表-未实现
    //!
    void sort();
    //!
    //! \brief reverse 逆向链表-未实现
    //!
    void reverse();

    //!
    //! \brief clear 清空链表
    //!
    void clear();

    //!
    //! \brief begin 开始节点
    //! \return
    //!
    iterator begin();
    const_iterator begin() const;
    inline const_iterator cbegin() const{return begin();}

    //!
    //! \brief end 结束节点
    //! \return
    //!
    iterator end();
    const_iterator end() const;
    inline const_iterator cend() const{return end();}

    //!
    //! \brief front 头部元素
    //! \return
    //!
    T& front();
    const T& front() const;

    //!
    //! \brief back 尾部元素
    //! \return
    //!
    T& back();
    const T& back() const;

    //!
    //! \brief push_front 在头和尾添加一个元素
    //! \param x
    //!
    void push_front( const T &x ){prepend(x);}
    void push_back( const T &x ){append(x);}

    //!
    //! \brief pop_front 在头和尾删掉一个元素
    //!
    void pop_front();
    void pop_back();

#if kyLanguage < kyLanguage11
    void insert(iterator itr, size_t n, const T &x );
    void splice(iterator pos, ky_linked& x);
    void splice(iterator pos, ky_linked& x, iterator i);
    void splice(iterator pos, ky_linked& x, iterator first, iterator last);
    void merge(ky_linked& x);
    //!
    //! \brief insert 插入操作
    //! \param itr
    //! \param x
    //! \return
    //!
    iterator insert(iterator itr, const ky_linked<T, Alloc> &x );
    iterator insert(iterator itr, const T &x );
    iterator insert(iterator itr, const iterator &x );

    //!
    //! \brief erase 擦除操作
    //! \param itr
    //! \return
    //!
    iterator erase(iterator itr );
    iterator erase(iterator start, iterator end );
#endif
    //!
    //! \brief assign 赋值操作
    //! \param itr
    //! \param x
    //!
    void assign(iterator itr, const T &x );
    void assign(iterator itr, const iterator &x );
    void assign(size_t, const T &x );

    //void remove(const T& val)
    //void unique()

    // C++11
#if kyLanguage >= kyLanguage11
public:
    explicit ky_linked(const ky_linked<T, Alloc> &&rhs);
    ky_linked(const std::initializer_list<T> & il);
    ky_linked<T, Alloc>& operator = (ky_linked<T, Alloc> &&rhs);

    void push_front( T &&x ){prepend(x);}
    void push_back( T &&x ){append(x);}

    void splice(const_iterator pos, ky_linked<T, Alloc>&& x);
    void splice(const_iterator pos, ky_linked<T, Alloc>& x);
    void splice(const_iterator pos, ky_linked<T, Alloc>&& x, const_iterator i);
    void splice(const_iterator pos, ky_linked<T, Alloc>& x, const_iterator i);
    void splice(const_iterator pos, ky_linked<T, Alloc>&& x, const_iterator first, const_iterator last);
    void splice(const_iterator pos, ky_linked<T, Alloc>& x, const_iterator first, const_iterator last);
    void merge(ky_linked<T, Alloc>&& x);
    //!
    //! \brief insert 插入操作
    //! \param itr
    //! \param x
    //! \return
    //!
    iterator insert(const_iterator itr, const ky_linked<T, Alloc> &x );
    iterator insert(const_iterator itr, const T &x );
    iterator insert(const_iterator itr, const iterator &x );
    iterator insert(const_iterator itr, T &&x );
    iterator insert(const_iterator itr, size_t n, const T &x );

    //!
    //! \brief erase 擦除操作
    //! \param itr
    //! \return
    //!
    iterator erase(const_iterator itr );
    iterator erase(const_iterator start, const_iterator end );
#endif

    // base
public:
    explicit ky_linked(const std::list<T> &rhs);
    //!
    //! \brief to_std
    //! \return 返回stl型链表
    //!
    std::list<T> to_std();
    //!
    //! \brief form stl型链表输入
    //! \param rhs
    //!
    void form(const std::list<T> &rhs);
    ky_linked<T, Alloc>& operator = (const std::list<T> &rhs);

    //!
    //! \brief operator += 链表追加
    //! \param rhs
    //! \return
    //!
    ky_linked<T, Alloc> &operator +=(const ky_linked<T, Alloc> &rhs);
    ky_linked<T, Alloc> &operator +=(const T &val);

    //!
    //! \brief operator + 链表相加
    //! \param rhs
    //! \return
    //!
    ky_linked<T, Alloc> operator +(const ky_linked<T, Alloc> &rhs) const;

    //!
    //! \brief operator << 链表添加
    //! \param val
    //! \return
    //!
    ky_linked<T, Alloc> &operator << (const T &val);
    ky_linked<T, Alloc> &operator << (const ky_linked<T, Alloc> &rhs);

    //!
    //! \brief operator 链表比对
    //! \param rhs
    //! \return
    //!
    bool operator ==(const ky_linked<T, Alloc> &rhs) const;
    bool operator !=(const ky_linked<T, Alloc> &rhs) const { return !(*this == rhs); }

    //!
    //! \brief is_empty 是否空链表
    //! \return
    //!
    bool is_empty() const;
    bool is_null() const;
    //!
    //! \brief count 链表长度
    //! \return
    //!
    size_t count() const;
    //!
    //! \brief prepend 前置添加元素
    //! \param val
    //!
    void prepend(const T& val);
    void prepend(const ky_linked<T, Alloc>& val) ;
    //!
    //! \brief append 后置添加元素
    //! \param val
    //!
    void append(const T& val) ;
    void append(const ky_linked& val);
    //!
    //! \brief insert 根据位置插入元素
    //! \param pos
    //! \param val
    //!
    iterator insert(size_t pos, const T& val) ;
    void insert(size_t pos, const ky_linked<T, Alloc>& val);

    //!
    //! \brief replace 替换操作
    //! \param i
    //! \param t
    //!
    void replace(size_t i, const T &t);
    //!
    //! \brief move 移动操作
    //! \param inx
    //! \param to
    //!
    void move(size_t inx, size_t to);

    //!
    //! \brief remove 根据位置删除元素
    //! \param pos
    //!
    void remove(size_t pos) ;
    void remove() {pop_back();}
    //!
    //! \brief swap 交换操作
    //! \param i
    //! \param j
    //!
    void swap(size_t i, size_t j);
    void swap(iterator x, iterator y);
    //!
    //! \brief contains 查找是否包含指定元素
    //! \param t
    //! \return
    //!
    bool contains(const T &t) const;
    int find(const T &t) const;

protected:
    struct list_node
    {
        list_node  *prev;
        list_node  *next;

        T        data;
    };

public:
    class opelist_node
    {
        friend class ky_linked;
    public:

        opelist_node():ope(NULL){}
        opelist_node(list_node *p): ope(p){}
        opelist_node(const opelist_node&ope_n): ope(ope_n.ope){}
        ~opelist_node(){}

        T* operator ->(){return &ope->data;}
        T& operator *() {return ope->data;}
        const T& operator *() const{return ope->data;}

        bool operator ==(const opelist_node &rhs) const { return ope == rhs.ope; }
        bool operator !=(const opelist_node &rhs) const { return ope != rhs.ope; }
        bool operator <(const opelist_node& rhs) const { return ope < rhs.ope; }
        bool operator <=(const opelist_node& rhs) const { return ope <= rhs.ope; }
        bool operator >(const opelist_node& rhs) const { return ope > rhs.ope; }
        bool operator >=(const opelist_node& rhs) const { return ope >= rhs.ope; }

        opelist_node operator +(int c);
        const opelist_node operator +(int c)const;
        inline opelist_node operator -(int c){return operator+(-c);}
        const opelist_node operator -(int c)const{return operator+(-c);}

        int operator -(const opelist_node &rhs)const;

        const opelist_node& operator++()const;
        const opelist_node operator++(int)const;

        const opelist_node& operator--()const;
        const opelist_node operator--(int)const;

        opelist_node& operator++() ;
        opelist_node operator++(int);

        opelist_node& operator--();
        opelist_node operator--(int);

        opelist_node &operator +=(int c){return *this = *this + c; }
        const opelist_node &operator +=(int c)const{return *this = *this + c; }
        opelist_node &operator -=(int c){return *this = *this - c; }
        const opelist_node &operator -=(int c)const{return *this = *this - c; }

    private:
        list_node *ope;
    };

private:
    union {_ky_linked_ *impl; list_node *list;};

private:
    list_node *newnode(const T &);
    void freenode(list_node *n);
    void free(struct _ky_linked_ *n);
    void copy();
    void detach();

    _ky_linked_ *header() {return impl;}
    const _ky_linked_ *header()const {return impl;}
};


#include "ky_linked.inl"
#endif // KY_LINKED_H
