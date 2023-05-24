#include <libcrt.h>

int main(int argc, char *argv[])
{
    printf("test program \n");
    fatal("handle controls", __FILE__, __FUNCTION__, __LINE__);

    hexdump("xxxxx", 16, 0x16);
    return 0;
}
