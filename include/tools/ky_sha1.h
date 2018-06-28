#ifndef BS_SHA1_H
#define BS_SHA1_H

#include "bs_define.h"
#include "bs_string.h"

static const uint Sha1DigestBit = 5;  /* number of 32bit integers per SHA1 digest */
static const uint Sha1BlockBit = 16;  /* number of 32bit integers per SHA1 block */
static const uint Sha1BlockByte = Sha1BlockBit * 4;

class bs_sha1
{
public:
    bs_sha1();
    ~bs_sha1();

    inline void update(const bs_string &s){update ((uint8*)s.data (), s.count ());}
    bs_byte final();
    void reset();

private:
    void update(const uint8* dat, uint32 uLen);
    void transform(uint32 block[Sha1BlockByte]);

    uint32  _count;
    uint8   _digest[Sha1DigestBit];
    uint8   _buffer[Sha1BlockByte];
};

#endif // BS_SHA1_H
