
#ifndef _PRIVATE_RIJNDAEL_H
#define _PRIVATE_RIJNDAEL_H

#define MAXKC	(256/32)
#define MAXKB	(256/8)
#define MAXNR	14

typedef unsigned char	u8;
typedef unsigned short	u16;
typedef unsigned int	u32;

#ifndef u_char 
#define u_char u8
#endif

/*  The structure for key information */
typedef struct _rijndael_ctx{
	int	decrypt;
	int	Nr;			/* key-length-dependent number of rounds */
	u32	ek[4*(MAXNR + 1)];	/* encrypt key schedule */
	u32	dk[4*(MAXNR + 1)];	/* decrypt key schedule */
} rijndael_ctx;

void	 rijndael_set_key(rijndael_ctx *, u_char *, int, int);
void	 rijndael_decrypt(rijndael_ctx *, u_char *, u_char *);
void	 rijndael_encrypt(rijndael_ctx *, u_char *, u_char *);

#endif /* _PRIVATE_RIJNDAEL_H */
