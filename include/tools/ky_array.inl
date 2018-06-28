


template <typename T>
T* ky_array<T>::__detach_insert(int i, int c)
{
    const bool the_copy = !is_empty ();

    T *n = (T*)Layout::begin ();
    LayoutHeader *oldh = Layout::detach(&i, c, sizeOf);

    if (the_copy)
    {
        T* b = (T*)Layout::begin ();
        T* e = (T*)Layout::end();

        T* cur = b;
        T* t = n;
        T* to = (b+(i));
        while(cur != to)
        {
            if (not_construct)
                *cur = *t;
            else
                new (cur) T(*t);
            cur ++;
            t  ++;
        }

        cur = (b + (i +c));
        to = e;
        t = n + i;
        while(cur != to)
        {
            if (not_construct)
                *cur = *t;
            else
                new (cur) T(*t);
            cur ++;
            t ++;
        }
    }

    Layout old(oldh);
    if (!old.is_null() && old.lessref ())
    {
        T * cur = (T*)old.begin();
        T * e = (T*)old.end ();
        while (cur++ != e)
        {
            if (not_construct)
                ;
            else
                (cur)->~T();
        }
        old.destroy ();
    }

    return (T*)Layout::at (i);
}

template <typename T>
void ky_array<T>::__destroy_helper()
{
    if (!is_null() && lessref ())
    {
        T* cur = (T*)Layout::begin();
        T* e = (T*)Layout::end ();
        while (cur++ != e)
        {
            if (not_construct)
                ;
            else
                (cur)->~T();
        }
        Layout::destroy ();
    }
}

template<typename T>
void ky_array<T>::__detach_helper ()
{
    if (refer ().is_shared ())
    {
        if (!is_null () && refer ().has_detach ())
            __detach_insert (0, 0);
    }
}

template<typename T>
ky_array<T>::ky_array():
    Layout()
{
}

template<typename T>
ky_array<T>::ky_array(size_t size):
    Layout()
{
    reserve(size);
}

template<typename T>
ky_array<T>::ky_array(const ky_array<T> &rhs):
    Layout()
{
    *this = rhs;
}

template<typename T>
ky_array<T>::ky_array(const Type *buf, size_t len):
    Layout()
{
    append(buf, len);
}

template<typename T>
ky_array<T>::~ky_array()
{
    __destroy_helper();
}

template<typename T>
void ky_array<T>::reserve(size_t size)
{
    Layout::reserve (size, sizeOf);
}

template<typename T>
void ky_array<T>::resize(size_t s)
{
    __detach_helper ();
    Layout::resize (s, sizeOf);
}

template<typename T>
void ky_array<T>::resize(size_t s, const Type &v)
{
    resize(s);
    fill(v, s);
}

template<typename T>
ky_array<T>::operator T *() const
{
    return (T*)Layout::begin ();
}

template<typename T>
ky_array<T>::operator T *()
{
    __detach_helper ();
    return (T*)Layout::begin ();
}

template<typename T>
ky_array<T>::operator void *() const
{
    return (T*)Layout::begin ();
}

template<typename T>
ky_array<T>::operator void *()
{
    __detach_helper ();
    return (T*)Layout::begin ();
}

template<typename T>
T *ky_array<T>::data()
{
    __detach_helper();
    return (T*)Layout::begin ();
}

template<typename T>
T *ky_array<T>::data() const
{
    ky_array<T> *self = (ky_array<T> *)this;
    return (T*)self->Layout::begin ();
}

template<typename T>
T *ky_array<T>::offset(int inx)
{
    kyASSERT(inx < size() && inx >= 0, "The index is illegal");
    __detach_helper();
    return (T*)Layout::at (inx);
}

template<typename T>
T *ky_array<T>::offset(int inx)const
{
    kyASSERT(inx < size() && inx >= 0, "The index is illegal");
    ky_array<T> *self = (ky_array<T> *)this;
    return (T*)self->Layout::at (inx);
}

template<typename T>
void ky_array<T>::clear()
{
    fill(Type(), size(), 0);
}

