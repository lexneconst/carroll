
#ifndef __CZRIJNDAEL_H
#define __CZRIJNDAEL_H


#include "libc.h"
#include "cipher/rijndael.h"

#define RIJNDAEL_BLOCKSIZE 16
typedef struct _ssh_rijndael_ctx
{
	rijndael_ctx	r_ctx;
	u_char		    r_iv[RIJNDAEL_BLOCKSIZE];
	u_int           encrypt;
}ssh_rijndael_ctx;


 int
ssh_rijndael_init( ssh_rijndael_ctx *c, const u_char *key, const u_char *iv, int enc);
 int
ssh_rijndael_cbc( ssh_rijndael_ctx *c, u_char *dest, const u_char *src,
    u_int len);
    
    
#endif /* __RIJNDAEL_H */
