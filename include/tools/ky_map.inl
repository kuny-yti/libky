#ifndef ky_MAP_INL
#define ky_MAP_INL

template<typename KeyT, typename ValT>
void ky_nodemap<KeyT, ValT>::destroy()
{
    if (!ky_is_type(KeyT))
        key.~KeyT();
    if (!ky_is_type(ValT))
        value.~ValT();
    if (!ky_is_type(KeyT) || !ky_is_type(ValT))
    {
        if (left())
            left()->destroy();
        if (right())
            right()->destroy();
    }
}
// 销毁红黑树节点并进行修正
template<typename Alloc>
void ky_treedata::destroy_rebalance(ky_treebase *z, Alloc &al)
{
    ky_treebase *&rot = header.lchild;
    ky_treebase *y = z;
    ky_treebase *x;
    ky_treebase *x_parent;
    if (y->lchild == NULL)
    {
        x = y->rchild;
        if (y == most_left)
        {
            if (x)
                most_left = x; // It cannot have (left) children due the red black invariant.
            else
                most_left = y->parent;
        }
    }
    else
    {
        if (y->rchild == NULL)
            x = y->lchild;
        else
        {
            y = y->rchild;
            while (y->lchild != NULL)
                y = y->lchild;
            x = y->rchild;
        }
    }
    if (y != z)
    {
        z->lchild->parent = y;
        y->lchild = z->lchild;
        if (y != z->rchild)
        {
            x_parent = y->parent;
            if (x)
                x->parent = y->parent;
            y->parent->lchild = x;
            y->rchild = z->rchild;
            z->rchild->parent = y;
        }
        else
            x_parent = y;

        if (rot == z)
            rot = y;
        else if (z->parent->lchild == z)
            z->parent->lchild = y;
        else
            z->parent->rchild = y;
        y->parent = z->parent;
        // Swap the colors
        ky_treebase::rbColor c = y->color;
        rb_set_color(y, z->color);
        rb_set_color(z, c);
        y = z;
    }
    else
    {
        x_parent = y->parent;
        if (x)
            x->parent = y->parent;
        if (rot == z)
            rot = x;
        else if (z->parent->lchild == z)
            z->parent->lchild = x;
        else
            z->parent->rchild = x;
    }
    if (y->color != ky_treebase::rbRed)
    {
        while (x != rot && (x == NULL || rb_is_black(x)))
        {
            if (x == x_parent->lchild)
            {
                ky_treebase *w = x_parent->rchild;
                if (rb_is_red(w))
                {
                    rb_set_black(w);
                    rb_set_red(x_parent);
                    left_rotate(x_parent);
                    w = x_parent->rchild;
                }
                if ((w->lchild == NULL || rb_is_black(w->lchild)) &&
                    (w->rchild == NULL || rb_is_black(w->rchild)))
                {
                    rb_set_red(w);
                    x = x_parent;
                    x_parent = x_parent->parent;
                }
                else
                {
                    if (w->rchild == NULL || rb_is_black(w->rchild))
                    {
                        if (w->lchild)
                            rb_set_black(w->lchild);
                        rb_set_red(w);
                        right_rotate(w);
                        w = x_parent->rchild;
                    }
                    rb_set_color(w, x_parent->color);
                    rb_set_black(x_parent);
                    if (w->rchild)
                        rb_set_black(w->rchild);
                    left_rotate(x_parent);
                    break;
                }
            }
            else
            {
                ky_treebase *w = x_parent->lchild;
                if (rb_is_red(w))
                {
                    rb_set_black(w);
                    right_rotate(x_parent);
                    w = x_parent->lchild;
                }
                if ((w->rchild == NULL || rb_is_black(w->rchild)) &&
                        (w->lchild == NULL || rb_is_black(w->lchild)))
                {
                    rb_set_red(w);
                    x = x_parent;
                    x_parent = x_parent->parent;
                }
                else
                {
                    if (w->lchild == 0 || rb_is_black(w->lchild))
                    {
                        if (w->rchild)
                            rb_set_black(w->rchild);
                        rb_set_red(w);
                        left_rotate(w);
                        w = x_parent->lchild;
                    }
                    rb_set_color(w, x_parent->color);
                    rb_set_black(x_parent);
                    if (w->lchild)
                        rb_set_black(w->lchild);
                    right_rotate(x_parent);
                    break;
                }
            }
        }
        if (x)
            rb_set_black(x);
    }
    al.destroy(y);
    --count;
}
template<typename Alloc>
ky_treebase *ky_treedata::create(int alloc, ky_treebase *parent, bool left, Alloc &al)
{
    ky_treebase *node = (ky_treebase *)al.alloc(alloc);

    memset(node, 0, alloc);
    ++count;

    if (parent)
    {
        if (left)
        {
            parent->lchild = node;
            if (parent == most_left)
                most_left = node;
        }
        else
            parent->rchild = node;

        node->parent = parent;
        rebalance(node);
    }
    return node;
}
// 销毁红黑树
template<typename Alloc>
void ky_treedata::destroy(ky_treebase *x, Alloc &al)
{
    if (x->lchild)
        destroy<Alloc>(x->lchild, al);
    if (x->rchild)
        destroy<Alloc>(x->rchild, al);
    al.destroy(x);
}


