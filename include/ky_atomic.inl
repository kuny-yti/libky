#ifndef KY_ATOMIC_HH
#define KY_ATOMIC_HH

#if 1

#elif 0
template<int s>
struct atomic_base_arm
{
    template <typename T>
    static bool ref(T &_v) ;
    template <typename T>
    static bool deref(T &_v);
    template <typename T>
    static bool compare_exchange(T &_v, T ev, T nv) ;
    template <typename T>
    static bool compare_exchange(T &_v, T ev, T nv, T *cv) ;
    template <typename T>
    static T fetch_store(T &_v, T nv) ;
    static inline void atomic_pause(void) {
        kyInlineASM ("" ::: "memory");
    }

    static void memory_fence(eMemoryOrders = Memory_Acquire)
    {

    }
};
template<typename X>
struct atomic_base: public atomic_base_arm<sizeof(X)>{};


template<> template<typename T>
inline bool atomic_base_arm<4>::ref(T &_v)
{
    register T nv;
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrex %[nv], [%[_v]]\n"
                "add %[nv], %[nv], #1\n"
                "strex %[result], %[nv], [%[_v]]\n"
                "teq %[result], #0\n"
                "bne 0b\n"
                : [nv] "=&r" (nv), [result] "=&r" (result), "+m" (_v)
                : [_v] "r" (&_v)
                : "cc", "memory"
            );
    return nv != 0;
}

template<> template <typename T>
inline bool atomic_base_arm<4>::deref(T &_v)
{
    register T nv;
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrex %[nv], [%[_v]]\n"
                "sub %[nv], %[nv], #1\n"
                "strex %[result], %[nv], [%[_v]]\n"
                "teq %[result], #0\n"
                "bne 0b\n"
                : [nv] "=&r" (nv), [result] "=&r" (result), "+m" (_v)
                : [_v] "r" (&_v)
                : "cc", "memory"
            );
    return nv != 0;
}

// Relaxed
template<> template <typename T>
inline bool atomic_base_arm<4>::compare_exchange(T &_v, T ev, T nv)
{
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrex %[result], [%[_v]]\n"
                "eors %[result], %[result], %[ev]\n"
                "itt eq\n"
                "strexeq %[result], %[nv], [%[_v]]\n"
                "teqeq %[result], #1\n"
                "beq 0b\n"
                : [result] "=&r" (result), "+m" (_v)
                : [ev] "r" (ev), [nv] "r" (nv), [_v] "r" (&_v)
                : "cc"
            );
    return result == 0;
}

// Relaxed
template<> template <typename T>
inline T atomic_base_arm<4>::fetch_store(T &_v, T nv)
{
    register T ov;
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrex %[ov], [%[_v]]\n"
                "strex %[result], %[nv], [%[_v]]\n"
                "teq %[result], #0\n"
                "bne 0b\n"
                : [ov] "=&r" (ov), [result] "=&r" (result), "+m" (_v)
                : [nv] "r" (nv), [_v] "r" (&_v)
                : "cc"
            );
    return ov;
}

// Relaxed
template<> template <typename T>
inline T atomic_base_arm<4>::fetch_add(T &_v, typename atomic_add<T>::type va)
{
    register T ov;
    register T nv;
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrex %[ov], [%[_v]]\n"
                "add %[nv], %[ov], %[va]\n"
                "strex %[result], %[nv], [%[_v]]\n"
                "teq %[result], #0\n"
                "bne 0b\n"
                : [ov] "=&r" (ov), [nv] "=&r" (nv), [result] "=&r" (result), "+m" (_v)
                : [va] "r" (va * atomic_add<T>::size), [_v] "r" (&_v)
                : "cc"
            );
    return ov;
}

#  if ((kyArchitecture & kyArch_ARMv7) == kyArch_ARMv7)

