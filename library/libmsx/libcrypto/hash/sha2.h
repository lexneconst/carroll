
#include <libcrt.h>

#if !defined(HAVE_EVP_SHA256) && !defined(HAVE_SHA256_UPDATE)

/*** SHA-256/384/512 Various Length Definitions ***********************/
#define SHA256_BLOCK_LENGTH		64
#define SHA256_DIGEST_LENGTH		32
#define SHA256_DIGEST_STRING_LENGTH	(SHA256_DIGEST_LENGTH * 2 + 1)
#define SHA384_BLOCK_LENGTH		128
#define SHA384_DIGEST_LENGTH		48
#define SHA384_DIGEST_STRING_LENGTH	(SHA384_DIGEST_LENGTH * 2 + 1)
#define SHA512_BLOCK_LENGTH		128
#define SHA512_DIGEST_LENGTH		64
#define SHA512_DIGEST_STRING_LENGTH	(SHA512_DIGEST_LENGTH * 2 + 1)


/*** SHA-256/384/512 Context Structures *******************************/
typedef struct _SHA256_CTX {
	u_int32_t	state[8];
	u_int64_t	bitcount;
	u_int8_t	buffer[SHA256_BLOCK_LENGTH];
} SHA256_CTX;
typedef struct _SHA512_CTX {
	u_int64_t	state[8];
	u_int64_t	bitcount[2];
	u_int8_t	buffer[SHA512_BLOCK_LENGTH];
} SHA512_CTX;

#if 0
typedef SHA512_CTX SHA384_CTX;
#endif

void SHA256_Init(SHA256_CTX *);
void SHA256_Transform(u_int32_t state[8], const u_int8_t [SHA256_BLOCK_LENGTH]);
void SHA256_Update(SHA256_CTX *, const u_int8_t *, size_t);
void SHA256_Pad(SHA256_CTX *);
void SHA256_Final(u_int8_t [SHA256_DIGEST_LENGTH], SHA256_CTX *);
char *SHA256_End(SHA256_CTX *, char *);
char *SHA256_File(const char *, char *);
char *SHA256_FileChunk(const char *, char *, off_t, off_t);
char *SHA256_Data(const u_int8_t *, size_t, char *);

#if 0
void SHA384_Init(SHA384_CTX *);
void SHA384_Transform(u_int64_t state[8], const u_int8_t [SHA384_BLOCK_LENGTH]);
void SHA384_Update(SHA384_CTX *, const u_int8_t *, size_t);
void SHA384_Pad(SHA384_CTX *);
void SHA384_Final(u_int8_t [SHA384_DIGEST_LENGTH], SHA384_CTX *);
char *SHA384_End(SHA384_CTX *, char *);
char *SHA384_File(const char *, char *);
char *SHA384_FileChunk(const char *, char *, off_t, off_t);
char *SHA384_Data(const u_int8_t *, size_t, char *);
#endif /* 0 */

void SHA512_Init(SHA512_CTX *);
void SHA512_Transform(u_int64_t state[8], const u_int8_t [SHA512_BLOCK_LENGTH]);
void SHA512_Update(SHA512_CTX *, const u_int8_t *, size_t);
void SHA512_Pad(SHA512_CTX *);
void SHA512_Final(u_int8_t [SHA512_DIGEST_LENGTH], SHA512_CTX *);
char *SHA512_End(SHA512_CTX *, char *);
char *SHA512_File(const char *, char *);
char *SHA512_FileChunk(const char *, char *, off_t, off_t);
char *SHA512_Data(const u_int8_t *, size_t, char *);

#endif /* !defined(HAVE_EVP_SHA256) && !defined(HAVE_SHA256_UPDATE) && \
    (OPENSSL_VERSION_NUMBER >= 0x00907000L) */
    