template <typename K, typename V, typename Alloc>
ky_map<K, V, Alloc>::ky_map():
    impl((ky_mapdata<K, V, Alloc> *)ky_mapdata<K, V, Alloc>::null ())
{
}

template <typename K, typename V, typename Alloc>
ky_map<K, V, Alloc>::ky_map(const ky_map<K, V, Alloc> &m):
    impl((ky_mapdata<K, V, Alloc> *)ky_mapdata<K, V, Alloc>::null ())
{
    if (!is_null() && impl->lessref())
        impl->destroy();
    if (impl == m.impl)
        return ;
    impl = m.impl;
    if (impl->has_shareable())
        impl->addref();
    else
        detach();
}
template <typename K, typename V, typename Alloc>
ky_map<K, V, Alloc>::~ky_map()
{
    if (!is_null() && impl->lessref())
        impl->destroy();
    impl = NULL;
}
template <typename K, typename V, typename Alloc>
void ky_map<K, V, Alloc>::detach()
{
    if (!is_null() && impl->has_detach() && impl->is_shared())
        copy();
}
template <typename K, typename V, typename Alloc>
void ky_map<K, V, Alloc>::copy()
{
    ky_mapdata<K, V, Alloc> *tmp = impl;
    impl = ky_mapdata<K, V, Alloc>::create();

    for (size_t i = 0; i < tmp->count; ++i)
    {
        iterator ite = iterator(tmp->begin())+i;
        this->append(ite.key(), ite.value());
    }
    if (tmp->lessref())
        tmp->destroy();
}

template <typename K, typename V, typename Alloc>
typename ky_map<K, V, Alloc>::iterator ky_map<K, V, Alloc>::begin()
{
    detach();
    return iterator(impl->begin());
}
template <typename K, typename V, typename Alloc>
typename ky_map<K, V, Alloc>::const_iterator ky_map<K, V, Alloc>::begin() const
{
    return const_iterator(impl->begin());
}
template <typename K, typename V, typename Alloc>
typename ky_map<K, V, Alloc>::iterator ky_map<K, V, Alloc>::end()
{
    detach();
    return iterator(impl->end());
}

template <typename K, typename V, typename Alloc>
typename ky_map<K, V, Alloc>::const_iterator ky_map<K, V, Alloc>::end() const
{
    return const_iterator(impl->end());
}
#if kyLanguage < kyLanguage11
template <typename K, typename V, typename Alloc>
void ky_map<K, V, Alloc>::erase(iterator pos)
{
    if (is_null() || pos == iterator(impl->end()))
        return ;
    ky_nodemap<K, V> *node = (begin()+inx).ope;
    impl->delnode(node);
}

