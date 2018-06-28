/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_map.h
 * @brief    模板数组类(支持C++11)
 *       ky_array 类主要完成一个连续性地址的模板数组管理，内部使用ky_alloc来完成内存分配，
 * 有ky_ref完成对类的引用管理，具备共享后修改时会进行单独复制使用。
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.2.3
 * @date     2010/05/02
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2010/05/02 | 1.0.0.1   | kunyang  | 创建文件
 * 2011/02/12 | 1.0.1.0   | kunyang  | 加入ky_alloc模板分配类
 * 2014/08/10 | 1.0.2.0   | kunyang  | 加入对象引用计数，同时加入C++11支持
 * 2015/03/06 | 1.0.2.2   | kunyang  | 修改将头信息和实际类数据分开
 * 2016/06/29 | 1.0.2.3   | kunyang  | 修改引用计数的可复制对象
 */
#ifndef ky_MAP
#define ky_MAP

#include "ky_define.h"
#include "ky_list.h"
#include "ky_mapdata.h"
#include <map>

template <typename K, typename V, typename Alloc = ky_mapalloc >
class ky_map : Alloc
{
public:
    class opemap_node;
    class const_opemap_node;
    typedef opemap_node iterator;
    typedef const_opemap_node const_iterator;

    /// STL style
public:
    ky_map();
    ky_map(const ky_map<K, V, Alloc> &m);
    ~ky_map();

    //!
    //! \brief begin 返回开始迭代和结束迭代
    //! \return
    //!
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const ;
#if kyLanguage < kyLanguage11
    //!
    //! \brief erase 擦除指定pos的元素
    //! \param pos
    //!
    void erase(iterator pos);

    //!
    //! \brief erase 擦除从first开始到last之间的元素
    //! \param first
    //! \param last
    //!
    void erase(iterator first, iterator last);
    //!
    //! \brief find 查找key并返回一个迭代器
    //! \param key
    //! \return
    //!
    const_iterator find(const K &key)const;
    //!
    //! \brief insert 插入key，val
    //! \param key
    //! \param val
    //!
    void insert(const K& key, const V& val);
#endif
    //!
    //! \brief operator = 默认赋值
    //! \param rhs
    //! \return
    //!
    ky_map<K, V, Alloc> &operator = (const ky_map<K, V, Alloc> &rhs);

    //!
    //! \brief at 返回key的值
    //! \param key
    //! \return
    //!
    V &at(const K &key);
    const V &at(const K &key)const;
    V &operator [](const K &key);

    //!
    //! \brief insert 插入key并返回迭代器
    //! \param key
    //! \return
    //!
    iterator insert(const K& key);
    //!
    //! \brief insert 根据pos位置插入key并返回迭代器
    //! \param pos
    //! \param key
    //! \return
    //!
    iterator insert(iterator pos, const K& key);
    //!
    //! \brief insert 插入key，val并返回迭代器
    //! \param key
    //! \param val
    //! \return
    //!
    iterator insert(const K& key, const V& val);

    //!
    //! \brief swap 交换两个map
    //! \param rhs
    //!
    void swap(ky_map<K, V, Alloc> &rhs);
    friend void ky_swap(ky_map<K, V, Alloc> &a, ky_map<K, V, Alloc> &b) {a.swap(b);}
    //!
    //! \brief clear 清空map
    //!
    void clear();
    //!
    //! \brief empty 是否为空map
    //! \return
    //!
    bool empty() const;
    //!
    //! \brief size map的元素数
    //! \return
    //!
    size_t size() const;
    //!
    //! \brief find 查找第一个key出现的迭代器
    //! \param key
    //! \return
    //!
    iterator find(const K &key);

    /// C++ 11
    #if kyLanguage >= kyLanguage11
public:
    ky_map(ky_map<K, V, Alloc> &&m);
    ky_map(std::initializer_list<std::pair<K,V> > list);
    //ky_map<K, V, Alloc>& operator = (ky_map<K, V, Alloc> &&m) ;
    const_iterator cbegin() const;
    const_iterator cend() const;

    V &operator []( K &&key);
    iterator insert(const_iterator pos, const V& val);
    iterator erase(const_iterator pos);
    iterator erase(iterator pos);
    size_t erase(const K& key);
    iterator erase(const_iterator first, const_iterator last);

    const_iterator find(const K &key) const;
    #endif

public :
    explicit ky_map(const std::map<K, V> &m);

    //!
    //! \brief to_std
    //! \return 和stl模板进行转换
    //!
    std::map<K, V> &to_std();
    void form(std::map<K, V> &m);

