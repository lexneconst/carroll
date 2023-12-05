
#include <libcrt.h>

#ifndef MD5_C

#define MD5_DIGEST_LENGTH  16
/* typedef a 32-bit type */


/* Data structure for MD5 (Message-Digest) computation */
typedef struct {
  UINT4 i[2];                   /* number of _bits_ handled mod 2^64 */
  UINT4 buf[4];                                    /* scratch buffer */
  unsigned char in[64];                              /* input buffer */
  //unsigned char digest[16];     /* actual digest after MD5Final call */
} MD5_CTX;

void MD5Init (MD5_CTX *);
void MD5Update (MD5_CTX *,unsigned char *,unsigned int);
void MD5Final (unsigned char *,MD5_CTX *);


#endif

void MD5Transform(UINT4 *,UINT4 *);
