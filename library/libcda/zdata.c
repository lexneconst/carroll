
#include "libcda.h"

int zdata_write(char *file, char *buffer, uint32_t length, xcda_header_t **zte)
{
    xcda_header_t  *ptr = (xcda_header_t*) *zte;
    int        ret = 0;

    libmsx_encode_file(file, buffer, length, CDA_KEYS , sizeof(xcda_header_t) );

    *zte = (xcda_header_t*) ptr;
    return ret;
}

int zdata_read(char *file, char *buffer, uint32_t *len, xcda_header_t **zte)
{
    uint32_t  length = 0;
    xcda_header_t  *ptr = (xcda_header_t*) *zte;
    int        ret = 0;

    buffer = libmsx_decode_file("test.dat", &length, CDA_KEYS, sizeof(xcda_header_t) );
   
    *len = length;
    *zte = (xcda_header_t*) ptr;
    return ret;
}