    //!
    //! \brief operator []
    //! \param key
    //! \return 返回指定key的值
    //!
    const V operator [](const K &key)const;
    //!
    //! \brief operator ==
    //! \param rhs
    //! \return 返回两个map是否相等
    //!
    bool operator==(const ky_map<K, V, Alloc> &rhs) const;
    //!
    //! \brief operator !=
    //! \param rhs
    //! \return 返回两个map是否不相等
    //!
    bool operator!=(const ky_map<K, V, Alloc> &rhs) const { return !(*this == rhs); }

    //!
    //! \brief value
    //! \param key
    //! \return 返回指定key的值
    //!
    V value(const K& key);

    //!
    //! \brief append 添加一个关联
    //! \param key
    //! \param val
    //!
    void append(const K& key, const V& val);
    //!
    //! \brief remove 删除指定key
    //! \param key
    //!
    void remove(const K& key);
    //!
    //! \brief remove 删除全部关联
    //!
    void remove();

#if kyLanguage >= kyLanguage11
    //!
    //! \brief insert 根据pos位置插入
    //! \param pos
    //! \param key
    //! \param value
    //!
    void insert(const_iterator pos, const K &key, const V &value);
#endif

    //!
    //! \brief contains
    //! \param key
    //! \return 是否包括key
    //!
    bool contains(const K &key) const;
    //!
    //! \brief key
    //! \param value
    //! \return 返回指定值的第一个key
    //!
    const K key(const V &value) const;
    //!
    //! \brief value
    //! \param key
    //! \return 返回指定key的值
    //!
    const V value(const K &key) const;
    //!
    //! \brief keys
    //! \return 返回map的全部或指定值的key
    //!
    ky_list<K> keys() const;
    ky_list<K> keys(const V &value) const;
    //!
    //! \brief values
    //! \return 返回map的全部或指定key的值
    //!
    ky_list<V> values() const;
    ky_list<V> values(const K &key) const;

    //!
    //! \brief first
    //! \return 返回第一个的值
    //!
    V &first() ;
    const V &first()const;
    //!
    //! \brief last
    //! \return 返回最后一个的值
    //!
    V &last();
    const V &last()const;

    //!
    //! \brief count
    //! \return 返回关联数
    //!
    size_t count()const;
    //!
    //! \brief is_empty
    //! \return 返回是否为空
    //!
    bool is_empty()const ;
    bool is_null()const ;

public:
    class opemap_node
    {
        friend class const_opemap_node;
    private:
        ky_nodemap<K, V> *ope;
    public:
        opemap_node():ope(NULL){}
        opemap_node(ky_nodemap<K, V>  *p): ope(p){}
        opemap_node(const opemap_node &ope_n): ope(ope_n.ope){}
        ~opemap_node(){}

        const K &key() const { return ope->key; }
        V &value() const { return ope->value; }
        V &operator*() const { return ope->value; }
        V *operator->() const { return &ope->value; }
        bool operator==(const opemap_node &o) const { return ope == o.ope; }
        bool operator!=(const opemap_node &o) const { return ope != o.ope; }
        bool operator<(const opemap_node& rhs) const { return ope < rhs.ope; }
        bool operator<=(const opemap_node& rhs) const { return ope <= rhs.ope; }
        bool operator>(const opemap_node& rhs) const { return ope > rhs.ope; }
        bool operator>=(const opemap_node& rhs) const { return ope >= rhs.ope; }

        int operator -(const opemap_node &rhs)const
        {
            ky_map<K, V, Alloc>::opemap_node *take = (ky_map<K, V, Alloc>::opemap_node*)this;
            int retval = 0;
            ky_nodemap<K, V> * nt = ope;
            forever (ope != NULL && ope != rhs.ope)
            {
                this->operator --();
                ++retval;
            }
            if (ope == NULL)
            {
                take->ope = nt;
                retval = 0;
                forever (ope != NULL && ope != rhs.ope)
                {
                    this->operator ++();
                    --retval;
                }
            }
            take->ope = nt;
            return retval;
        }

        opemap_node operator +(int c)
        {
            opemap_node node = *this;
            if (c > 0)
                while (c--) ++node;
            else
                while (c++) --node;
            return (node);
        }
        const opemap_node operator +(int c)const
        {
            opemap_node node = *this;
            if (c > 0)
                while (c--) ++node;
            else
                while (c++) --node;
            return (node);
        }
        opemap_node operator -(int c) {return operator+(-c);}
        const opemap_node operator -(int c)const{ return operator+(-c);}

        const opemap_node& operator++()const{ope = ope->next(); return *this;}
        const opemap_node operator++(int)const{opemap_node tn = *this; ope = ope->next(); return tn;}

        const opemap_node& operator--()const{ope = ope->prev(); return *this;}
        const opemap_node operator--(int)const{opemap_node tn = *this; ope = ope->prev(); return tn;}

