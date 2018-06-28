/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_array.h
 * @brief    模板数组类(支持C++11)
 *       ky_array 类主要完成一个连续性地址的模板数组
 *       1.支持初始化列表初始化
 *       2.内部算法使用ky_memory::array
 *       3.支持引用计数和写时拷贝
 *       4.连续地址内存建议使用此类作为基础进行扩展，此类经过对
 *         插入以及前向添加进行了优化速度比平时连续内存操作较快
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.2.0.1
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
 * 2017/11/09 | 1.1.0.1   | kunyang  | 将内存分配器去掉，并把内存分配并入私有类中，用于实现内存池机制
 * 2018/03/26 | 1.2.0.1   | kunyang  | 将内存操作部分修改为ky_memory::array,实现高效内存操作
 */

#ifndef KY_ARRAY_H
#define KY_ARRAY_H

#include "ky_define.h"
#include "ky_memory.h"
#include "ky_typeinfo.h"

/*!
 * @brief The ky_array class
 * @class ky_array
 * @note 索引使用参考
 *       1.只是访问对象则使用at，更快速
 *       2.需要改变时可采用[]会进行分离，较at慢
 *       3.也可采样data访问指针型,需要自行处理内存越界,较at更快速
 */
template<typename T>
class ky_array : protected ky_memory::array
{
public:
    typedef T Type ;
    typedef ky_memory::array Layout;
    typedef Layout::header_t LayoutHeader;
    enum {not_construct = ky_is_type(T)};
    enum {sizeOf = sizeof(Type)};

    // C++11
#if kyLanguage >= kyLanguage11
public:
    ky_array(ky_array&& rhs);
    ky_array& operator =(ky_array&& rhs);
    ky_array(const std::initializer_list<Type> & il);

#endif

public:
    ky_array();
    //!
    //! \brief ky_array 根据默认储备大小构造函数
    //! \param size 储备空间
    //!
    explicit ky_array(size_t size );
    //!
    //! \brief ky_array 默认引用构造函数
    //! \param ref
    //!
    ky_array(const ky_array &ref);
    //!
    //! \brief ky_array 根据数据构造函数
    //! \param buf
    //! \param len
    //!
    ky_array(const Type *buf, size_t len);
    virtual ~ky_array();

    //!
    //! \brief capacity 返回当前储备空间大小
    //! \return
    //!
    inline size_t capacity() const{return Layout::capacity ();}

    //!
    //! \brief size 返回当前数据的大小
    //! \return
    //!
    inline size_t size()const {return Layout::count ();}
    //!
    //! \brief bytecount 返回当前字节数
    //! \return
    //!
    inline size_t bytecount()const{return size () * sizeOf;}
    //!
    //! \brief is_empty 返回当前数组是否是空数据
    //! \return
    //!
    inline bool is_empty() const{return Layout::is_empty ();}
    inline bool is_null()const{return Layout::is_null ();}

    //!
    //! \brief reserve 分配储备空间
    //! \param size
    //! \note 已经储备则进行内存从新分配
    //!
    void reserve(size_t size);
    //!
    //! \brief resize 申请数据空间
    //! \param s
    //! \note 当内部有数据时并且也被引用此时会将数据分离
    //!
    void resize(size_t s);
    void resize(size_t s, const Type &v);

    //!
    //! \brief shrink 收缩储备空间到合适位置,返回收缩的空间
    //! \return
    //!
    //size_t shrink();

    operator Type *() const;
    operator void *() const;
    operator Type *() ;
    operator void *() ;

    //!
    //! \brief data 返回当前数据指针
    //! \return
    //! \note 当内部有数据时并且也被引用此时会将数据分离
    //!
    Type *data();
    //!
    //! \brief data 返回当前数据指针
    //! \return
    //! \note const 禁止内部执行分离，因为外部只是使用
    //!
    Type * data() const;
    //!
    //! \brief offset 返回inx数据指针
    //! \param inx
    //! \return
    //! \note 当内部有数据时并且也被引用此时会将数据分离
    //!
    Type *offset(int inx);
    //!
    //! \brief offset 返回inx数据指针
    //! \param inx
    //! \return
    //! \note const 禁止内部执行分离，因为外部只是使用
    //!
    Type *offset(int inx)const;
    //!
    //! \brief clear 清除空间
    //! \note 当内部有数据时并且也被引用此时会将数据分离
    //!
    void clear();
    //!
    //! \brief fill 填充空间为c
    //! \param c
    //! \param len
    //! \note 当内部有数据时并且也被引用此时会将数据分离
    //!
    void fill(const Type &c, size_t len, int pos = 0);
    //!
    //! \brief at 返回i的位置数组元素
    //! \param i
    //! \return
    //!
    Type at(int i);
    //!
    //! \brief at 返回i的位置数组元素
    //! \param i
    //! \return
    //!
    const Type at(int i)const;