template<> struct atomic_has<1> { enum{has = 1};};
template<> struct atomic_has<2> { enum{has = 1};};
template<> struct atomic_has<8> { enum{has = 1};};
template<> template<typename T>
inline bool atomic_base_arm<1>::ref(T &_v)
{
    register T nv;
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrexb %[nv], [%[_v]]\n"
                "add %[nv], %[nv], #1\n"
                "strexb %[result], %[nv], [%[_v]]\n"
                "teq %[result], #0\n"
                "bne 0b\n"
                : [nv] "=&r" (nv), [result] "=&r" (result), "+m" (_v)
                : [_v] "r" (&_v)
                : "cc", "memory"
            );
    return nv != 0;
}

template<> template <typename T>
inline bool atomic_base_arm<1>::deref(T &_v)
{
    register T nv;
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrexb %[nv], [%[_v]]\n"
                "sub %[nv], %[nv], #1\n"
                "strexb %[result], %[nv], [%[_v]]\n"
                "teq %[result], #0\n"
                "bne 0b\n"
                : [nv] "=&r" (nv), [result] "=&r" (result), "+m" (_v)
                : [_v] "r" (&_v)
                : "cc", "memory"
            );
    return nv != 0;
}

template<> template <typename T>
inline bool atomic_base_arm<1>::compare_exchange(T &_v, T ev, T nv)
{
    register T result;
    kyInlineASM
            (
                "0:\n"
                "ldrexb %[result], [%[_v]]\n"
                "eors %[result], %[result], %[ev]\n"
                "itt eq\n"
                "strexbeq %[result], %[nv], [%[_v]]\n"
                "teqeq %[result], #1\n"
                "beq 0b\n"
                : [result] "=&r" (result), "+m" (_v)
                : [ev] "r" (ev), [nv] "r" (nv), [_v] "r" (&_v)
                : "cc"
            );
    return result == 0;
}

template<> template <typename T>
inline T atomic_base_arm<1>::fetch_store(T &_v, T nv)
{
    register T ov;
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrexb %[ov], [%[_v]]\n"
                "strexb %[result], %[nv], [%[_v]]\n"
                "teq %[result], #0\n"
                "bne 0b\n"
                : [ov] "=&r" (ov), [result] "=&r" (result), "+m" (_v)
                : [nv] "r" (nv), [_v] "r" (&_v)
                : "cc"
            );
    return ov;
}

template<> template <typename T>
inline T atomic_base_arm<1>::fetch_add(T &_v, typename atomic_add<T>::type va)
{
    register T ov;
    register T nv;
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrexb %[ov], [%[_v]]\n"
                "add %[nv], %[ov], %[va]\n"
                "strexb %[result], %[nv], [%[_v]]\n"
                "teq %[result], #0\n"
                "bne 0b\n"
                : [ov] "=&r" (ov), [nv] "=&r" (nv), [result] "=&r" (result), "+m" (_v)
                : [va] "r" (va * atomic_add<T>::size), [_v] "r" (&_v)
                : "cc"
            );
    return ov;
}

template<> template<typename T>
inline bool atomic_base_arm<2>::ref(T &_v)
{
    register T nv;
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrexh %[nv], [%[_v]]\n"
                "add %[nv], %[nv], #1\n"
                "strexh %[result], %[nv], [%[_v]]\n"
                "teq %[result], #0\n"
                "bne 0b\n"
                : [nv] "=&r" (nv), [result] "=&r" (result), "+m" (_v)
                : [_v] "r" (&_v)
                : "cc", "memory"
            );
    return nv != 0;
}

template<> template <typename T>
inline bool atomic_base_arm<2>::deref(T &_v)
{
    register T nv;
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrexh %[nv], [%[_v]]\n"
                "sub %[nv], %[nv], #1\n"
                "strexh %[result], %[nv], [%[_v]]\n"
                "teq %[result], #0\n"
                "bne 0b\n"
                : [nv] "=&r" (nv), [result] "=&r" (result), "+m" (_v)
                : [_v] "r" (&_v)
                : "cc", "memory"
            );
    return nv != 0;
}

