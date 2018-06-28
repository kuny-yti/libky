#ifndef ky_MAPDATA
#define ky_MAPDATA

#include "ky_define.h"
#include "ky_typeinfo.h"
#include "ky_memory.h"

#define rb_parent(r)   ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r)   ((r)->color == ky_treebase::rbRed)
#define rb_is_black(r)  ((r)->color == ky_treebase::rbBlack)
#define rb_set_black(r)  do { (r)->color = ky_treebase::rbBlack; } while (0)
#define rb_set_red(r)  do { (r)->color = ky_treebase::rbRed; } while (0)
#define rb_set_parent(r,p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)  do { (r)->color = (c); } while (0)

typedef ky_alloc<void> ky_mapalloc;

struct ky_treebase
{
    typedef enum  {rbBlack = 0, rbRed = 1, rbMask = 3}rbColor;
    rbColor color;
    ky_treebase *lchild;
    ky_treebase *rchild;
    ky_treebase *parent;

    //! 下一个或上一个节点
    ky_treebase *next();
    ky_treebase *prev();
};

template<typename KeyT, typename ValT>
struct ky_nodemap : ky_treebase
{
    KeyT key;
    ValT value;

    //! 销毁key和val类型，若为对象则执行析构函数
    void destroy();

    //! 返回当前的左节点或又节点
    inline ky_nodemap *left() const { return (ky_nodemap *)lchild; }
    inline ky_nodemap *right() const { return (ky_nodemap *)rchild; }

    inline ky_nodemap *next(){return (ky_nodemap *)ky_treebase::next();}
    inline ky_nodemap *prev(){return (ky_nodemap *)ky_treebase::prev();}
};

struct ky_treedata : ky_ref
{
    ky_atomic<size_t> count;
    ky_treebase header;
    ky_treebase *most_left;

    static const ky_treedata shared_null;
    static bool is_null (intptr p);
    static intptr null();

    //! 对红黑树的节点(x)进行左旋转
    void left_rotate(ky_treebase* x);
    //! 对红黑树的节点(y)进行右旋转
    void right_rotate(ky_treebase* y);
    //! 对红黑树进行修正
    void rebalance(ky_treebase *x);
    //! 销毁红黑树节点并进行修正
    template<typename Alloc = ky_mapalloc>
    inline void destroy_rebalance(ky_treebase *z, Alloc &al = ky_mapalloc());

    //! 创建一个红黑树节点
    template<typename Alloc = ky_mapalloc>
    inline ky_treebase *create(int alloc, ky_treebase *parent, bool left, Alloc &al);
    //! 销毁红黑树节点(以及子节点)
    template<typename Alloc = ky_mapalloc>
    inline void destroy(ky_treebase *x, Alloc &al = ky_mapalloc());

    //! 创建和销毁数据
    template<typename Alloc = ky_mapalloc>
    static ky_treedata *create_data(Alloc &al = ky_mapalloc())
    {
        ky_treedata *dat = (ky_treedata *)al.alloc(sizeof(ky_treedata));
        dat->count = 0;
        dat->set(ky_ref::refShareableDetach);

        rb_set_black(&dat->header);
        dat->header.lchild = NULL;
        dat->header.rchild = NULL;
        dat->most_left = &(dat->header);
        return dat;
    }

 };

template <typename KeyT, typename ValT, typename Alloc = ky_mapalloc >
struct ky_mapdata : public ky_treedata
{
    typedef ky_nodemap<KeyT, ValT> node;
    typedef const node const_node;

    inline node *root() const { return (node *)header.lchild;}

    inline const_node *end() const { return (const_node *)&header;}
    inline node *end() { return (node *)&header; }
    inline const_node *begin() const { if (root()) return (const_node*)most_left; return end();}
    inline node *begin() { if (root()) return (node *)most_left; return end();}

    node *find (const KeyT &key)const
    {
        node* tn = (node*)begin();

        forever (tn != end() && tn->key != key)
            tn = tn->next ();
        return tn == end() ? 0 : tn;
    }

    node *newnode(const KeyT &k, const ValT &v, node *parent = 0, bool left = false)
    {
        node *n = (node *)ky_treedata::create<Alloc>(sizeof(node), parent, left, staticAlloc);
        if (!ky_is_type(KeyT))
            new (&n->key) KeyT(k);
        else
            n->key = k;
        if (!ky_is_type(ValT))
            new (&n->value) ValT(v);
        else
            n->value = v;
        return n;
    }
    void delnode(node *z)
    {
        if (!ky_is_type(KeyT))
            z->key.~KeyT();
        if (!ky_is_type(ValT))
            z->value.~ValT();
        destroy_rebalance<Alloc>(z, staticAlloc);
    }

    static ky_mapdata *create()
    {
        return (ky_mapdata *)ky_treedata::create_data<Alloc>(staticAlloc);
    }
    inline void destroy()
    {
        if (root())
        {
            root()->destroy();
            ky_treedata::destroy<Alloc>(header.lchild, staticAlloc);
        }
        staticAlloc.destroy (this);
    }

    static Alloc staticAlloc;
};
template <typename KeyT, typename ValT, typename Alloc >
Alloc ky_mapdata<KeyT, ValT, Alloc>::staticAlloc;

// hash map

#endif // ky_MAPDATA

