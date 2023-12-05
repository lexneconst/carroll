
#include "libalsa.h"


void _kmgcsp(const char *message, const char *func, unsigned int line, unsigned int length, uint32_t number, uint32_t per)
{
   #ifndef __INFO
   printf("[%s]=%6d, %12s (%16s) - %3d | %3d - (%d)\n", __FUNCTION__, line, func, message, per, number, length );
   #endif
   return ;
}


CRT_FUNCDECL(int, libmic_init,(struct sysnode_proc_t *str))
{
    libmic_run_state = 1;

    libcrt_snupdate(str, "/mic/0000", NULL, 0);
    return 0;
}
CRT_FUNCDECL(int, libmic_exit,(struct sysnode_proc_t *str))
{
    return 0;
}

CRT_FUNCDECL(struct sound_struct_t *,libmic_alloc,(void))
{
    struct sound_struct_t *ptr = (struct sound_struct_t*) malloc ( sizeof(struct sound_struct_t));
    do{ 
        if(ptr==NULL){ 
            fatal("allocate memory ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        //printf("%s detect allocate %d\n",__FUNCTION__, __LINE__);
        ptr->check = 0;
        ptr->block = 0;
        ptr->length = MGC_SEEK1 * SSC_STREAM_BLOCK ;
        ptr->buffer = (char *) malloc ( ptr->length );
        if(ptr->buffer==NULL){
            fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
            ptr = libmic_free(ptr);            
            break;        
        }
        //printf("%s detect allocate %d\n",__FUNCTION__, __LINE__);
        memset(ptr->buffer, '\x4e', ptr->length);
        ptr->length = 0;
    }while(0);
    return ptr;
}
CRT_FUNCDECL(struct sound_struct_t *,libmic_realloc,(struct sound_struct_t *ptr))
{
    //struct sound_struct_t *ptr = (struct sound_struct_t *) ptr;
    do{ 
        if(ptr==NULL){ 
            fatal("allocate memory ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        
        if(ptr->buffer==NULL){
            if(ptr->length ==0){
                ptr->length = MGC_SEEK1 * SSC_STREAM_BLOCK ;
            }
            ptr->buffer = (char *) realloc ( ptr->buffer, ptr->length );   
            if(ptr->buffer==NULL){
                fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
            }       
            break;        
        }
        //memset(ptr->buffer, '\4e', ptr->length);
        
    }while(0);
    return ptr;
}
CRT_FUNCDECL(int, libmic_detect,(char *buffer, unsigned int length))
{
   unsigned int i=0;
   unsigned int count=0,pt=0;

   for(i=0;i<length; i++){
        if(buffer[i]==0)
          count++;
   }
   pt = (long)(0.9*length);
   if(count > pt){
        return 0;
   }else{
        return 1;
   }
}

CRT_FUNCDECL(int, libmic_create,(struct sound_struct_t *ptr, char *buf, unsigned int len))
{
   uint32_t             i=0,j=0;
   uint32_t             count1=0,count2=0;
   uint32_t             ret=0;
   float                pt1=0, pt2=0;
   //char                astr,bstr;
   //char                *sbuf = NULL;
   //char                *ptr=NULL;
   //struct _mgc_point_t    zpy;
   unsigned char                 ndata[LIBMGC_POINT];
   //struct sound_struct_t *ptr = (struct sound_struct_t *) *pstr;
   //uint8_t                 zstr[4];
   //uint32_t                zbuf[SSC_STREAM_MAX];

do{
          
   if(ptr==NULL){
      fatal("stack memory failed.,", __FILE__,  __FUNCTION__, __LINE__) ;
      break;
   }
   if(ptr->buffer == NULL)
   {
       fatal("stack memory failed.,", __FILE__, __FUNCTION__, __LINE__);
       break;
   }
   
   //printf("<%s> %d \n", __FUNCTION__, ret); 
   //cmemset(zbuf,0 ,sizeof(zbuf));
   ndata[0]=0.333* buf[i] + (1.0-0.333);
   ndata[1]=0;
   
   #ifndef DEBUG_TEST 
   for(i=0;i<len;i+= LIBMGC_POINT){
      //for(j=0;j<LIBMGC_POINT;j++){
      //  astr=ndata[j]  = buf[i];
        //#ifndef __NOSIE__
        ndata[1]  = 0.333* buf[i] + (1.0-0.333)* ndata[0];
        //#endif
      //}
      //zbuf[ count ] = ndata[0];  
      /*#ifndef LOW_NOISE
      bstr=sbuf[ count ] = buf[i];                
      astr=sbuf[ count ] = 0.333 * 
          buf[ i ] + ( 1.0 - 0.333 ) * sbuf[ count-1 ];
      #else
          sbuf[ count ] = buf[i]; 
      #endif
     // */
      //ret = mgc_check(ndata[0]);
     // if(ret) {
      	 //break;
	  //}
      //#ifndef DEBUG_H
	  if(/*abs(buf[i]) >= 0 && abs(buf[i]) <= LIBPUBSIZEA &&*/
	     abs(ndata[1]) >= LIBPUBSIZEW && abs(ndata[1]) <= LIBPUBSIZEA ){
          //printf("%3d ", ndata[0]);
		  //if(count > (len /128)){
		  	  //printf("=======================================\n");
		  	// printf("ID( %d ) LEN ( %d ) %d \n", i, len, len/128);
		  	  //printf("=======================================\n");
		      //ret = 1;
           //   break;
		  //}
          count1+=1;
		  //printf("%5d %5d ", (uint32_t)zbuf[count], LIBPUBSIZE);
          //printf("%d ", zbuf[count]);
          //if(icount==0){
             //zpt->cxstart = i;
          //}else{
             //zpt->cxfinal = i;
          //}
          //icount += 1;
      //}//else 
      //if(/*buf[i] <= 0 && */ndata[0] <= LIBPUBSIZEW){
      	//  count2+=1;
	  }else{
	  	  //printf("%d ", ndata[0]);
	  	  count2+=1;
	  }
      //#endif
      ndata[0] = ndata[1];
   } 
   if(count1<= 0) count1++;
   if(count2<= 0) count2++;
   
   pt1 = percent(len,count1);
   pt2 = percent(len,count2);

   #ifdef DEBUG_H
   printf("<%s> %d Percent %6d, %6d - %6d | (%3d) (%3d) \n", __FUNCTION__, ret, 
    	  count1,
    	  count2,
    	  count1+count2,
		  (uint32_t)pt1,
		  (uint32_t)pt2
		  );
   #endif
   

   if((uint32_t)pt1 >= 42 && (uint32_t)pt2 <= 57){ // 45-55
   	    #ifdef DEBUG_H
         printf("<%s> %d Percent %6d, %6d - %6d | (%3d) (%3d) \n", __FUNCTION__, ret, 
    	  count1,
    	  count2,
    	  count1+count2,
		  (uint32_t)pt1,
		  (uint32_t)pt2
		  );
        #endif
   
   	    ret=1;
   }
    
   #endif
   
  
   ///printf("\n");
    
   if(ret){
    
     if(ptr->check == 0){   
        #ifdef __INFO_SPD   
        printf("-------------------------------------\n");      
        #endif
     }
     #ifdef __INFO_SPD
     printf("  +MGC_SEEK1[%d] LIBMGC_POINT[%d] SSC[%d][%d]\n", 
         MGC_SEEK1,
         LIBMGC_POINT,
         SSC_STREAM_MAX, 
         SSC_STREAM_MAX*LIBMGC_POINT
         );   
     #endif
     
     if(ptr->check == 0){
        
        memset(ptr->buffer, 0, len);
        memcpy(ptr->buffer, buf, len);
        ptr->length = len;
        //ptr->block=0;
        //ptr->block+=1;      
        #ifdef __SS_DBG_
        printf("[%s]DBG_HANDLE =INIT(%d) \n",__FUNCTION__, 
                             SSC_STREAM_MAX*ptr->block); 
        #endif
        _kmgcsp(" INIT ", __FUNCTION__, __LINE__, ptr->length, ptr->block, pt1);
        
        ptr->check = 1;
        ptr->block =1;
     }else{
        
        //ptr->block += 1;
        memcpy(ptr->buffer+ptr->length, buf, len);
        ptr->length += len;
        
        #ifdef __SS_DBG_
        printf("[%s]DBG_HANDLE =UPDATE(%d) \n",__FUNCTION__, 
                             SSC_STREAM_MAX*ptr->block); 
        #endif
        
        _kmgcsp(" UPDATE ", __FUNCTION__, __LINE__, ptr->length, ptr->block , pt1);

        ptr->block += 1;
        if(ptr->block >=  SSC_STREAM_BLOCK-1)
        {
            ptr->check = 3;
            _kmgcsp(" NEWLINE ", __FUNCTION__, __LINE__, ptr->length, ptr->block, pt1);
            goto NEWLINE;
        }
        ptr->check = 2;
     }    
   }else{
     NEWLINE:
     if(ptr->check == 1 || ptr->check == 2 || ptr->check == 3){
        //data->block += 1;
        if(ptr->check > 0 && ptr->check < 3){ 
            memcpy(ptr->buffer+ptr->length, buf, len);
            ptr->length += len;
        }
        _kmgcsp(" FINAL ", __FUNCTION__, __LINE__, ptr->length, ptr->block, pt1);
        if(ptr->length <=0 || ptr->length > MGC_SEEK1*SSC_STREAM_BLOCK ){     
            ptr->length = MGC_SEEK1 * ptr->block ;
            _kmgcsp(" DEBUG ", __FUNCTION__, __LINE__, ptr->length, ptr->block, pt1);        
        }        
        #ifdef __INFO_SPD
        printf("-------------------------------------\n");        
        #endif
        ptr->check = 0;
        //ptr->length = SSC_STREAM_MAX*ptr->block;
        if(ptr->block >= 1) ret = 2;
        
     }else{

        ptr->check = 0;
        ptr->block = 0;
        ret = 0;
       #ifdef __INFO_SPD
        printf("  -MGC_SEEK1[%d] LIBMGC_POINT[%d] SSC[%d][%d]\n", 
         MGC_SEEK1,
         LIBMGC_POINT,
         SSC_STREAM_MAX, 
         SSC_STREAM_MAX*LIBMGC_POINT
         );  
       #endif
     }
   }
   
}while(0);
 
   //if(sbuf!=NULL)
   //  free_t(sbuf), sbuf=NULL;
   //printf("<%s> %d %x\n", __FUNCTION__, ret, LIBPUBSIZE);
   //*pstr = ptr;

   return ret;
}
CRT_FUNCDECL(uint32_t,libmic_surw,(struct sound_struct_t *ptr, char *buffer, unsigned int length))
{
    uint32_t nstate = 0;    
    //struct sound_struct_t *ptr = (struct sound_struct_t *) *pstr;

    do{
        if(ptr==NULL){ 
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        if(ptr->buffer==NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;
        }

        nstate = libmic_create(ptr, buffer, length);
        //printf("Callback state [%d] \n", nstate);        
        if(nstate == 2){ // true +
         //memcpy((*ppv), rtdata , sizeof(libssc_t )); 
         //_libmgc_copyW(rtdata,&pnv->temp);
         //if(ptr!=NULL){
             //memcpy(pnv->data, r_stream, sizeof(UPBUFFER_T));
         //    if(ptr->buffer == NULL){
                 
                //ptr = libmic_empty(ptr);
         //       fatal("stack memory failed.,", __FILE__, __FUNCTION__, __LINE__);
                 
        //     }else{
                 //cmemcpy(pnv->data->buffer, r_stream->buffer, r_stream->length);
                 //pnv->data->length = r_stream->length;
                 //cmemset(data, 0, length);
        //         //printf(">> OK [%d]= [%d] length(%d)\n",MGC_SEEK1, pnv->data->length, r_stream->length);
        //     } 
        // }else{
        //    fatal("stack memory failed.,", __FILE__, __FUNCTION__, __LINE__ );
        // }
         //_libmgc_packW(pnv);

         //#ifdef DEBUG_TEST
         //printf("%s = %x, %d \n", __FUNCTION__, ptr->buffer, ptr->length);
         //#endif
         //ret = 1;
         break;
        }else if(nstate == 0){
            break; 
        }else if(nstate == 1){
            break;
        }else{
            break;
        }
    } while(0);

    //*pstr= ptr;

    //printf("memory: %x \n", ptr);
    return nstate;
}

CRT_FUNCDECL(struct sound_struct_t *, libmic_free,(struct sound_struct_t *ptr))
{
    if(ptr!=NULL){
        if(ptr->buffer!=NULL) free(ptr->buffer), ptr->buffer=NULL;
        memset(ptr, 0, sizeof(struct sound_struct_t));

        if(ptr!=NULL) free(ptr);
        ptr=NULL; 
    }
    //printf("Stack (%x) \n", ptr);
    return ptr;
}
CRT_FUNCDECL(struct sound_struct_t *, libmic_empty,(struct sound_struct_t *ptr))
{
    if(ptr!=NULL){
        //if(ptr->buffer!=NULL) free(ptr->buffer), ptr->buffer=NULL;
        //memset(ptr, 0, sizeof(struct sound_struct_t));
        memset(ptr->buffer, '\x70', ptr->length);
        ptr->length = 0;
        ptr->block = 0;
        ptr->check = 0;
        //if(ptr!=NULL) free(ptr);
        //ptr=NULL; 
    }
    //printf("Stack (%x) \n", ptr);
    return ptr;
}
CRT_FUNCDECL(uint32_t,sound_detection,(struct sound_struct_t **pstr, sound_struct_threads ctx, char *buf, unsigned int len, uint32_t *f))
{
    struct sound_struct_t *ptr= (struct sound_struct_t *) *pstr;
    //printf("detect initialized length (%d) \r\n", len);
    //return ptr;
    uint32_t result=0;
    uint32_t state = 0;
    uint32_t flags = (uint32_t *) *f;

    do{
        if(ctx!=NULL){ 
            if(flags == SOUND_BEGIN){
                ptr = libmic_alloc();
            }else if(flags == SOUND_FINAL){
                ptr = libmic_empty(ptr);
            }
            flags = SOUND_WAIT;      
            
            //if(ptr->buffer==NULL){
            //    fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            //    break;
            //}
            state = ctx(ptr, buf, len);
            if(state == 1){
                flags == SOUND_CUR;
            }
            ptr->flags = flags;
            //if(ptr!=NULL){
            if(state == 2){
                if(ptr->block > 0){ result = 1; }
            }
        }
    }while(0);
    //printf("return callback (%x) \n", ptr);
    *f = flags;
    *pstr = ptr;

    return result;
}