template<> template <typename T>
inline bool atomic_base_arm<2>::compare_exchange(T &_v, T ev, T nv)
{
    register T result;
    kyInlineASM
            (
                "0:\n"
                "ldrexh %[result], [%[_v]]\n"
                "eors %[result], %[result], %[ev]\n"
                "itt eq\n"
                "strexheq %[result], %[nv], [%[_v]]\n"
                "teqeq %[result], #1\n"
                "beq 0b\n"
                : [result] "=&r" (result), "+m" (_v)
                : [ev] "r" (ev), [nv] "r" (nv), [_v] "r" (&_v)
                : "cc"
            );
    return result == 0;
}

template<> template <typename T>
inline T atomic_base_arm<2>::fetch_store(T &_v, T nv)
{
    register T ov;
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrexh %[ov], [%[_v]]\n"
                "strexh %[result], %[nv], [%[_v]]\n"
                "teq %[result], #0\n"
                "bne 0b\n"
                : [ov] "=&r" (ov), [result] "=&r" (result), "+m" (_v)
                : [nv] "r" (nv), [_v] "r" (&_v)
                : "cc"
            );
    return ov;
}

template<> template <typename T>
inline T atomic_base_arm<2>::fetch_add(T &_v, typename atomic_add<T>::type va)
{
    register T ov;
    register T nv;
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrexh %[ov], [%[_v]]\n"
                "add %[nv], %[ov], %[va]\n"
                "strexh %[result], %[nv], [%[_v]]\n"
                "teq %[result], #0\n"
                "bne 0b\n"
                : [ov] "=&r" (ov), [nv] "=&r" (nv), [result] "=&r" (result), "+m" (_v)
                : [va] "r" (va * atomic_add<T>::size), [_v] "r" (&_v)
                : "cc"
            );
    return ov;
}

template<> template<typename T>
inline bool atomic_base_arm<8>::ref(T &_v)
{
    register T nv;
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrexd %[nv], %H[nv], [%[_v]]\n"
                "adds %Q[nv], %Q[nv], #1\n"
                "adc %R[nv], %R[nv], #0\n"
                "strexd %[result], %[nv], %H[nv], [%[_v]]\n"
                "teq %[result], #0\n"
                "bne 0b\n"
                : [nv] "=&r" (nv), [result] "=&r" (result), "+m" (_v)
                : [_v] "r" (&_v)
                : "cc", "memory"
            );
    return nv != 0;
}

template<> template <typename T>
inline bool atomic_base_arm<8>::deref(T &_v)
{
    register T nv;
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrexd %[nv], %H[nv], [%[_v]]\n"
                "subs %Q[nv], %Q[nv], #1\n"
                "sbc %R[nv], %R[nv], #0\n"
                "strexd %[result], %[nv], %H[nv], [%[_v]]\n"
                "teq %[result], #0\n"
                "bne 0b\n"
                : [nv] "=&r" (nv), [result] "=&r" (result), "+m" (_v)
                : [_v] "r" (&_v)
                : "cc", "memory"
            );
    return nv != 0;
}

template<> template <typename T>
inline bool atomic_base_arm<8>::compare_exchange(T &_v, T ev, T nv)
{
    register T result;
    kyInlineASM
            (
                "0:\n"
                "ldrexd %[result], %H[result], [%[_v]]\n"
                "eor %[result], %[result], %[ev]\n"
                "eor %H[result], %H[result], %H[ev]\n"
                "orrs %[result], %[result], %H[result]\n"
                "itt eq\n"
                "strexdeq %[result], %[nv], %H[nv], [%[_v]]\n"
                "teqeq %[result], #1\n"
                "beq 0b\n"
                : [result] "=&r" (result), "+m" (_v)
                : [ev] "r" (ev), [nv] "r" (nv), [_v] "r" (&_v)
                : "cc"
            );
    return uint32(result) == 0;
}

