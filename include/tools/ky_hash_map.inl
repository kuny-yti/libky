#ifndef KY_HASH_MAP_INL
#define KY_HASH_MAP_INL


template <typename K, typename V, typename Alloc>
void ky_hash_map<K, V, Alloc>::swap(ky_hash_map<K, V, Alloc> &rhs)
{
    if (rhs.buckets != buckets)
        ky_swap (rhs.buckets, buckets);
}


template <typename K, typename V, typename Alloc>
typename ky_hash_map<K, V, Alloc>::iterator ky_hash_map<K, V, Alloc>::insert(const K& key)
{
    return insert(key, V());
}

template <typename K, typename V, typename Alloc>
#if kyLanguage >= kyLanguage11
typename ky_hash_map<K, V, Alloc>::iterator ky_hash_map<K, V, Alloc>::insert(const K& k, const V& val)
#else
void ky_hash_map<K, V, Alloc>::insert(const K& k, const V& val);
#endif
{
    detach ();
    const size_t index = ky_hash_map(k) % capacity ();

    bucket_item* pred = NULL;
    bucket_item* item = buckets[index].first;
    while (item != NULL && item->key() != k)
    {
        pred = item;
        item = item->next;
    }

    if (item == NULL)
    {
        item = newitem (k, val);

        if (pred != NULL)
            pred->next = item;
        else
            buckets[index].first = item;
        ++header ()->ecount;

        if(header ()->ecount > header ()->ecountmax)
            grow((size_t)(capacity () * _ky_hash_map_::sGrowRate)+1);
    }

#if kyLanguage >= kyLanguage11
    return iterator(this, index, item);
#endif
}

template <typename K, typename V, typename Alloc>
V &ky_hash_map<K, V, Alloc>::at(const K &key)
{
    return this->operator [] (key);
}
template <typename K, typename V, typename Alloc>
const V &ky_hash_map<K, V, Alloc>::at(const K &key)const
{
    return this->operator [] (key);
}

template <typename K, typename V, typename Alloc>
V& ky_hash_map<K, V, Alloc>::operator[](const K& k)
{
    detach ();
    const size_t index = ky_hash(k) % capacity ();

    bucket_item* pred = NULL;
    bucket_item* item = buckets[index].first;
    while (item != NULL && item->key() != k)
    {
        pred = item;
        item = item->next;
    }

    if (item == NULL)
    {
        item = newitem (k, V());

        if (pred != NULL)
            pred->next = item;
        else
            buckets[index].first = item;
        ++header ()->ecount;

        if(header ()->ecount > header ()->ecountmax)
            grow((size_t)(capacity () * _ky_hash_map_::sGrowRate)+1);
    }

    return item->value ();
}

template <typename K, typename V, typename Alloc>
const V& ky_hash_map<K, V, Alloc>::operator[](const K& k) const
{
    return search(k).value ();
}

template <typename K, typename V, typename Alloc>
ky_hash_map<K, V, Alloc> &ky_hash_map<K, V, Alloc>::operator = (const ky_hash_map<K, V, Alloc> &rhs)
{
    if (!is_null() && header ()->lessref ())
        destroy ();
    buckets = rhs.buckets;
    if (!is_null ())
        header ()->addref ();
}

template <typename K, typename V, typename Alloc>
void ky_hash_map<K, V, Alloc>::reserve(size_t size)
{
    if (!is_null() && header()->lessref())
        destroy();
    detach();
    grow(size);
}
template <typename K, typename V, typename Alloc >
ky_list<V> ky_hash_map<K, V, Alloc>::values(const K &k) const
{
    ky_list<V> ov;
    if (is_null ())
        return ov;
    const size_t index = ky_hash_map(k) % capacity ();

    const bucket_item* item = buckets[index].first;
    while(item != NULL)
    {
        if (item->key () == k)
            ov.append (item->value ());
        item = item->next;
    }

    return ov;
}
template <typename K, typename V, typename Alloc >
void ky_hash_map<K, V, Alloc>::append(const K& k, const V& v){append(hash_entry(k, v));}