template <typename K, typename V, typename Alloc>
void ky_map<K, V, Alloc>::erase(iterator first, iterator last)
{
    if (is_null())
        return ;
    while (first != last)
    {
        erase(first);
        ++first;
    }
}
template <typename K, typename V, typename Alloc>
typename ky_map<K, V, Alloc>::const_iterator ky_map<K, V, Alloc>::find(const K &key)const
{
    return const_iterator(impl->find(key));
}
template <typename K, typename V, typename Alloc>
void ky_map<K, V, Alloc>::insert(const K& key, const V& val)
{
    if (is_null())
        impl = ky_mapdata<K, V, Alloc>::create();

    ky_nodemap<K, V> *node = impl->root();
    ky_nodemap<K, V> *y = d->end();
    ky_nodemap<K, V> *lastnode = NULL;
    bool  left = true;
    while (node != NULL)
    {
        y = node;
        if (key < node->key)
        {
            lastnode = node;
            left = true;
            node = node->left();
        } else {
            left = false;
            node = node->right();
        }
    }
    if (lastnode && lastnode->key < key)
    {
        lastnode->value = val;
        return ;//iterator(lastnode);
    }
    ky_nodemap<K, V> *z = d->create(key, val, y, left);
    //return iterator(z);
}
#endif
template <typename K, typename V, typename Alloc>
ky_map<K, V, Alloc> &ky_map<K, V, Alloc>::operator = (const ky_map<K, V, Alloc> &rhs)
{
    if (impl != rhs.impl)
    {
        ky_map<K, V, Alloc> tmp(rhs);
        tmp.swap(*this);
    }
    return *this;
}

template <typename K, typename V, typename Alloc>
V &ky_map<K, V, Alloc>::operator [](const K &key)
{
    if (is_null())
        impl = ky_mapdata<K, V, Alloc>::create();

    detach();
    ky_nodemap<K, V> *node = impl->find(key);
    if (node == NULL)
        return *insert(key);
    return node->value;
}

template <typename K, typename V, typename Alloc>
bool ky_map<K, V, Alloc>::empty() const
{
    return is_empty();
}
template <typename K, typename V, typename Alloc>
size_t ky_map<K, V, Alloc>::size() const
{
    return count();
}

template <typename K, typename V, typename Alloc>
V &ky_map<K, V, Alloc>::at(const K &key)
{
    return operator [](key);
}
template <typename K, typename V, typename Alloc>
const V &ky_map<K, V, Alloc>::at(const K &key)const
{
    return operator [](key);
}

