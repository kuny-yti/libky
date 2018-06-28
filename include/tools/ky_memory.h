/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_memory.h
 * @brief    内存的操作定义
 *       1.ky_alloc 简单的内存分配器
 *       2.ky_memory 内存的拷贝及清理的快速实现，可以评估那种加速方式更高效
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.1.1
 * @date     2013/05/01
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2013/05/01 | 1.0.0.1   | kunyang  | 创建文件
 * 2014/01/10 | 1.0.0.2   | kunyang  | 加入快速评估加速指令的选择
 * 2014/02/20 | 1.0.1.0   | kunyang  | 建立ky_alloc
 * 2018/03/10 | 1.0.1.1   | kunyang  | 加入内存增长计算
 *
 */

#ifndef KY_MEMORY_H
#define KY_MEMORY_H

#include "ky_define.h"
#include "ky_utils.h"

typedef enum
{
    Memcpy_Default = 0,
    Memcpy_SSE = 1 << 0,
    Memcpy_MMX = 1 << 2,
    Memcpy_MMX2 = 1 << 3,
    MemCoy_3DNOW = 1 << 4,
    Memcpy_Auto = 1 << 5
}eMemcpyModes;

template<typename T>
struct ky_alloc
{
    static T* alloc(size_t size)
    {
        return (T*)kyMalloc (size);
    }
    static T* realloc(T* mem, size_t size)
    {
        return (T*)::kyRealloc((void*)mem, size);
    }
    static void destroy(void *mem)
    {
        kyFree (mem);
    }
};

template <typename T>
struct ky_allocate;

#define ky_mem() ky_memory::instance ()

//!
//! \brief The ky_memory class
//! \note
//!   1.内部为内存池机构，扩展出对象池机构
//!   2.可快速分配地址和对象
//!   3.自动回收垃圾机制
//!   4.内存分析机制：
//!     a.那些对象经常申请或释放内存
//!     b.每个对象使用的地址范围
//!     c.每个对象使用的内存情况
//!     d.在出现异常时可以快速确定异常的对象和内存操作的位置
//!
class ky_memory /*: public ky_singleton<ky_memory>*/
{
    //friend struct ky_allocate;
    friend class ky_singleton<ky_memory>;
private:
    ky_memory();
    ~ky_memory();

public:
    //!
    //! \brief copy 快速内存拷贝
    //! \param dst
    //! \param src
    //! \param len
    //! \return
    //!
    void *copy(void *dst, const void* src, size_t len);
    //!
    //! \brief move 快速内存移动
    //! \param dst
    //! \param src
    //! \param len
    //! \return
    //!
    void *move(void *dst, const void* src, size_t len);
    //!
    //! \brief compare 内存比较
    //! \param dst
    //! \param src
    //! \param len
    //! \return
    //!
    int compare(const void *dst, const void* src, size_t len);
    //!
    //! \brief zero 内存清零
    //! \param dst
    //! \param len
    //! \param fill
    //!
    void zero(void *dst, size_t len, int fill = 0);
    //!
    //! \brief block_size
    //! \param size
    //! \param align
    //! \param header
    //! \return
    //!
    size_t block_size(size_t size, int align, size_t header);
    //!
    //! \brief block_growing 根据大小和对齐计算内存的增长
    //! \param size   需要分配的尺寸
    //! \param align  内存的字节对齐
    //! \param header 内存的头字节
    //! \param count  返回的申请到的尺寸
    //! \return 返回当前内存大小
    //!
    size_t block_growing(size_t size, int align, size_t header, size_t *count = 0);

    static ky_memory* instance();

    //!
    //! \brief The heap struct
    //! \note Algorithm complexity:
    //!        1.insert   O(n)
    //!        2.append   O(1)-near
    //!        3.prepend  O(1)-near
    //!        4.at       O(1)
    //!
    struct heap
    {
        typedef struct header_t header_t;
        typedef struct {void *value;} node_t;
        enum{NodeSize = sizeof(node_t)};

        header_t *header;

        heap();
        explicit heap(const header_t *);
        virtual ~heap();

        //!
        //! \brief refer 返回引用计数器
        //! \return
        //!
        inline ky_ref &refer(){return *(ky_ref*)header;}
        inline ky_ref &refer()const{return *(ky_ref*)header;}
        inline void addref(){refer ().addref ();}
        inline bool lessref(){return refer ().lessref ();}

        //!
        //! \brief is_null 当前内存块是否为默认
        //! \return
        //!
        bool is_null ();
        bool is_null()const;

        //!
        //! \brief null 返回默认内存块
        //! \return
        //!
        header_t* null();

        //!
        //! \brief reserve 根据size创建内存块
        //! \param size
        //! \return
        //!
        header_t *reserve(size_t size);

        //!
        //! \brief destroy 释放当前内存块
        //!
        void destroy();