template <typename K, typename V, typename Alloc>
void ky_hash_map<K, V, Alloc>::append(const hash_entry& entry)
{
    detach ();
    const size_t index = ky_hash(entry.key ()) % capacity ();

    bucket_item* pred=0;
    bucket_item* item = buckets[index].first;
    while(item != NULL && item->key () != entry.key ())
    {
        pred = item;
        item = item->next;
    }

    if(item != NULL)
        item->entry (entry);
    else
    {

        bucket_item* nitem = newitem (entry.key (), entry.value ());

        if(pred != NULL)
            pred->next = nitem;
        else
            buckets[index].first = nitem;
        ++header ()->ecount;

        if(header ()->ecount > header ()->ecountmax)
            grow((size_t)(capacity () * _ky_hash_map_::sGrowRate) + 1);
    }
}

template <typename K, typename V, typename Alloc>
void ky_hash_map<K, V, Alloc>::remove(const K& k)
{
    detach ();
    const size_t index = ky_hash(k) % capacity ();

    bucket_item* pred=0;
    bucket_item* item = buckets[index].first;
    while (item != NULL && item->key ()!= k)
    {
        pred = item;
        item = item->next;
    }

    if(item != NULL)
    {
        if(pred != NULL)
            pred->next = item->next;
        else
            buckets[index].first = item->next;

        freeitem (item);
        --header ()->ecount;
    }
}

template <typename K, typename V, typename Alloc>
bool ky_hash_map<K, V, Alloc>::contains(const K& k) const
{
    if (is_null())
        return false;
    const size_t index = ky_hash(k) % capacity ();

    const bucket_item* item = buckets[index].first;
    while (item != NULL && item->key() != k)
        item = item->next;

    return item != NULL;
}

template <typename K, typename V, typename Alloc>
bool ky_hash_map<K, V, Alloc>::contains(const hash_entry& entry) const
{
    return contains(entry.key ());
}

template <typename K, typename V, typename Alloc>
const typename ky_hash_map<K, V, Alloc>::hash_entry& ky_hash_map<K, V, Alloc>::search(const K& k) const
{
    detach ();
    const size_t index = ky_hash_map(k) % capacity ();

    const bucket_item* item = buckets[index].first;
    while (item != NULL && item->key () != k)
        item = item->next;

    //if(item == NULL)
    //    throw EntryNotFoundError(findSource);

    return *item;
}

template <typename K, typename V, typename Alloc>
typename ky_hash_map<K, V, Alloc>::hash_entry&  ky_hash_map<K, V, Alloc>::search(const K& k)
{
    detach ();
    const size_t index = ky_hash_map(k) % capacity ();

    bucket_item* item = buckets[index].first;
    while (item != NULL && item->key () != k)
        item = item->next;

    //if(item == NULL)
    //    throw EntryNotFoundError(findSource);

    return *item;
}

template <typename K, typename V, typename Alloc>
typename ky_hash_map<K, V, Alloc>::iterator ky_hash_map<K, V, Alloc>::begin()
{
    return iterator(this);
}

template <typename K, typename V, typename Alloc>
typename ky_hash_map<K, V, Alloc>::const_iterator ky_hash_map<K, V, Alloc>::begin() const
{
    return const_iterator(this);
}

template <typename K, typename V, typename Alloc>
typename ky_hash_map<K, V, Alloc>::iterator ky_hash_map<K, V, Alloc>::end(void)
{
    return iterator(this, capacity (), 0);
}

template <typename K, typename V, typename Alloc>
typename ky_hash_map<K, V, Alloc>::const_iterator ky_hash_map<K, V, Alloc>::end(void) const
{
    return const_iterator(this,capacity (), 0);
}

template <typename K, typename V, typename Alloc>
typename ky_hash_map<K, V, Alloc>::iterator ky_hash_map<K, V, Alloc>::find(const K& k)
{
    detach ();
    const size_t index = ky_hash_map(k) % capacity ();

    bucket_item* item = buckets[index].first;
    while(item != NULL && item->key () != k)
        item = item->next;

    if(item != NULL)
        return iterator(this, index, item);

    return iterator(this, capacity (), 0);
}

#if kyLanguage >= kyLanguage11
template <typename K, typename V, typename Alloc>
V &ky_hash_map<K, V, Alloc>::operator []( K &&k)
{
    detach ();
    const size_t index = ky_hash_map(k) % capacity ();

    bucket_item* pred = NULL;
    bucket_item* item = buckets[index].first;
    while (item != NULL && item->key() != k)
    {
        pred = item;
        item = item->next;
    }

    if (item == NULL)
    {
        item = newitem (k, V());

        if (pred != NULL)
            pred->next = item;
        else
            buckets[index].first = item;
        ++header ()->ecount;

        if(header ()->ecount > header ()->ecountmax)
            grow((size_t)(capacity () * _ky_hash_map_::sGrowRate)+1);
    }

    return item->value ();
}
template <typename K, typename V, typename Alloc>
typename ky_hash_map<K, V, Alloc>::const_iterator ky_hash_map<K, V, Alloc>::find(const K& k) const
{
    detach ();
    const size_t index = ky_hash_map(k) % capacity ();

    const bucket_item* item = buckets[index].first;
    while(item != NULL && item->key () != k)
        item = item->next;

    if(item != NULL)
        return const_iterator(this, index, item);

    return const_iterator(this, capacity (), 0);
}
#endif


