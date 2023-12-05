
#include "libc.h"

int  zcompress2 (dest, destLen, source, sourceLen, level)
    Bytef *dest;
    uLongf *destLen;
    const Bytef *source;
    uLong sourceLen;
    int level;
{
    z_stream stream;
    int err;

    stream.next_in = ( Bytef *)source;
    stream.avail_in = (uInt)sourceLen;
#ifdef MAXSEG_64K
    /* Check for source > 64K on 16-bit machine: */
    if ((uLong)stream.avail_in != sourceLen) return Z_BUF_ERROR;
#endif
    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;
    
    //printf("stream.avail_out:[%d=%d]\n", stream.avail_out, *destLen);
    //(uLong)
    if (stream.avail_out != (uInt)*destLen){ 
       fatal("stream avail out ", __FILE__, __FUNCTION__, __LINE__);
       return Z_BUF_ERROR;
    }
    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;
    stream.opaque = (voidpf)0;

    err = deflateInit(&stream, level);
    if (err != Z_OK){ 
       fatal("deflateInit in function ", __FILE__, __FUNCTION__, __LINE__);
       return err;
    }
    err = deflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        deflateEnd(&stream);
        
        fatal("deflate stream endline ", __FILE__, __FUNCTION__, __LINE__);
        return err == Z_OK ? Z_BUF_ERROR : err;
    }
    //printf("> stream.total_out [%d] \n", stream.total_out);
    
    *destLen = stream.total_out;

    err = deflateEnd(&stream);
    return err;
}
int  zcompress (dest, destLen, source, sourceLen)
    Bytef *dest;
    uLongf *destLen;
    const Bytef *source;
    uLong sourceLen;
{
    return zcompress2(dest, destLen, source, sourceLen, Z_DEFAULT_COMPRESSION);
}


///////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////

int  zuncompress (dest, destLen, source, sourceLen)
    Bytef *dest;
    uLongf *destLen;
    const Bytef *source;
    uLong sourceLen;
{
    z_stream stream;
    int err;

    stream.next_in = ( Bytef *)source;
    stream.avail_in = (uInt)sourceLen;
    /* Check for source > 64K on 16-bit machine: */
    if ((uLong)stream.avail_in != sourceLen){
         fatal("stream avail in ", __FILE__, __FUNCTION__, __LINE__);
         return Z_BUF_ERROR;
    }
    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;
    if ((uLong)stream.avail_out != *destLen){ 
        fatal("stream avail out ", __FILE__, __FUNCTION__, __LINE__);
        return Z_BUF_ERROR;
    }
    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;

    err = inflateInit(&stream);
    if (err != Z_OK){ 
       fatal("inflateInit ", __FILE__, __FUNCTION__, __LINE__);
       return err;
    }
    err = inflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        inflateEnd(&stream);
        if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0)){
            fatal("inflateEnd ", __FILE__, __FUNCTION__, __LINE__);
            return Z_DATA_ERROR;
        }
        return err;
    }
    *destLen = stream.total_out;

    err = inflateEnd(&stream);
    return err;
}