        //!
        //! \brief expand 根据growth增长值扩充内存块
        //! \param growth
        //! \return
        //!
        bool expand(size_t growth);

        //!
        //! \brief detach 分离
        //!
        header_t *detach(int *idx, int count);

        //!
        //! \brief append 尾部添加n个元素
        //! \param n
        //! \return
        //!
        node_t *append(int n);

        //!
        //! \brief prepend 头部添加n个元素
        //! \param n
        //! \return
        //! \note 注意返回内存为正序
        //!
        node_t *prepend(int n);

        //!
        //! \brief insert 任意位置插入n个元素
        //! \param i
        //! \param n
        //! \return
        //!
        node_t *insert(int i, int n);

        //!
        //! \brief remove 任意位置删除n个元素
        //! \param i
        //! \param n
        //! \note 注意若为对象时，对象内存需要释放
        //!
        void remove(int i, int n);
        void remove(int i);

        node_t *erase(node_t *xi);

        //!
        //! \brief move 将from位置开始的n个元素移动到to位置处
        //! \param from
        //! \param n
        //! \param to
        //! \note 注意当from+n超过to时则不移动
        //!
        void move(int from, size_t n, int to);

        //!
        //! \brief count 元素数量
        //! \return
        //!
        size_t count() const;
        size_t capacity()const;

        //!
        //! \brief is_empty 内存块中元素为空
        //! \return
        //!
        bool is_empty() const;

        //!
        //! \brief at 取出指定位置的元素
        //! \param i
        //! \return
        //!
        node_t *at(int i);
        node_t *at(int i)const;

        //!
        //! \brief begin 返回元素的开始地址
        //! \return
        //!
        node_t *begin();
        node_t *begin()const;

        //!
        //! \brief end 返回元素的结束地址
        //! \return
        //!
        node_t *end();
        node_t *end()const;

    };

    //!
    //! \brief The array struct
    //! \note Algorithm complexity:
    //!        1.insert   O(n)
    //!        2.append   O(1)-near
    //!        3.prepend  O(1)-near
    //!        4.at       O(1)
    //!
    struct array
    {
        typedef struct header_t header_t;
        header_t * header;

        array();
        explicit array(const header_t *);
        virtual ~array();

        //!
        //! \brief refer 返回引用计数器
        //! \return
        //!
        ky_ref &refer() ;
        void addref();
        bool lessref();

        //!
        //! \brief is_null 当前内存块是否为默认
        //! \return
        //!
        bool is_null ();
        bool is_null ()const;
        header_t * null();

        //!
        //! \brief count 元素数量
        //! \return
        //!
        size_t count() const;
        size_t capacity()const;

        //!
        //! \brief is_empty 内存块中元素为空
        //! \return
        //!
        bool is_empty() const;

        //!
        //! \brief reserve 根据size创建内存块
        //! \param size
        //! \return
        //!
        header_t *reserve(size_t size, int align);
        void resize(size_t size, int align);

        //!
        //! \brief destroy 释放当前内存块
        //!
        void destroy();

        //!
        //! \brief expand 根据growth增长值扩充内存块
        //! \param growth
        //! \return
        //!
        bool expand(size_t growth);

        //!
        //! \brief detach 分离
        //!
        header_t *detach(int *idx, int count, int align);

        //!
        //! \brief append 尾部添加n个元素
        //! \param c
        //! \param n
        //! \return
        //!
        void* append(int c);

        //!
        //! \brief prepend 头部添加n个元素
        //! \param n
        //! \return
        //! \note 注意返回内存为正序
        //!
        void* prepend(int c);

        //!
        //! \brief insert 任意位置插入n个元素
        //! \param i
        //! \param n
        //! \return
        //!
        void* insert(int i, int c);

        //!
        //! \brief remove 任意位置删除n个元素
        //! \param i
        //! \param n
        //! \note 注意若为对象时，对象内存需要释放
        //!
        void remove(int i, int c);

        //!
        //! \brief move 将from位置开始的n个元素移动到to位置处
        //! \param from
        //! \param n
        //! \param to
        //! \note 注意当from+n超过to时则不移动
        //!
        void move(int from, int n, int to);

        //!
        //! \brief at 取出指定位置的元素
        //! \param i
        //! \return
        //!
        void* at(int i);

        //!
        //! \brief begin 返回元素的开始地址
        //! \return
        //!
        void* begin();

        //!
        //! \brief end 返回元素的结束地址
        //! \return
        //!
        void* end();

        static const float forward_retain;
        static const float backward_retain;
    };

protected:
    void *alloc(size_t size);
    void *realloc(void *m, size_t size);
    void destroy(void*);
};

template <typename T>
struct ky_allocate
{
    virtual T* alloc(size_t size)
    {

    }
    virtual T* realloc(T* mem, size_t size)
    {

    }
    virtual void destroy(T *mem)
    {

    }

};

#endif // ky_MEMCPY