    //!
    //! \brief operator [] 重载数组访问下标
    //! \param i
    //! \return
    //! \note 当内部有数据时并且也被引用此时会将数据分离
    //!
    Type &operator[](int i);
    //!
    //! \brief operator [] 重载数组访问下标
    //! \param i
    //! \return
    //! \note const 禁止内部执行分离，因为外部只是使用
    //!
    const Type &operator[](int i)const;

    //!
    //! \brief prepend 在最前面添加元素
    //! \param c
    //! \return
    //! \note 当内部有数据时并且也被引用此时会将数据分离
    //!
    ky_array &prepend(const Type &c);
    ky_array &prepend(const Type *s, int len);
    ky_array &prepend(const ky_array &a);
    //!
    //! \brief append 在尾部附加元素
    //! \param c
    //! \return
    //! \note 当内部有数据时并且也被引用此时会将数据分离
    //!
    ky_array &append(const Type &c);
    ky_array &append(const Type *s, size_t len);
    ky_array &append(const ky_array &a);
    //!
    //! \brief insert 在指定i位置插入元素
    //! \param i
    //! \param c
    //! \return
    //! \note 当内部有数据时并且也被引用此时会将数据分离
    //!
    ky_array &insert(int i, const Type &c);
    ky_array &insert(int i, const Type *s, size_t len);
    ky_array &insert(int i, const ky_array &a);
    //!
    //! \brief remove 在指定位置删除len长度的元素
    //! \param index
    //! \param len
    //! \return
    //! \note 当内部有数据时并且也被引用此时会将数据分离
    //!
    void remove(int index, size_t len = 1);

    //!
    //! \brief find 查找 c 第一个出现的位置
    //! \param c
    //! \param i 开始搜索位置
    //! \return
    //!
    int find(const Type &c, int i = 0)const;

    //!
    //! \brief replace 将指定位置的len长度元素替换为alen长度的s数据
    //! \param index
    //! \param len
    //! \param s
    //! \param alen
    //! \return
    //! \note 当内部有数据时并且也被引用此时会将数据分离
    //!
    ky_array &replace(int index, const Type &c);
    ky_array &replace(int index, size_t len, const Type *s, size_t alen);
    ky_array &replace(int index, size_t len, const ky_array &s);

    //!
    //! \brief move 将指定位置的len长度元素移动到to位置
    //! \param from
    //! \param n
    //! \param to
    //! \return
    //! \note 当内部有数据时并且也被引用此时会将数据分离
    //!
    ky_array &move(int from, size_t n, int to);
    ky_array &move(int from, int to);

    //!
    //! \brief operator = 默认赋值函数
    //! \return
    //!
    ky_array &operator =(const ky_array &);

    //!
    //! \brief extract  (count 负数时根据pos位置向前取，此时顺序不为逆)
    //! \param pos
    //! \param count
    //! \return
    //!
    ky_array extract(int pos, int count)const;
    inline ky_array mid(int pos, int count)const
    {return extract(pos, count);}
    //!
    //! \brief extract 根据pos位置将后面的全部提取
    //! \param pos
    //! \return
    //!
    ky_array extract(int pos)const;
    //!
    //! \brief start  从开头提取count个字符
    //! \param count
    //! \return
    //!
    ky_array start(int count)const;
    //!
    //! \brief ending  从结尾提取count个字符，此时不为逆
    //! \param count
    //! \return
    //!
    ky_array ending(int count)const;

    //!
    //! \brief swap  交换两个对象的数据
    //! \param rhs
    //!
    void swap(ky_array &rhs);
    friend void ky_swap(ky_array &a, ky_array &b) {a.swap(b);}

protected:
    void  __detach_helper();
    T *__detach_insert(int i, int c);
    void __destroy_helper();
};

#include "ky_array.inl"

typedef ky_array<uint8> ky_byte;

namespace ky_codec
{
static const char base64Breaks = '\n';
static const char base64Farther = '=';
extern ky_byte base64enc(const ky_byte &s);
extern ky_byte base64dec(const ky_byte &d);
}

#endif // ky_ARRAY_H