#if kyLanguage >= kyLanguage11
template <typename K, typename V, typename Alloc>
typename ky_hash_map<K, V, Alloc>::iterator ky_hash_map<K, V, Alloc>::erase(const_iterator it)
{
    detach ();
    iterator nit = it;
    ++nit;
    if(it.table == this && it.bindex < capacity () && it.bitem != NULL)
    {
        bucket_item* pred = NULL;
        bucket_item* item = buckets[it.bindex].first;
        while (item != NULL && item != it.bitem)
        {
            pred = item;
            item = item->next;
        }

        if(pred != NULL)
            pred->next = item->next;
        else
            buckets[it.bindex].first = item->next;

        freeitem (item);
        --header ()->ecount;
    }
    return nit;
}
#endif

template <typename K, typename V, typename Alloc>
#if kyLanguage >= kyLanguage11
typename ky_hash_map<K, V, Alloc>::iterator ky_hash_map<K, V, Alloc>::erase(iterator it)
#else
void ky_hash_map<K, V, Alloc>::erase(iterator pos)
#endif
{
    detach ();
#if kyLanguage >= kyLanguage11
    iterator nit = it;
    ++nit;
#endif
    if(it.table == this && it.bindex < capacity () && it.bitem != NULL)
    {
        bucket_item* pred = NULL;
        bucket_item* item = buckets[it.bindex].first;
        while (item != NULL && item != it.bitem)
        {
            pred = item;
            item = item->next;
        }

        if(pred != NULL)
            pred->next = item->next;
        else
            buckets[it.bindex].first = item->next;

        freeitem (item);
        --header ()->ecount;
    }
    #if kyLanguage >= kyLanguage11
    return nit;
#endif
}
template <typename K, typename V, typename Alloc>
size_t ky_hash_map<K, V, Alloc>::erase(const K& key)
{
    const size_t idx = ky_hash_map (key) % capacity ();
    remove(key);
    return idx;
}
template <typename K, typename V, typename Alloc>
#if kyLanguage >= kyLanguage11
typename ky_hash_map<K, V, Alloc>::iterator ky_hash_map<K, V, Alloc>::erase(const_iterator first, const_iterator last)
#else
void ky_hash_map<K, V, Alloc>::erase(iterator first, iterator last);
#endif
{
    detach ();
#if kyLanguage >= kyLanguage11
    iterator nit = last;
    ++nit;
#endif
    for (const_iterator bit = first; bit != last;)
        bit = erase(bit);
#if kyLanguage >= kyLanguage11
    return nit;
#endif
}

template <typename K, typename V, typename Alloc>
void ky_hash_map<K, V, Alloc>::remove(const iterator& it)
{
    detach ();
    if(it.table == this && it.bindex < capacity () && it.bitem != NULL)
    {
        bucket_item* pred = NULL;
        bucket_item* item = buckets[it.bindex].first;
        while (item != NULL && item != it.bitem)
        {
            pred = item;
            item = item->next;
        }

        if(pred != NULL)
            pred->next = item->next;
        else
            buckets[it.bindex].first = item->next;

        freeitem (item);
        --header ()->ecount;
    }
}
template <typename K, typename V, typename Alloc>
void ky_hash_map<K, V, Alloc>::remove(const const_iterator& it)
{
    detach ();
    if(it.table == this && it.bindex < capacity () && it.bitem != NULL)
    {
        bucket_item* pred = NULL;
        bucket_item* item = buckets[it.bindex].first;
        while (item != NULL && item != it.bitem)
        {
            pred = item;
            item = item->next;
        }

        if(pred != NULL)
            pred->next = item->next;
        else
            buckets[it.bindex].first = item->next;

        freeitem (item);
        --header ()->ecount;
    }
}
template <typename K, typename V, typename Alloc>
void ky_hash_map<K, V, Alloc>::clear()
{
    detach ();
    for(size_t i = 0; i < count ();++i)
    {
        while(buckets[i].first != NULL)
        {
            bucket_item* next = buckets[i].first->next;
            freeitem (buckets[i].first);
            buckets[i].first = next;
        }
    }

    header ()->ecount = 0;
}

