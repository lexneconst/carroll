

#include "libc.h"

const char cstream [] = "AE0125A00EF102CEA09901A45483E010";

#define AES_BLOCK_SIZE 16

u_int _secheck_bits(const char *str,u_int len)
{
     u_int  l = len;
     do{
       if((u_char )str[l] != 0x0) break;
       if(l == 0) break;
       l -= 1;
     }while(l>0);
     if(str[l] != 0x0)
        l+=1;
     
     return l;
}

unsigned int _secheck_size(unsigned int s)
{

    unsigned int n,l=s;
    l = s;
    n = s % AES_BLOCK_SIZE;
    if(n != 0){
       while(l++ ){
          if(l %AES_BLOCK_SIZE == 0) break;
       }
    }
   
    return l;
}

u_char *libccpy(const u_char *in, u_int length)
{
    u_char  *ptr=NULL;

do{
    ptr = (u_char *) emalloc( length );
    if(ptr == NULL){
       fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
       break;
    }
    memcpy(ptr, in, length);
}while(0);

    return ptr;
}


int  libc0_context_info(libc0_t *ctx)
{
    do{
              
    }while(0);
    
    return 0;
}


int  libc0_encrypt(libc0_t *c, const uint8_t *in, u_int insize)
{

    SHA256_CTX         ctx;
    MD5_CTX            context;
    u_char             key2[MD5_DIGEST_LENGTH];
    u_char             key1[SHA256_DIGEST_LENGTH];
    u_int              l=insize; 
    ///ssh_rijndael_ctx   ssh_ctx;
    ssh_rijndael_ctx    rkey;
    libccomp            zcomp;
    u_char             *inbuf=NULL;
    char                mhash[64];
    char               *rbuf=NULL;
    int                 err=0;
    
do{
    if(c == NULL){
        fatal("stack memory failed.,", __FILE__, __FUNCTION__, __LINE__);
        break;
    }
    if(in == NULL){
        fatal("buffer memory failed.,", __FILE__, __FUNCTION__, __LINE__);
        break;
    }
    if(insize <= 0){
        fatal("size of length ", __FILE__, __FUNCTION__, __LINE__);
        break;
    }
    c->ustatus = 0;
    memset(&c->mhash, 0, sizeof(libchash_t));
    memset(&c->skey, 0, sizeof(libckey_t));
    //indata= (u_char *) malloc_t( insize+32);
    //if(indata==NULL){
     //   fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
     //   break;
    //}
    
    memset(key1, 0, sizeof(key1));
    memset(key2, 0, sizeof(key2));
    
    //time();
    
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, in, l);
    SHA256_Final(key1, &ctx);
    
    memset(mhash, 0, sizeof(mhash));
    bin2hex(mhash, key1, SHA256_DIGEST_LENGTH);
    memcpy(c->mhash.hash, mhash, HASH_SIZE);
    c->mhash.length = HASH_SIZE;
    c->mhash.hash[c->mhash.length]='\0';
    
    MD5Init(&context);
    MD5Update(&context, (u_char *)in, l);
    MD5Final(key2, &context);
    
    memset(mhash, 0, sizeof(mhash));    
    bin2hex(mhash, key2, MD5_DIGEST_LENGTH);
    memcpy(c->skey.key, mhash, SKEY_SIZE);
    c->skey.length = SKEY_SIZE;
    c->skey.key[c->skey.length ] ='\0';
    
    memcpy(c->ckey.key, cstream, SKEY_SIZE);
    c->ckey.length = SKEY_SIZE;
    c->ckey.key[c->ckey.length ] ='\0';
    
    #ifdef __SS
    printf("> SHA [%s] [%d]\n", c->hash, strlen(c->hash));
    printf("> MD5 [%s] [%d]\n", c->key, strlen(c->key));
    #endif
    
    //ssh_rijndael_init(&ssh_ctx, key1, key2 , 1, 128);
    //ssh_rijndael_cbc(&ssh_ctx, indata, in, insize);
    
     //c->algo = 2;
    
     
     // char *outbuf, *retbuf;
     // zcomp->zcone.buffer=(insize*104+50)/100 + 384;
     // uLongf retsize= outsize;
     // u_int i;
      zcomp.zcone.buffer = (char *)in;
      zcomp.zcone.length = insize;
      zcomp.zctwo.length = (insize*104+50)/100 + 384;
      //printf("COMPRESS [%s] Algorithm [%d] \n", __FUNCTION__, c->algo );
      zcomp.zcmac.buffer= NULL;

      zcomp.zctwo.buffer = (char *) emalloc ( insize+1 );
      if(zcomp.zctwo.buffer == NULL){
         fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
      }else{
        
        inbuf = (char *)in;
        
        #ifdef __DBG_STREAM_
        
        hexdump(zcomp.zcone.buffer, zcomp.zcone.length, NULL);
                
        #endif
        //printf("IN [%d] \n", insize);
         
        err = zcompress2(zcomp.zctwo.buffer, (uLongf *)&zcomp.zctwo.length,
                  zcomp.zcone.buffer, zcomp.zcone.length, Z_BEST_COMPRESSION );
        if(err != Z_OK){
           fatal("compress in stream ", __FILE__, __FUNCTION__, __LINE__);
        }
        //printf("INSIZE [%8d] OUTSIZE [%8d] \n",
         //   zcomp.zcone.length, zcomp.zctwo.length);
        
        c->maxinum = zcomp.zcone.length;
        c->mininum  = zcomp.zctwo.length;
        
        //f(c->mininum>= (N56K_SIZE-16)){
        //   fatal("size of memory ", __FILE__, __FUNCTION__, __LINE__);
        //   c->ustatus = MAXINUM_56K_STREAM;
        //}else{
              
              c->vstate = _secheck_size(c->mininum);
              c->number = c->vstate + 16;
              //c->vstate -= c->mininum;
              
              //if(c->number > N56K_SIZE){
              //   fatal("size of memory" ,__FILE__, __FUNCTION__, __LINE__);
              //}else{
                  rbuf = (char *) emalloc ( c->number +1);
                  if(rbuf==NULL){
                     fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
                  }else{
                    
                    memset(c->buffer, 0, sizeof(c->buffer));
                    //printf(" digital [%d] sizeof[%d] \n", 
                    //  sizeof(libc0_t), N56K_SIZE- sizeof(libc0_t)); 
                    memcpy(rbuf, zcomp.zctwo.buffer, zcomp.zctwo.length);
                    memcpy(rbuf+ (c->number-4), "LIBC", 4);
                    //hexdump(rbuf, c->number, 0);

                    #ifdef __DBG_KEY
                     hexdump(key1, SHA256_DIGEST_LENGTH, NULL);
                     hexdump(key2, MD5_DIGEST_LENGTH, NULL);
                    #endif
                    
                     memset(&rkey,0, sizeof(ssh_rijndael_ctx)); 
                     ssh_rijndael_init(&rkey, key1, key2, 1); 
                     
                     ssh_rijndael_cbc(&rkey, c->buffer, rbuf, c->number);
                     
                     //printf("min=[%d,%d]\n", c->mininum, c->number);
                     //hexdump(c->buffer, c->number, 0);

                     #ifdef __COMP_SH2
                     memcpy(c->buffer+((c->number)+16)-4, 
                        "SHXX", 4);
                     
                     CLEAN(rbuf);
                     //if(rbuf!=NULL)
                       // free_t(rbuf), rbuf=NULL;
                        
                    rbuf = (char *) emalloc ( c->number+17 );
                    if(rbuf==NULL){
                       fatal("allocate memory failed.,", 
                          __FILE__, __FUNCTION__, __LINE__);
                       //break;
                    }else{
                       memset(rbuf, 0, c->number);
                       memcpy(rbuf, c->buffer, c->number+16);
                       
                      // hexdump(rbuf, c->number+16, NULL);
                      //printf(" ======================================== \n");
                     // memcpy(rbuf, c->buffer, c->number);
                      //printf(" =@@ [%s] [%d] @@=\n", __FILE__, __LINE__);
                      zcomp.zcmac.length = ((c->number+16) *104+50)/100 + 384;
                      zcomp.zcmac.buffer = emalloc ( c->number + 16);
                      if(zcomp.zcmac.buffer != NULL){
                                        
                        memset(zcomp.zcmac.buffer, 0, c->number+16);
                        //printf(" =@@ [%s] [%d] @@=\n", __FILE__, __LINE__);
                        
                        #ifdef __CRYPT_COMP
                        c->length  = zcomp.zcmac.length = 
                         SF_Compress(rbuf, zcomp.zcmac.buffer,  
                            c->number+16);
                        
                        #else
                        err = zcompress2(zcomp.zcmac.buffer, (uLongf*) 
                              &zcomp.zcmac.length, rbuf, c->number+16, Z_BEST_SPEED);
                        if(err != Z_OK){
                          fatal("compress in stream ", 
                            __FILE__, __FUNCTION__, __LINE__);
                        }
                        //printf(" =@@ [%s] [%d] @@=\n", __FILE__, __LINE__);
                        c->length = zcomp.zcmac.length;
                        #endif
                        
                        //zcomp.zcmac.buffer
                        kprintf("[%s] => {%8d} {%8d} \n", 
                           __FUNCTION__, zcomp.zcmac.length, c->number);
                      
                      }else{
                         fatal("allocate memory failed.," , 
                            __FILE__, __FUNCTION__, __LINE__);
                      }
                    }
                    c->rbits =  c->number ;
                    #else
                    c->length = c->rbits =  c->number ;
                    #endif
                    
                    CLEAN(rbuf);
                    //if(rbuf!=NULL)
                      // free_t(rbuf), rbuf=NULL;
                    
                    
                     
                  }
              //}  
        //}
        
        
      }
        
      
      
      
}while(0);
    //printf("[%d] \n", __LINE__);
    if( c->ustatus == 0){
        c->ustatus = 1;
    } 
    CLEAN(zcomp.zctwo.buffer);
    //if(zcomp.zctwo.buffer!=NULL)
     //     free_t(zcomp.zctwo.buffer), zcomp.zctwo.buffer=NULL;  
    CLEAN(zcomp.zcmac.buffer);
    //if(zcomp.zcmac.buffer!=NULL)
      //    free_t(zcomp.zcmac.buffer), zcomp.zcmac.buffer=NULL;  
          
    #ifdef __BUG_WL
    
    if(c->ustatus == 1){ // true bug
        fatal("stack memory failed.,", __FILE__, __FUNCTION__, __LINE__);
    }
    
    #endif
    //if(indata!=NULL)
    //  free_t(indata), indata=NULL;
    
      
     
}



