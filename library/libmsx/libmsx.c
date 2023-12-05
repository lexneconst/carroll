#include "libmsx.h"



char *libmsx_encode(const char *buffer, uint32_t len, char *keys, uint32_t *length)
{
    //char design[SHA256_BLOCK_LENGTH];
    SHA256_CTX  sha_ctx;
    //*length = strlen(buffer);
    libc0_t  *ctx = (libc0_t*) emalloc( sizeof(libc0_t));
    if(ctx==NULL){
        fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
        return NULL;    
    }

    SHA256_Init(&sha_ctx);
    SHA256_Update(&sha_ctx, LEXNE_CERT_PRIVATE_KEY, strlen(LEXNE_CERT_PRIVATE_KEY));
    SHA256_Final(ctx->private_key, &sha_ctx);

    SHA256_Init(&sha_ctx);
    SHA256_Update(&sha_ctx, keys, sizeof(keys));
    SHA256_Final(ctx->public_key, &sha_ctx);

    ctx->buffer = (char *) emalloc( len+128 );
    if(ctx->buffer == NULL){
        fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
        CLEAN(ctx);
        return NULL;    
    }
    memset(ctx->buffer,0, len);
    //if(strcmp(keys,ctx->private_key)!=0){
    //     fatal("private key not match ", __FILE__, __FUNCTION__, __LINE__);
    //     return NULL;
    //}
    ctx->s0 = 'M';
    ctx->s1 = 'S';
    ctx->s2 = 'X';
    libc0_encrypt(ctx, buffer, len);
    *length = ctx->number + sizeof(libc0_t) ;
    
    return (char *)ctx;
}
char *libmsx_decode(const char *buffer, uint32_t len, char *keys, uint32_t *length)
{    
    char *ptr =NULL;
    char private_keys[SHA256_DIGEST_LENGTH], public_keys[SHA256_DIGEST_LENGTH];
    SHA256_CTX  sha_ctx;
    char md1[SHA256_DIGEST_LENGTH],md2[SHA256_DIGEST_LENGTH];

    libc0_t  *ctx= (libc0_t*)buffer;
    
    SHA256_Init(&sha_ctx);
    SHA256_Update(&sha_ctx, LEXNE_CERT_PRIVATE_KEY, strlen(LEXNE_CERT_PRIVATE_KEY));
    SHA256_Final(private_keys, &sha_ctx);

    SHA256_Init(&sha_ctx);
    SHA256_Update(&sha_ctx, keys, sizeof(keys));
    SHA256_Final(public_keys, &sha_ctx);

    memset(md1,0, SHA256_DIGEST_LENGTH);
    memset(md2,0, SHA256_DIGEST_LENGTH);

    bin2hex(md1, private_keys, SHA256_DIGEST_LENGTH);
    bin2hex(md2, ctx->private_key, SHA256_DIGEST_LENGTH);
    
    //printf("md1[%s]\n", md1);
    //printf("md2[%s]\n", md2);

    if(strncmp(public_keys, ctx->public_key, SHA256_DIGEST_LENGTH)!=0){
         fatal("public key not match ", __FILE__, __FUNCTION__, __LINE__);
         return NULL;
    }

    if(strncmp(private_keys,ctx->private_key,SHA256_DIGEST_LENGTH )!=0){
         fatal("private key not match ", __FILE__, __FUNCTION__, __LINE__);
         return NULL;
    }

    if(ctx->s0 != 'M' || ctx->s1 != 'S' || ctx->s2 != 'X'){
         fatal("file type not match ", __FILE__, __FUNCTION__, __LINE__);
         return NULL;
    }

    ptr = (char *) emalloc( ctx->maxinum );
    if(ptr==NULL){
        fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
        return NULL;    
    }
    libc0_decrypt(ctx, ptr, ctx->maxinum);
    *length = ctx->maxinum;
    return ptr;
}