template <typename K, typename V, typename Alloc>
void ky_hash_map<K, V, Alloc>::detach()
{
    if (!is_null() && header ()->ref.is_shared () && header ()->ref.has_detach ())
    {
        struct{_ky_hash_map_*d; hash_bucket* e;} olds;
        struct{_ky_hash_map_*d; hash_bucket* e;} news;
        olds.d = header();
        olds.e = buckets;

        news.d = (_ky_hash_map_*)Alloc::alloc(sizeof(_ky_hash_map_)+sizeof(hash_bucket)*capacity ());
        news.d->bcapacity = olds.d->bcapacity;
        news.d->ecount = olds.d->ecount;
        news.d->ecountmax = olds.d->ecountmax;
        news.d->ref.set (ky_ref::refShareableDetach);

        news.e = (hash_bucket*)(news.d+1);
        ky_mem()->zero (news.e, sizeof(hash_bucket) *capacity ());
        for(size_t i = 0; i < capacity (); ++i)
        {
            while (olds.e[i].first != NULL)
            {
                bucket_item* item = olds.e[i].first;
                olds.e[i].first = item->next;

                bucket_item *nitem = newitem(item->key (), item->value ());
                const size_t nbi = ky_hash(item->key ()) % capacity ();
                nitem->next = news.e[nbi].first;
                news.e[nbi].first = nitem;
            }
        }
        if (olds.d->lessref ())
            destroy ();
        buckets = news.e;
    }
    else if (is_null())
        buckets = create();
}

template <typename K, typename V, typename Alloc>
typename ky_hash_map<K, V, Alloc>::hash_bucket *ky_hash_map<K, V, Alloc>::create(size_t mins)
{
    struct{_ky_hash_map_*d; hash_bucket* e;} news;
    news.d = (_ky_hash_map_*)Alloc::alloc (sizeof(_ky_hash_map_)+sizeof(hash_bucket) * mins);
    news.d->bcapacity = mins;
    news.d->ecount = 0;
    news.d->ecountmax = mins * news.d->sMaxUsageRate;
    news.d->ref.set (ky_ref::refShareableDetach);

    news.e = (hash_bucket*)(news.d+1);
    ky_mem()->zero (news.e, sizeof(hash_bucket) * mins);
    return news.e;
}

template <typename K, typename V, typename Alloc>
void ky_hash_map<K, V, Alloc>::destroy()
{
    if (!is_null() && header()->lessref ())
    {
        for(size_t i = 0; i < capacity(); ++i)
        {
            while(buckets[i].first != NULL)
            {
                bucket_item* next = buckets[i].first->next;
                freeitem (buckets[i].first);
                buckets[i].first = next;
            }
        }

        Alloc::destroy (header());
    }
    buckets = (hash_bucket*)_ky_hash_map_::null ();
}
template <typename K, typename V, typename Alloc>
void ky_hash_map<K, V, Alloc>::grow(size_t newsize)
{
    struct{_ky_hash_map_*d; hash_bucket* e;} news;
    struct{_ky_hash_map_*d; hash_bucket* e;} olds;
    olds.d = header();
    olds.e = buckets;
    news.d = (_ky_hash_map_*)Alloc::alloc (sizeof(_ky_hash_map_) +
                                       sizeof(hash_bucket) * newsize);
    news.e = (hash_bucket*)(news.d+1);
    ky_mem()->zero (news.e, sizeof(hash_bucket) * newsize);

    for(size_t i = 0; i < capacity (); ++i)
    {
        while(olds.e[i].first != NULL)
        {
            bucket_item* item = olds.e[i].first;
            olds.e[i].first = item->next;

            size_t nbi = ky_hash(item->key ()) % newsize;

            item->next = news.e[nbi].first;
            news.e[nbi].first = item;
        }
    }

    news.d->ref.set (ky_ref::refShareableDetach);
    news.d->bcapacity = newsize;
    news.d->ecount = olds.d->ecount;
    news.d->ecountmax = (size_t)(newsize* olds.d->sMaxUsageRate);
    buckets = news.e;
    Alloc::destroy (olds.d);
}

#endif // ky_hash_map_INL