template <typename K, typename V, typename Alloc>
typename ky_map<K, V, Alloc>::iterator ky_map<K, V, Alloc>::insert(const K& key)
{
    return this->insert(key, V());
}
template <typename K, typename V, typename Alloc>
typename ky_map<K, V, Alloc>::iterator ky_map<K, V, Alloc>::insert(iterator pos, const K& key)
{
    if (is_null())
        impl = ky_mapdata<K, V, Alloc>::create();

    if (pos == end())
    {
        ky_nodemap<K, V> *node = (ky_nodemap<K, V> *)pos.ope->lchild;
        if (node != NULL)
        {
            while (node->rchild != NULL)
                node = (ky_nodemap<K, V> *)node->rchild;
            if (key < node->key)
                return this->insert(key);

            ky_nodemap<K, V> *z = impl->newnode(key, V(), node, false);
            return iterator(z);
        }
        return this->insert(key);
    }
    else
    {
        ky_nodemap<K, V>  *next = (ky_nodemap<K, V> *)pos.ope;
        if (next->key < key)
            return this->insert(key);

        if (pos == begin())
        {
            if (next->key < key)
                return iterator(next);
            ky_nodemap<K, V> *z = impl->create(key, V(), begin().ope, true);
            return iterator(z);
        }
        else
        {
            ky_nodemap<K, V> *prev = (ky_nodemap<K, V>*)pos.ope->prev();
            if (key < prev->key)
                return this->insert(key);

            if (next->key < key)
                return iterator(next);

            if (prev->rchild == NULL)
            {
                ky_nodemap<K, V> *z = impl->create(key, V(), prev, false);
                return iterator(z);
            }
            if (next->lchild == NULL)
            {
                ky_nodemap<K, V> *z = impl->create(key, V(), next, true);
                return iterator(z);
            }
            return this->insert(key);
        }
    }
}
template <typename K, typename V, typename Alloc>
typename ky_map<K, V, Alloc>::iterator ky_map<K, V, Alloc>::insert(const K& key, const V& val)
{
    if (is_null())
        impl = ky_mapdata<K, V, Alloc>::create();

    detach();
    ky_nodemap<K, V> *node = impl->root();
    ky_nodemap<K, V> *y = impl->end();
    ky_nodemap<K, V> *lastnode = NULL;
    bool  left = true;
    while (node != NULL)
    {
        y = node;
        if (!(node->key < key))
        {
            lastnode = node;
            left = true;
            node = node->left();
        } else {
            left = false;
            node = node->right();
        }
    }
    if (lastnode &&  !(key < lastnode->key))
    {
        lastnode->value = val;
        return iterator(lastnode);
    }
    ky_nodemap<K, V> *z = impl->newnode(key, val, y, left);
    return iterator(z);
}

template <typename K, typename V, typename Alloc>
void ky_map<K, V, Alloc>::swap(ky_map<K, V, Alloc> &rhs)
{
    ky_mapdata<K, V, Alloc> *tmp = impl;
    impl = rhs.impl;
    rhs.impl = tmp;
}
template <typename K, typename V, typename Alloc>
void ky_map<K, V, Alloc>::clear()
{
    *this = ky_map<K, V, Alloc>();
}
template <typename K, typename V, typename Alloc>
typename ky_map<K, V, Alloc>::iterator ky_map<K, V, Alloc>::find(const K &key)
{
    ky_nodemap<K, V> *node = impl->find(key);
    return iterator(node ? node : impl->end());
}

/// C++ 11
#if kyLanguage >= kyLanguage11
template <typename K, typename V, typename Alloc>
ky_map<K, V, Alloc>::ky_map(ky_map<K, V, Alloc> &&m):
    impl((ky_mapdata<K, V, Alloc> *)ky_mapdata<K, V, Alloc>::null ())
{
    impl = m.impl;
    m.clear();
}
template <typename K, typename V, typename Alloc>
ky_map<K, V, Alloc>::ky_map(std::initializer_list<std::pair<K,V> > list):
    impl((ky_mapdata<K, V, Alloc> *)ky_mapdata<K, V, Alloc>::null ())
{
    for (typename std::initializer_list<std::pair<K, V> >::const_iterator it = list.begin();
         it != list.end(); ++it)
        append(it->first, it->second);
}
template <typename K, typename V, typename Alloc>
typename ky_map<K, V, Alloc>::const_iterator ky_map<K, V, Alloc>::cbegin() const
{
    return const_iterator(impl->begin());
}
template <typename K, typename V, typename Alloc>
typename ky_map<K, V, Alloc>::const_iterator ky_map<K, V, Alloc>::cend() const
{
    return const_iterator(impl->end());
}

