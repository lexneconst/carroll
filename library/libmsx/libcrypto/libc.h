
#ifndef _DLL_H_
#define _DLL_H_



#ifndef NULL
#define NULL 0
#endif


#define MAXINUM_56K_STREAM 0x15

#include <libcrt.h>

#include "aes.h"


//#include "hash/crc32.h"
//#include "hash/md2.h"
//#include "hash/md4.h"
#include "hash/md5.h"
//#include "hash/ripemd.h"
//#include "hash/sha1.h"

//#include "hash/sha256.h"
//#include "hash/sha256_sha224.h"
#include "hash/sha2.h"
//#include "hash/snefru.h"
//#include "hash/tiger.h"
//#include "hash/whirlpool.h"
#include "cipher/rijndael.h"
//#include "bfc/huffman.h"
//#include "bfc/lz.h"
//#include "bfc/rice.h"
//#include "bfc/rle.h"
//#include "bfc/shannonfano.h"
#ifdef ZEXPORT
#undef ZEXPORT
#define ZEXPORT 
#else
#define ZEXPORT
#endif

#include "bzlib/bzlib.h"

int  zcompress (    
    Bytef *des,
    uLongf *destLen,
    const Bytef *source,
    uLong sourceLen);
int  zcompress2 (
    Bytef *des,
    uLongf *destLen,
    const Bytef *source,
    uLong sourceLen,
    int level);
int  zuncompress ( 
    Bytef *dest,
    uLongf *destLen,
    const Bytef *source,
    uLong sourceLen);


typedef struct _libcstr_t
{
    char        *buffer;
    unsigned int length;
}libcstr_t;
typedef struct _libccomp_t
{
    libcstr_t   zcone;
    libcstr_t   zctwo;
    libcstr_t   zcmac;
    
    libcstr_t   zchdd; // final core
    
}libccomp;





#endif /* _DLL_H_ */