template<> template <typename T> inline
inline T atomic_base_arm<8>::fetch_store(T &_v, T nv)
{
    register T ov;
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrexd %[ov], %H[ov], [%[_v]]\n"
                "strexd %[result], %[nv], %H[nv], [%[_v]]\n"
                "teq %[result], #0\n"
                "bne 0b\n"
                : [ov] "=&r" (ov), [result] "=&r" (result), "+m" (_v)
                : [nv] "r" (nv), [_v] "r" (&_v)
                : "cc"
            );
    return ov;
}

template<> template <typename T> inline
inline T atomic_base_arm<8>::fetch_add(T &_v, typename atomic_add<T>::type va)
{
    register T ov;
    register T nv;
    register int result;
    kyInlineASM
            (
                "0:\n"
                "ldrexd %[ov], %H[ov], [%[_v]]\n"
                "adds %Q[nv], %Q[ov], %Q[va]\n"
                "adc %R[nv], %R[ov], %R[va]\n"
                "strexd %[result], %[nv], %H[nv], [%[_v]]\n"
                "teq %[result], #0\n"
                "bne 0b\n"
                : [ov] "=&r" (ov), [nv] "=&r" (nv), [result] "=&r" (result), "+m" (_v)
                : [va] "r" (va * atomic_add<T>::size), [_v] "r" (&_v)
                : "cc"
            );
    return ov;
}

#  elif ((kyArchitecture & kyArch_ARMv6) == kyArch_ARMv6)

#  elif ((kyArchitecture & kyArch_ARMv5) == kyArch_ARMv5)

#  endif

#elif kyCompiler == kyCompiler_GNUC || kyCompiler == kyCompiler_CLANG // gcc _sync_

#include <pthread.h>
template<> struct atomic_has<1>{enum{has = 1};};
template<> struct atomic_has<2>{enum{has = 1};};
template<> struct atomic_has<8>{enum{has = 1};};
template <typename X>
struct atomic_base :public atomic_generic<atomic_base<X> >
{
    typedef atomic_generic<atomic_base<X> > GenericT;
    typedef X type;

    template <typename T>
    static void memory_fence(eMemoryOrders = Memory_Acquire)
    {

    }

#if ((kyArchitecture & kyArch_IA64) != kyArch_IA64)
    template <typename T>
    static T load_acquire(const T &_v)
    {
        T t = _v;
        __sync_synchronize();
        return t;
    }

    template <typename T>
    static void store_release(T &_v, T nv)
    {
        __sync_synchronize();
        _v = nv;
    }
#endif

    // Relaxed
    template <typename T>
    static bool compare_exchange(T &_v, T ev, T nv, eMemoryOrders mo = Memory_Unknown)
    {
        if (mo == Memory_Relaxed)
            return __sync_bool_compare_and_swap(&_v, ev, nv);

        return GenericT::compare_exchange(_v, ev, nv, mo);
    }

    // Relaxed
    template <typename T>
    static bool compare_exchange(T &_v, T ev, T nv, T *cv, eMemoryOrders mo = Memory_Unknown)
    {
        if (mo == Memory_Relaxed)
        {
            bool t = __sync_bool_compare_and_swap(&_v, ev, nv);
            if (t)
                *cv = ev;
            else
                *cv = _v;
            return t;
        }
        return GenericT::compare_exchange(_v, ev, nv, cv, mo);
    }

    // Relaxed
    template <typename T>
    static T fetch_store(T &_v, T nv, eMemoryOrders mo = Memory_Unknown)
    {
        if (mo == Memory_Relaxed)
            return __sync_lock_test_and_set(&_v, nv);
        else if (mo == Memory_Release)
        {
            __sync_synchronize();
            return __sync_lock_test_and_set(&_v, nv);
        }
        return GenericT::fetch_store(_v, nv, mo);
    }

    // Relaxed
    template <typename T> static
    T fetch_add(T &_v, typename atomic_add<T>::type va, eMemoryOrders mo = Memory_Unknown)
    {
        if (mo == Memory_Relaxed)
            return __sync_fetch_and_add(&_v, va * atomic_add<T>::size);
        return GenericT::fetch_add(_v, va, mo);
    }

};
#endif

#endif // KY_ATOMIC_HH
