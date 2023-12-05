
#include "aes.h"


 int
ssh_rijndael_init( ssh_rijndael_ctx *c, const u_char *key, const u_char *iv, int enc)
{
	//struct ssh_rijndael_ctx *c;

	//if ((c = EVP_CIPHER_CTX_get_app_data(ctx)) == NULL) {
	//	c = xmalloc(sizeof(*c));
	//	EVP_CIPHER_CTX_set_app_data(ctx, c);
	//}
    c->encrypt = enc;
	if (key != NULL) {
		//if (enc == -1)
		//	enc = ctx->encrypt;
		rijndael_set_key(&c->r_ctx, (u_char *)key,
		    256, enc);
	}
	if (iv != NULL)
		memcpy(c->r_iv, iv, RIJNDAEL_BLOCKSIZE);
	return (1);
}

 int
ssh_rijndael_cbc( ssh_rijndael_ctx *c, u_char *dest, const u_char *src,
    u_int len)
{
	//struct ssh_rijndael_ctx *c;
	u_char buf[RIJNDAEL_BLOCKSIZE];
	u_char *cprev, *cnow, *plain, *ivp;
	int i, j, blocks = len / RIJNDAEL_BLOCKSIZE;

	if (len == 0)
		return (1);
	if (len % RIJNDAEL_BLOCKSIZE)
		fatal("ssh_rijndael_cbc: bad len", __FILE__, __FUNCTION__, __LINE__);
	//if ((c = EVP_CIPHER_CTX_get_app_data(ctx)) == NULL) {
	//	error("ssh_rijndael_cbc: no context");
	//	return (0);
	//}
	if (c->encrypt) {
		cnow  = dest;
		plain = (u_char *)src;
		cprev = c->r_iv;
		for (i = 0; i < blocks; i++, plain+=RIJNDAEL_BLOCKSIZE,
		    cnow+=RIJNDAEL_BLOCKSIZE) {
			for (j = 0; j < RIJNDAEL_BLOCKSIZE; j++)
				buf[j] = plain[j] ^ cprev[j];
			rijndael_encrypt(&c->r_ctx, buf, cnow);
			cprev = cnow;
		}
		memcpy(c->r_iv, cprev, RIJNDAEL_BLOCKSIZE);
	} else {
		cnow  = (u_char *) (src+len-RIJNDAEL_BLOCKSIZE);
		plain = dest+len-RIJNDAEL_BLOCKSIZE;

		memcpy(buf, cnow, RIJNDAEL_BLOCKSIZE);
		for (i = blocks; i > 0; i--, cnow-=RIJNDAEL_BLOCKSIZE,
		    plain-=RIJNDAEL_BLOCKSIZE) {
			rijndael_decrypt(&c->r_ctx, cnow, plain);
			ivp = (i == 1) ? c->r_iv : cnow-RIJNDAEL_BLOCKSIZE;
			for (j = 0; j < RIJNDAEL_BLOCKSIZE; j++)
				plain[j] ^= ivp[j];
		}
		memcpy(c->r_iv, buf, RIJNDAEL_BLOCKSIZE);
	}
	return (1);
}
