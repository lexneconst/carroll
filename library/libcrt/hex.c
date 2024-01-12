#include <libcrt.h>

CRT_FUNCDECL(void, zhexdump,(const unsigned char *src, size_t len, unsigned long fp))
{
	static const unsigned char hex[]	= "0123456789abcdef";
	static const unsigned char tmpl[]	= "                                                    |                |\n";
	unsigned char ln[sizeof tmpl];
	const unsigned char *sp, *se;
	unsigned char *h, *g;
	unsigned i, n;
	unsigned int id=0;

do{
    if(len <= 0) break;
    if(src==NULL) break;
    
	sp	= src;
	se	= sp + len;

	while (sp < se) {
		printf("%08x", fp+id);
		id+=16;
		
		memcpy(ln, tmpl, sizeof ln);
		
		h	= &ln[2];
		g	= &ln[53];

		for (n = 0; n < 2; n++) {
			for (i = 0; i < 8 && se - sp > 0; i++, sp++) {
				h[0]	= hex[0x0f & (*sp >> 4)];
				h[1]	= hex[0x0f & (*sp >> 0)];
				h	+= 3;

				*g++	= (isgraph(*sp))? *sp : '.';
			}

			h++;
		}
        printf("%s", ln);
		//fputs((char *)ln, fp);
	}


}while(0);

	return /* void */;
} /* hexdump() */


