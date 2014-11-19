#ifndef __BITSET_H__
#define __BITSET_H__


typedef unsigned long long ull;


#define ull_bits (sizeof(ull) * 8)


#define DEFINE_BITSET(name, bits) ull name[(bits + ull_bits) / ull_bits]


#define BITSET_GETBIT(name, k) \
    (!!((name[k / ull_bits]) & (1 << (k % ull_bits))))


#define BITSET_SETBIT(name, k, val)                         \
    do {                                                    \
        if (val) {                                          \
            name[k / ull_bits] |= (1 << (k % ull_bits));    \
        } else {                                            \
            name[k / ull_bits] &= (~(1 << (k % ull_bits))); \
        }                                                   \
    } while (0)

#endif

