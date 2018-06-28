#ifndef ky_STREAM_INL
#define ky_STREAM_INL

template <typename OpeT>
ky_stream<OpeT>::ky_stream(const ky_byte &ay):
    impl(kyAsCast(tStreamDatas*)(this))
{
    if (!impl)
        return ;
    impl->border = (kyArchEndian);
    impl->array = ((ky_byte*)&ay);

    int bv = *(eByteOrders*)impl->array->data();
    if (bv == kyBigEndian)
        impl->border = kyBigEndian;
    else if (bv == kyLittleEndian)
        impl->border = kyLittleEndian;
    else
        impl->array = NULL;

    if (impl->array)
        impl->array->remove(0, sizeof(eByteOrders));
}
template <typename OpeT>
ky_stream<OpeT>::ky_stream(ky_byte *ay):
    impl(kyAsCast(tStreamDatas*)(this))
{
    if (!impl)
        return ;
    impl->border = (kyArchEndian);
    impl->array = (ay);
    if (impl->array)
        impl->put((char*)&impl->border, sizeof(eByteOrders));
}
template <typename OpeT>
ky_stream<OpeT>::~ky_stream(){impl = NULL;}
template <typename OpeT>
size_t ky_stream<OpeT>::size()const
{
    return impl && impl->array ? impl->array->size():0;
}
template <typename OpeT>
ky_stream<OpeT> & operator << (ky_stream<OpeT> &in, const ky_stream<OpeT> & v)
{
    size_t vs = v.size();
    if (vs && v.impl)
    {
        in.put(&vs, sizeof(size_t));
        char *tmpbuf = (char*)v.take(&vs);
        in.put(tmpbuf, vs);
    }
    return in;
}

template <typename OpeT>
ky_stream<OpeT> & operator >> (ky_stream<OpeT> &out, ky_stream<OpeT> & v)
{
    if (out.impl && out.size() && v.impl)
    {
        int len = sizeof(size_t);
        size_t vs = *(size_t*)out.take(&len);
        v.put(&vs, len);

        char *vd = (char*)out.take(&vs);
        v.put(vd, vs);
    }
    return out;
}

template <typename OpeT>
ky_stream<OpeT> &operator << (ky_stream<OpeT> &in, const ky_byte &v)
{
    size_t vs = v.size();
    in.put(&vs, sizeof(size_t));
    in.put(v.data(), vs);
    return in;
}
template <typename OpeT>
ky_stream<OpeT> &operator >> (ky_stream<OpeT> &out, ky_byte &v)
{
    if (out.impl && out.size())
    {
        int len = sizeof(size_t);
        len = *(size_t*) out.take(&len);
        char *buf = (char*)out.take(&len);
        v.append((ky_byte::Type*)buf, len);
    }
    return out;
}


#endif // ky_STREAM_INL