        opemap_node& operator++() {ope = ope->next(); return *this;}
        opemap_node operator++(int){opemap_node tn = *this; ope = ope->next(); return tn;}

        opemap_node& operator--(){ope = ope->prev(); return *this;}
        opemap_node operator--(int){opemap_node tn = *this; ope = ope->prev(); return tn;}

        opemap_node &operator +=(int c){return *this = *this + c; }
        const opemap_node &operator +=(int c)const{return *this = *this + c;}
        opemap_node &operator -=(int c){return *this = *this - c;}
        const opemap_node &operator -=(int c)const{return *this = *this - c;}
    };
    class const_opemap_node
    {
        friend class opemap_node;
    private:
        ky_nodemap<K, V> *ope;
    public:
        const_opemap_node():ope(NULL){}
        const_opemap_node(const ky_nodemap<K, V>  *p): ope((ky_nodemap<K, V>  *)p){}
        const_opemap_node(const opemap_node &ope_n): ope(ope_n.ope){}
        ~const_opemap_node(){}

        const K &key() const { return ope->key; }
        const V &value() const { return ope->value; }
        const V &operator*() const { return ope->value; }
        const V *operator->() const { return &ope->value; }
        bool operator==(const const_opemap_node &o) const { return ope == o.ope; }
        bool operator!=(const const_opemap_node &o) const { return ope != o.ope; }
        bool operator<(const const_opemap_node& rhs) const { return ope < rhs.ope; }
        bool operator<=(const const_opemap_node& rhs) const { return ope <= rhs.ope; }
        bool operator>(const const_opemap_node& rhs) const { return ope > rhs.ope; }
        bool operator>=(const const_opemap_node& rhs) const { return ope >= rhs.ope; }

        int operator -(const const_opemap_node &rhs)const
        {
            ky_map<K, V, Alloc>::const_opemap_node *take = (ky_map<K, V, Alloc>::const_opemap_node*)this;
            int retval = 0;
            ky_nodemap<K, V> * nt = ope;
            forever (ope != NULL && ope != rhs.ope)
            {
                this->operator --();
                ++retval;
            }
            if (ope == NULL)
            {
                take->ope = nt;
                retval = 0;
                forever (ope != NULL && ope != rhs.ope)
                {
                    this->operator ++();
                    --retval;
                }
            }
            take->ope = nt;
            return retval;
        }

        const_opemap_node operator +(int c)
        {
            const_opemap_node node = *this;
            if (c > 0)
                while (c--) ++node;
            else
                while (c++) --node;
            return (node);
        }
        const const_opemap_node operator +(int c)const
        {
            const_opemap_node node = *this;
            if (c > 0)
                while (c--) ++node;
            else
                while (c++) --node;
            return (node);
        }
        const_opemap_node operator -(int c) {return operator+(-c);}
        const const_opemap_node operator -(int c)const{ return operator+(-c);}

        const const_opemap_node& operator++()const{ope = ope->next(); return *this;}
        const const_opemap_node operator++(int)const{const_opemap_node tn = *this; ope = ope->next(); return tn;}

        const const_opemap_node& operator--()const{ope = ope->prev(); return *this;}
        const const_opemap_node operator--(int)const{const_opemap_node tn = *this; ope = ope->prev(); return tn;}

        const_opemap_node& operator++() {ope = ope->next(); return *this;}
        const_opemap_node operator++(int){const_opemap_node tn = *this; ope = ope->next(); return tn;}

        const_opemap_node& operator--(){ope = ope->prev(); return *this;}
        const_opemap_node operator--(int){const_opemap_node tn = *this; ope = ope->prev(); return tn;}

        const_opemap_node &operator +=(int c){return *this = *this + c; }
        const const_opemap_node &operator +=(int c)const{return *this = *this + c;}
        const_opemap_node &operator -=(int c){return *this = *this - c;}
        const const_opemap_node &operator -=(int c)const{return *this = *this - c;}
    };
    friend class opemap_node;
    friend class const_opemap_node;

protected:
    void detach();
private:
    void copy();
private:
    ky_mapdata<K, V, Alloc> *impl;
};

#include "ky_map.inl"
/*template <typename K, typename V, typename Alloc>
ky_map<K, V, Alloc>& ky_map<K, V, Alloc>::operator = (ky_map<K, V, Alloc> &&m)
{
    if (!this->is_null () && impl->lessref())
    {
        impl->destroy();
        impl = (ky_mapdata<K, V, Alloc> *)ky_mapdata<K, V, Alloc>::null ();
    }
    if (m.is_null ())
        return *this;
    impl = m.impl;
    m.impl = (ky_mapdata<K, V, Alloc> *)ky_mapdata<K, V, Alloc>::null ();
    return *this;
}*/
#endif // ky_MAP

