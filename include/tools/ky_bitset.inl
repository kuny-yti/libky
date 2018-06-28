

template <int tBits>
ky_bitset<tBits>::ky_bitset(uint64 v)
{
    val.ob = 0;val.v = v;
}

template <int tBits>
ky_bitset<tBits>::ky_bitset(const ky_bitset<tBits> &rhs)
{
    val.v = rhs.val.v;
    val.ob = rhs.val.ob;
}

template <int tBits>
typename ky_bitset<tBits>::BIT &ky_bitset<tBits>::BIT::operator = (eBitsets b)
{
    if (b)
        v |= ((1 << ob) & ~(0));
    else
        v &= ~((1 << ob) & ~(0));
    return *this;
}
template <int tBits>
bool ky_bitset<tBits>::BIT::operator == (eBitsets b)const
{
    return ((v >> ob) & 0x01) == b;
}
template <int tBits>
bool ky_bitset<tBits>::BIT::operator != (eBitsets b)const
{
    return ((v >> ob) & 0x01) != b;
}

template <int tBits>
ky_bitset<tBits> ky_bitset<tBits>::operator & (const ky_bitset<tBits> &rhs)const
{
    ky_bitset<tBits> out;
    out.val.v = this->val.v & rhs.val.v;
    return out;
}
template <int tBits>
ky_bitset<tBits> ky_bitset<tBits>::operator | (const ky_bitset<tBits> &rhs)const
{
    ky_bitset<tBits> out;
    out.val.v = this->val.v | rhs.val.v;
    return out;
}
template <int tBits>
ky_bitset<tBits> ky_bitset<tBits>::operator ~ ()const
{
    ky_bitset<bits> out;
    out.val.v = ~this->val.v;
    return out;
}
template <int tBits>
ky_bitset<tBits> ky_bitset<tBits>::operator ^ (const ky_bitset<tBits> &rhs)const
{
    ky_bitset<bits> out;
    out.val.v = ~this->val.v ^ rhs.val.v;
    return out;
}
template <int tBits>
ky_bitset<tBits> &ky_bitset<tBits>::operator &= (const ky_bitset<tBits> &rhs)
{
    this->val.v &= rhs.val.v;
    return *this;
}
template <int tBits>
ky_bitset<tBits> &ky_bitset<tBits>::operator |= (const ky_bitset<tBits> &rhs)
{
    this->val.v |= rhs.val.v;
    return *this;
}
template <int tBits>
ky_bitset<tBits> &ky_bitset<tBits>::operator ~ ()
{
    this->val.v = ~this->val.v;
    return *this;
}
template <int tBits>
ky_bitset<tBits> &ky_bitset<tBits>::operator ^= (const ky_bitset<tBits> &rhs)
{
    this->val.v ^= rhs.val.v;
    return *this;
}
template <int tBits>
ky_bitset<tBits> ky_bitset<tBits>::operator << (uint b)const
{
    ky_bitset<bits> out;
    out.val.v = this->val.v << b;
    return out;
}
template <int tBits>
ky_bitset<tBits> ky_bitset<tBits>::operator >> (uint b)const
{
    ky_bitset<bits> out;
    out.val.v = this->val.v >> b;
    return out;
}
template <int tBits>
ky_bitset<tBits> &ky_bitset<tBits>::operator <<= (uint b)
{
    this->val.v <<= b;
    return *this;
}
template <int tBits>
ky_bitset<tBits> &ky_bitset<tBits>::operator >>= (uint b)
{
    this->val.v >>= b;
    return *this;
}
template <int tBits>
bool ky_bitset<tBits>::operator || (const ky_bitset<tBits> &rhs)const
{
    return this->val.v || rhs.val.v;
}
template <int tBits>
bool ky_bitset<tBits>::operator && (const ky_bitset<tBits> &rhs)const
{
    return this->val.v && rhs.val.v;
}
template <int tBits>
bool ky_bitset<tBits>::operator ! ()const
{
    return !this->val.v;
}

template <int tBits>
bool ky_bitset<tBits>::operator == (const ky_bitset<tBits> &rhs)
{
    return this->val.v == rhs.val.v;
}
template <int tBits>
bool ky_bitset<tBits>::operator != (const ky_bitset<tBits> &rhs)
{
    return !this->operator == (rhs);
}
template <int tBits>
typename ky_bitset<tBits>::BIT &ky_bitset<tBits>::operator [](uint bit)
{
    static BIT sv;
    if (bit < bits)
    {
        val.ob = bit;
        return val;
    }
    return sv;
}
template <int tBits>
const typename ky_bitset<tBits>::BIT &ky_bitset<tBits>::operator [](uint bit)const
{
    static BIT sv;
    ky_bitset<tBits> *fack = (ky_bitset<tBits> *)this;
    if (bit < bits)
    {
        fack->val.ob = bit;
        return val;
    }
    return sv;
}
template <int tBits>
void ky_bitset<tBits>::set()
{
    val.v = ~(0);
}
template <int tBits>
void ky_bitset<tBits>::set(uint bit)
{
    this->operator [] (bit) = TRUE;
}
template <int tBits>
void ky_bitset<tBits>::unset()
{
    val.v = 0;
}
template <int tBits>
void ky_bitset<tBits>::unset(uint bit)
{
    this->operator [] (bit) = FALSE;
}
template <int tBits>
bool ky_bitset<tBits>::any()const
{
    return val.v & ~(0);
}
template <int tBits>
bool ky_bitset<tBits>::none()const
{
    return !any();
}
template <int tBits>
int ky_bitset<tBits>::count()const
{
    int num = 0;
    for (int i = 0; i < bits; ++i)
        if (this->operator [] (i) == TRUE)
            num++;
    return num;
}
template <int tBits>
void ky_bitset<tBits>::flip()
{
    this->operator ~ ();
}
template <int tBits>
void ky_bitset<tBits>::flip(uint bit)
{
    if (this->operator [] (bit))
        unset(bit);
    else
        set(bit);
}


template <int tBits>
ky_string ky_bitset<tBits>::to_string()const
{
    ky_string out("b");
    out.form (val.v, 2);
    return out;
}

template <int tBits>
void ky_bitset<tBits>::form_string(const ky_string &s)
{
    ky_string tmp(s);
    if (tmp[0] == ky_char('b'))
        tmp.remove(0,1);
    else if (tmp.is_empty ())
        return ;
    val.v = tmp.to_ulonglong (2);
}
template <int tBits>
void ky_bitset<tBits>::swap(ky_bitset &b)
{
     BIT tmp = b.val;
     b.val = this->val;
     this->val = tmp;
}