template <typename K, typename V, typename Alloc>
V &ky_map<K, V, Alloc>::operator []( K &&key)
{
    if (is_null())
        impl = ky_mapdata<K, V, Alloc>::create();

    detach();
    ky_nodemap<K, V> *node = impl->find(key);
    if (!node)
        return *insert(key);
    return node->value;
}
template <typename K, typename V, typename Alloc>
typename ky_map<K, V, Alloc>::iterator ky_map<K, V, Alloc>::erase(const_iterator pos)
{
    if (is_null() || pos == iterator(impl->end()))
        return pos;
    detach ();
    ky_nodemap<K, V> *node = pos.ope;
    ++pos;
    impl->delnode(node);
    return pos;
}
template <typename K, typename V, typename Alloc>
typename ky_map<K, V, Alloc>::iterator ky_map<K, V, Alloc>::erase(iterator pos)
{
    if (is_null() || pos == iterator(impl->end()))
        return pos;
    detach ();
    ky_nodemap<K, V> *node = pos.ope;
    ++pos;
    impl->delnode(node);
    return pos;
}
#endif
template <typename K, typename V, typename Alloc>
ky_map<K, V, Alloc>::ky_map(const std::map<K, V> &m):
    impl((ky_mapdata<K, V, Alloc> *)ky_mapdata<K, V, Alloc>::null ())
{
    impl = ky_mapdata<K, V, Alloc>::create();
    form(m);
}

template <typename K, typename V, typename Alloc>
std::map<K, V> &ky_map<K, V, Alloc>::to_std()
{
    std::map<K, V> map;
    const_iterator it = end();
    for (const_iterator it = begin(); it != end(); ++it)
        map.insert(map.end(), std::pair<K, V>(it.key(), it.value()));
    return map;
}
template <typename K, typename V, typename Alloc>
void ky_map<K, V, Alloc>::form(std::map<K, V> &m)
{
    detach ();
    clear();
    if (is_null())
        impl = ky_mapdata<K, V, Alloc>::create();
    for (typename std::map<K, V>::const_iterator it = m.begin(); it != m.end(); ++it)
        this->insert((*it).first, (*it).second);
}

template <typename K, typename V, typename Alloc>
const V ky_map<K, V, Alloc>::operator [](const K &key)const
{
    return value(key);
}

template <typename K, typename V, typename Alloc>
bool ky_map<K, V, Alloc>::operator== (const ky_map<K, V, Alloc> &rhs) const
{
    if (size() != rhs.size())
        return false;
    if (impl == rhs.impl)
        return true;

    const_iterator it1 = begin();
    const_iterator it2 = rhs.begin();

    while (it1 != end())
    {
        if (!(it1.value() == it2.value()) || it1.key() != it2.key())
            return false;
        ++it2;
        ++it1;
    }
    return true;
}

template <typename K, typename V, typename Alloc>
V ky_map<K, V, Alloc>::value(const K& key)
{
    ky_nodemap<K, V> *node = impl->find(key);
    return node ? node->value : V();
}

template <typename K, typename V, typename Alloc>
void ky_map<K, V, Alloc>::append(const K& key, const V& val)
{
    this->insert(key, val);
}

template <typename K, typename V, typename Alloc>
void ky_map<K, V, Alloc>::remove(const K& key)
{
    detach();
    while (ky_nodemap<K, V> *node = impl->find(key))
        impl->delnode(node);
}

template <typename K, typename V, typename Alloc>
void ky_map<K, V, Alloc>::remove()
{
    detach ();
    clear();
}