template<typename T>
void ky_array<T>::fill(const Type &c, size_t len, int pos)
{
    if (size())
    {
        __detach_helper ();
        len = ((len+pos) > size() ? size()-pos : len);
        T* ptr = (T*)Layout::begin ();
        do
            *(ptr +pos++) = c;
        while (--len);
    }
}

template<typename T>
T ky_array<T>::at(int i)
{
    kyASSERT(i < size() && i >= 0, "The index is illegal");
    return *(T*)Layout::at (i);
}

template<typename T>
const T ky_array<T>::at(int i)const
{
    kyASSERT(i < size() && i >= 0, "The index is illegal");
    ky_array<T> *self = (ky_array<T> *)this;
    return *(T*)self->Layout::at (i);
}

template<typename T>
T &ky_array<T>::operator[](int i)
{
    kyASSERT(i < size() && i >= 0, "The index is illegal");
    __detach_helper ();
    return *(T*)Layout::at (i);
}

template<typename T>
const T &ky_array<T>::operator[](int i) const
{
    kyASSERT(i < size() && i >= 0, "The index is illegal");
    ky_array<T> *self = (ky_array<T> *)this;
    return *(T*)self->Layout::at (i);
}

template<typename T>
int ky_array<T>::find(const Type &c, int i)const
{
    for (size_t j = i; j < size(); ++j)
        if (at(j) == c)
            return j;
    return -1;
}

template<typename T>
ky_array<T> &ky_array<T>::prepend(const T &c)
{
    return prepend(&c, 1);
}

template<typename T>
ky_array<T> &ky_array<T>::prepend(const T *s, int len)
{
    if (refer ().is_shared ())
    {
        T* n = __detach_insert (0, len);
        while (len-- > 0)
        {
            if (not_construct)
                *n = *s++;
            else
                new (n) T(*s++);
            n ++;
        }
    }
    else
    {
        T* n = Layout::prepend(len);
        while (len-- > 0)
        {
            if (not_construct)
                *n = *s++;
            else
                new (n) T(*s++);
            n ++;
        }
    }
    return *this;
}

template<typename T>
ky_array<T> &ky_array<T>::prepend(const ky_array<T> &a)
{
    return prepend(a.data (), a.size ());
}

template<typename T>
ky_array<T> &ky_array<T>::append(const T &c)
{
    return append (&c, 1);
}

template<typename T>
ky_array<T> &ky_array<T>::append(const T *s, size_t len)
{
    if (refer ().is_shared ())
    {
        T* n = __detach_insert(INT_MAX, len);
        while (len-- > 0)
        {
            if (not_construct)
                *n = *s++;
            else
                new (n) T(*s++);
            n ++;
        }
    }
    else
    {
        T* n = (T*)Layout::append(len);
        while (len-- > 0)
        {
            if (not_construct)
                *n = *s++;
            else
                new (n) T(*s++);
            n ++;
        }
    }
    return *this;
}

template<typename T>
ky_array<T> &ky_array<T>::append(const ky_array<T> &a)
{
    return append(a.data (), a.size ());
}

template<typename T>
ky_array<T> &ky_array<T>::operator = (const ky_array<T> &rhs)
{
    if (header == rhs.header)
        return *this;

    __destroy_helper ();
    if (!rhs.is_null())
    {
        Layout *x = (Layout *)&rhs;
        if (x->refer ().has_shareable ())
        {
            x->addref ();
            header = x->header;
        }
        else
        {
            resize (rhs.size ());
            T* cur = (T*)Layout::begin ();
            T* t = (T*)x->begin ();
            T* to = (T*)Layout::end ();
            while(cur != to)
            {
                if (not_construct)
                    *cur = *t;
                else
                    new (cur) T(*t);
                cur ++;
                t  ++;
            }
        }
    }
    return *this;
}

template<typename T>
ky_array<T> &ky_array<T>::insert(int i, const T& c)
{
    return insert(i, &c, 1);
}

