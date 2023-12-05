
#include "libc.h"


#define BN_BITS2    32
#define BN_BYTES	2
void bin2hex(char *md, const unsigned char *digest, int len) /* {{{ */
{
     unsigned int i,j,m;
     static char *h="0123456789ABCDEF";
     
     for (i=0,j=0; i<len,j>=0; i++,j+=2)
			{
         if(i==len) break;
         
         
           //printf("[%s] I[%d] J[%d] [%d] \n", __FUNCTION__, i, j, len); 
		/*	if ((i != 0) && (i%35 == 0))
				{
				if (BIO_write(bp,"\\\n",2) != 2) goto err;
				n+=2;
				}*/
			md[j]=h[((unsigned char)digest[i]>>4)&0x0f];
			md[j+1]=h[((unsigned char)digest[i]   )&0x0f];
			//if (BIO_write(bp,buf,2) != 2) goto err;
			//n+=2;
			//} 
			
		}
     
     for(i=len*2;i>0;i--){
        m = (unsigned int )md[i];
        if ((m >= '0') && (m <= '9')){
           break;
        }else 
        if ((m >= 'a') && (m <= 'f')){
		   break;
		}else 
        if ((m >= 'A') && (m <= 'F')){
           break;
		}else{
	       md[i] = '\0';
		}
    }
}
void hex2bin(unsigned char *digest, const char *md, int len)
{
     unsigned int i,j,m,k,n;
     k = 0;
     i = len/2;
     for (j=0; j<i; j++,k+=2)
			{
			for (n=0; n<2; n++)
				{
				m=md[k+n];
				if ((m >= '0') && (m <= '9'))
					m-='0';
				else if ((m >= 'a') && (m <= 'f'))
					m=m-'a'+10;
				else if ((m >= 'A') && (m <= 'F'))
					m=m-'A'+10;
				else
					{
				
					}
				
				//printf(" J[%d] K[%d] , N[%d] \n", j, k , n);
				 digest[j]<<=4;
				 digest[j]|=m;
				}
			}
}