#if kyLanguage >= kyLanguage11
template <typename K, typename V, typename Alloc>
void ky_map<K, V, Alloc>::insert(const_iterator pos, const K &key, const V &value)
{
    if (is_null())
        impl = ky_mapdata<K, V, Alloc>::create();
    if (pos == end())
    {
        ky_nodemap<K, V> *node = (ky_nodemap<K, V> *)pos.ope->lchild;
        if (node != NULL)
        {
            while (node->rchild != NULL)
                node = (ky_nodemap<K, V> *)node->rchild;
            if (key < node->key)
            {
                this->insert(key, value);
                return ;
            }

            ky_nodemap<K, V> *z = impl->newnode(key, value, node, false);
            return ;//iterator(z);
        }
        return this->insert(key);
    }
    else
    {
        ky_nodemap<K, V>  *next = (ky_nodemap<K, V> *)pos.ope;
        if (next->key < key)
        {
            this->insert(key, value);
            return ;
        }

        if (pos == begin())
        {
            if (next->key < key)
            {
                next->value = value;
                return ;
            }
            ky_nodemap<K, V> *z = impl->newnode(key, value, begin().ope, true);
            return ;
        }
        else
        {
            ky_nodemap<K, V> *prev = (ky_nodemap<K, V>*)pos.ope->prev();
            if (key < prev->key)
            {
                 this->insert(key, value);
                return ;
            }

            if (next->key < key)
            {
                next->value = value;
                return ;
            }

            if (prev->rchild == NULL)
            {
                ky_nodemap<K, V> *z = impl->newnode(key, value, prev, false);
                return ;
            }
            if (next->lchild == NULL)
            {
                ky_nodemap<K, V> *z = impl->newnode(key, value, next, true);
                return ;
            }
            return this->insert(key, value);
        }
    }
}
#endif

template <typename K, typename V, typename Alloc>
bool ky_map<K, V, Alloc>::contains(const K &key) const
{
    return impl->find(key) != NULL;
}

template <typename K, typename V, typename Alloc>
const K ky_map<K, V, Alloc>::key(const V &value) const
{
    const_iterator i = begin();
    while (i != end())
    {
        if (i.value() == value)
            return i.key();
        ++i;
    }

    return K();
}

template <typename K, typename V, typename Alloc>
const V ky_map<K, V, Alloc>::value(const K &key) const
{
    ky_nodemap<K, V> *node = impl->find(key);
    return node ? node->value : V();
}

template <typename K, typename V, typename Alloc>
ky_list<K> ky_map<K, V, Alloc>::keys() const
{
    ky_list<K> res;
    const_iterator i = begin();
    while (i != end())
    {
        res.append(i.key());
        ++i;
    }
    return res;
}
template <typename K, typename V, typename Alloc>
ky_list<K> ky_map<K, V, Alloc>::keys(const V &value) const
{
    ky_list<K> res;
    const_iterator i = begin();
    while (i != end())
    {
        if (i.value() == value)
            res.append(i.key());
        ++i;
    }
    return res;
}

template <typename K, typename V, typename Alloc>
ky_list<V> ky_map<K, V, Alloc>::values() const
{
    ky_list<V> res;
    const_iterator i = begin();
    while (i != end())
    {
        res.append(i.value());
        ++i;
    }
    return res;
}
template <typename K, typename V, typename Alloc>
ky_list<V> ky_map<K, V, Alloc>::values(const K &key) const
{
    ky_list<V> res;
    const_iterator i = begin();
    while (i != end())
    {
        if (key == i.key())
            res.append(i.value());
        ++i;
    }
    return res;
}

template <typename K, typename V, typename Alloc>
V &ky_map<K, V, Alloc>::first()
{
    return *begin();
}
template <typename K, typename V, typename Alloc>
const V &ky_map<K, V, Alloc>::first()const
{
    return *cbegin();
}

template <typename K, typename V, typename Alloc>
V &ky_map<K, V, Alloc>::last()
{
    return *(--end());
}
template <typename K, typename V, typename Alloc>
const V &ky_map<K, V, Alloc>::last()const
{
    return *(--cend());
}


template <typename K, typename V, typename Alloc>
size_t ky_map<K, V, Alloc>::count()const
{
    return impl->count;
}
template <typename K, typename V, typename Alloc>
bool ky_map<K, V, Alloc>::is_empty()const
{
    return impl->count == 0;
}
template <typename K, typename V, typename Alloc>
bool ky_map<K, V, Alloc>::is_null()const
{
    return ky_mapdata<K, V, Alloc>::is_null((intptr)impl) ;
}
#endif // ky_MAP_INL