int  libc0_decrypt(libc0_t *c,  uint8_t *buffer, u_int length)
{
    int                 ret=1;
    char               *ptr,*str;
    ssh_rijndael_ctx    rkey;
    u_int               size = 0;
    u_char              key2[MD5_DIGEST_LENGTH];
    u_char              key1[SHA256_DIGEST_LENGTH];
    libccomp            zcomp;
    u_int               usize = 0;
    
do{
    if(c==NULL) {
       fatal("stack memory failed.,", __FILE__, __FUNCTION__, __LINE__);
       ret=0;
       break;
    }
    
    ptr = (char *) emalloc ( c->number +1 );
    if(ptr == NULL){
       fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
       ret = 0;
       break;
    }
    memset(ptr,0, c->length);
    //memcpy(ptr, c->buffer, c->rbits);
    
    #ifdef L_SS
    hexdump(c->buffer, c->length, NULL);
    #endif
   
 
    hex2bin(key1, c->mhash.hash ,SHA256_DIGEST_LENGTH*2);
    hex2bin(key2, c->skey.key ,MD5_DIGEST_LENGTH*2);
    
    #ifdef __DBG_KEY
    hexdump(key1, SHA256_DIGEST_LENGTH, NULL);
    hexdump(key2, MD5_DIGEST_LENGTH, NULL);
    #endif

    //printf("min=[%d,%d]\n", c->mininum, c->number);
    //hexdump(c->buffer, c->number, 0);

    memset(&rkey,0, sizeof(ssh_rijndael_ctx)); 
    ssh_rijndael_init(&rkey, key1, key2, 0); 
                     
    ssh_rijndael_cbc(&rkey, ptr, c->buffer, c->number);
    //AES_set_decrypt_key(key1, 256 , &caes.rkey);
    //AES_ige_encrypt(c->buffer, ptr , c->number, &caes.rkey, 
     //             key2, AES_DECRYPT);
     
     memset(&zcomp, 0, sizeof(libccomp));
     zcomp.zcone.buffer = (char *) emalloc( c->maxinum+1);
     if(zcomp.zcone.buffer == NULL){
         fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
         ret = 0;
         break;
     }
     memset(zcomp.zcone.buffer,0, c->maxinum);
     zcomp.zcone.length = c->maxinum;
     
     #ifdef L_SS
     hexdump(ptr, c->number, NULL);
     #endif
     
     str = ptr+(c->number-4);
     if(strncmp(str, "LIBC", 4) != 0){
        fatal("true is signature " , __FILE__, __FUNCTION__, __LINE__);
        hexdump(ptr, strlen(ptr), 0);
        ret = 0;
        break;
     }   
     //c->number -= 16;
     //c->number -= c->vstate ;
     
     //if(c->mininum != c->number ){
      //  fatal("length of match ", __FILE__, __FUNCTION__, __LINE__);
      //  ret = 0;
     //   break;
     //}
     //printf(" CHECKED TRUE IS SIGNATURE \n");
     
     usize = zuncompress(zcomp.zcone.buffer, 
        (uLongf*)&zcomp.zcone.length, ptr, c->mininum);
     
     //printf("RET[%8d] SIZ[%8d] CTX[%8d] \n", 
     //      usize ,zcomp.zcone.length, c->maxinum);        
     if(zcomp.zcone.length != c->maxinum ){
         fatal("sizeof return length ", __FILE__, __FUNCTION__, __LINE__);
         ret = 0;
         break;
     }
      
     if(length != zcomp.zcone.length){
        fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
        ret = 0;
        break;
     }
     
     if(buffer==NULL){
        fatal("heap memory failed.,", __FILE__, __FUNCTION__, __LINE__);
        ret = 0;
        break;
     }
    memcpy(buffer, zcomp.zcone.buffer, zcomp.zcone.length);
    //c->buffer[c->length ] = '\0';
    //printf("[CHECK] -> S[%8d] C[%8d]\n", size, c->number);  
    //fatal("round left failed.,", __FILE__, __FUNCTION__, __LINE__);
    #ifdef _DBG_STREAM_
    hexdump(zcomp.zcone.buffer, zcomp.zcone.length , NULL);                
    kprintf("[%s] [%p] \n", __FUNCTION__, c);   
    #endif
    
    ret = 1;
}while(0);

    if(zcomp.zcone.buffer!=NULL){
       CLEAN(zcomp.zcone.buffer);
      //  free_t(zcomp.zcone.buffer), zcomp.zcone.buffer=NULL;
    }
    if(ptr!=NULL)
      CLEAN(ptr); //free_t(ptr), ptr=NULL;

    return ret;
}