template<typename T>
ky_array<T> &ky_array<T>::insert(int i, const T *s, size_t len)
{
    if (refer ().is_shared ())
    {
        T*n = __detach_insert(i, len);
        while (len-- > 0)
        {
            if (not_construct)
                *n = *s++;
            else
                new (n) T(*s++);
            n ++;
        }
    }
    else
    {
        T* n = (T*)Layout::insert(i, len);
        while (len-- > 0)
        {
            if (not_construct)
                *n = *s++;
            else
                new (n) T(*s++);
            n ++;
        }
    }
    return *this;
}

template<typename T>
ky_array<T> &ky_array<T>::insert(int i, const ky_array<T> &a)
{
    return insert(i, a.data(), a.size());
}

template<typename T>
void ky_array<T>::remove(int i, size_t len)
{
    if (i >= size() || i < 0)
        return ;
    __detach_helper ();

    len = ((len +i) > size() ? size() -i : len);
    for (size_t j = 0; j < len; ++j)
    {
        if (not_construct)
            ;
        else
            ((T*)Layout::at (i+j))->~T();
    }
    ky_memory::array::remove (i, len);
}

template<typename T>
ky_array<T> &ky_array<T>::replace(int index, const T&c)
{
    return replace(index, 1, &c, 1);
}

template<typename T>
ky_array<T> &ky_array<T>::replace(int index, size_t len, const T *s, size_t alen)
{
    if (index < size() && index >= 0)
    {
        __detach_helper ();
        if (len > alen)
        {
            remove (index +alen, len - alen);
            len = alen;
        }
        else if (alen > len)
        {
            Layout::insert (index +alen-len, alen-len);
            len = alen;
        }

        len = ((len+index) > size() ? size()-index : len);
        T* ptr = (T*)Layout::at(index);
        do
            *ptr++ = *s++;
        while(--len);

    }
    return *this;
}

template<typename T>
ky_array<T> &ky_array<T>::replace(int index, size_t len, const ky_array<T> &s)
{
    return replace(index, len, s.data(), s.size());
}

template<typename T>
ky_array<T> &ky_array<T>::move(int from, size_t n, int to)
{
    Layout::move (from, n, to);
    return *this;
}

template<typename T>
ky_array<T> &ky_array<T>::move(int from, int to)
{
    Layout::move (from, 1, to);
    return *this;
}

// C++11
#if kyLanguage >= kyLanguage11
template<typename T>
ky_array<T>::ky_array(ky_array&& rhs):
    Layout()
{
    *this = rhs;
}

template<typename T>
ky_array<T>& ky_array<T>::operator =(ky_array&& rhs)
{
    __destroy_helper ();
    if (rhs.is_null())
        return *this;

    header = rhs.header;
    rhs.header = null();
    return *this;
}

template<typename T>
ky_array<T>::ky_array(const std::initializer_list<T> & il):
    Layout()
{
    foreach (const T & a, il)
        append (a);
}
#endif

template<typename T>
void ky_array<T>::swap(ky_array<T> &rhs)
{
    ky_atomic<LayoutHeader*> tmp = header;
    header = rhs.header;
    rhs.header = tmp;
}

template<typename T>
ky_array<T> ky_array<T>::extract( int pos, int count)const
{
    ky_array<T> tmp(this->data(), this->size());
    if (count > 0)
        tmp.remove(pos, count);
    else
        tmp.remove(pos -abs(count), abs(count));
    return tmp;
}

template<typename T>
ky_array<T> ky_array<T>::extract( int pos )const
{
    ky_array<T> tmp(this->data(), this->size());
    tmp.remove(0, pos);
    return tmp;
}

template<typename T>
ky_array<T> ky_array<T>::start( int count)const
{
    ky_array<T> tmp(this->data(), this->size());
    tmp.remove(count, this->size() - count);
    return tmp;
}

template<typename T>
ky_array<T> ky_array<T>::ending( int count)const
{
    ky_array<T> tmp(this->data(), this->size());
    tmp.remove(0, this->size() - count);
    return tmp;
}

