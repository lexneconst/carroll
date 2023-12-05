#include <libcrt.h>

CRT_FUNCDECL(void, get_time_start,(uint32_t *min, uint32_t *sec))
{
	time_t        result=0;
	struct tm *   local=NULL;
	result = time(NULL);
	local = (struct tm *)localtime((time_t*)&result);
	*sec = local->tm_sec;
	*min = local->tm_min;
	//printf("TIME (%3d,%3d) \n", *min, *sec);
}	

CRT_FUNCDECL(uint32_t, get_time_final,(uint32_t min, uint32_t sec, uint32_t *lmin, uint32_t *lsec))
{

	uint32_t        utime=0,cmin=0, csec=0;
	uint32_t        i=1;
	time_t          result=0;
	struct tm *     local=NULL;
	//printf("== [%s] %d,%d \n", __FUNCTION__, min, sec);
	result = time(NULL);
	local = (struct tm *)localtime((time_t*)&result);
	//printf("== [%s] %d,%d \n", __FUNCTION__, min, sec);	
	*lmin = 0;
    *lsec = 0;

	do{
		if(min == 0 && sec == 0)
		{
			
			break;
		}
	    if(min == local->tm_min && sec == local->tm_sec){
	    	
	    	break;
		} 
	    if(utime+sec == local->tm_sec && local->tm_min == min){
	        //printf("utime 1 <%d,%d> <%d,%d> \n", utime+sec, local->tm_sec, local->tm_min, min);
			break;
	    } 
		if(utime+sec > 60 && local->tm_min == min+1){
			//printf("utime 2 <%d,%d> <%d,%d> \n", utime+sec, local->tm_sec, local->tm_min, min);
			break;	
		} 
		if(cmin == local->tm_min && csec == local->tm_sec){
		   //printf("utime 1 <%d,%d> <%d,%d> \n", cmin, local->tm_min, csec, local->tm_sec);
		   break;	
		}
		if(utime >= 60 ){
		   break;
		}else{
		   //printf("%s > (%d)(%d) %d, %d, %d, %d \n", __FUNCTION__, i, utime, min, sec, local->tm_min, local->tm_sec );
		   if(i > 60) break;
		   if(utime ==0) i+=1;
		}
		
	   if(local->tm_sec > sec){
	    if(csec >= 60){
	         cmin+=1; csec = 0;
		  }else{
		     csec ++;
	      }
		  utime ++;
	   }
	   if(local->tm_sec< sec)
	      if(local->tm_min > min)
	         utime ++;
	   
	}while(i);
	
	//printf("Done [%s] %d,%d \n", __FUNCTION__, min, sec);	  
	*lmin = cmin;
	*lsec = csec;
	// printf("TIME: %d, %d,  === %d \n", cmin, csec, utime );
	return utime;
}

