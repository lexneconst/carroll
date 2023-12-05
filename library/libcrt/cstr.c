#include <libcrt.h>

uint32_t cstring_create(xcstring_t** ptr)
{
	xcstring_t *ext=NULL;
do{
	ext = (cstring_t *) emalloc( sizeof(xcstring_t));
	if(ext==NULL){
		fatal("allocate memory ", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	ext->buffer=NULL;
	ext->length=0;
	
}while(0);
    *ptr = (xcstring_t *) ext;
    
	return 0;
}

uint32_t cstring_clean(xcstring_t** ptr)
{
	xcstring_t  *ext = (xcstring_t *) *ptr;
do{
	if(ext==NULL){
		fatal("segment memory ", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	
	CLEAN(ext->buffer);
	ext->length = 0;
	CLEAN(ext);
	
}while(0);
	return 0;
}

uint32_t cstring_clone(xcstring_t** ptr, xcstring_t *str)
{
	xcstring_t *ext = (xcstring_t *) *ptr;
do{
	cstring_create(&ext);
    if(ext==NULL){
    	fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
    	break;
	}
	if(str==NULL){
		fatal("segment memory ", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	if(str->length<=0){
		fatal("cstring length", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	if(str->buffer==NULL){
		fatal("cstring buffer", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	ext->length = str->length;
	ext->buffer = (char *) emalloc( ext->length+2);
	if(ext->buffer==NULL){
		fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	strcpy(ext->buffer, str->buffer);
}while(0);
    *ptr = (xcstring_t *) ext;
    
	return 0;
}
uint32_t cstring_check(xcstring_t **ptr)
{
	uint32_t   ret=0;
	cstring_t *ext = (xcstring_t *) *ptr;
do{
    if(ext==NULL){
    	fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
    	break;
	}
	if(ext->buffer==NULL){
		ret = 1;
	}
}while(0);
    *ptr = (xcstring_t *) ext;
    	
	return ret;	
}
char* cstring_str(xcstring_t *ptr)
{
	if(ptr->buffer==NULL){
		fatal("segment memory ", __FILE__, __FUNCTION__, __LINE__);
		return NULL;
	}
	return ptr->buffer;
}

uint32_t cstring_len(xcstring_t *ptr)
{
	return ptr->length;
}
uint32_t cstring_newadd(xcstring_t **ptr, xcstring_t *dst , xcstring_t *src)
{
	xcstring_t *ext = (xcstring_t *) *ptr;
do{
	//cstring_create(&ext);
    if(ext==NULL){
    	fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
    	break;
	}
	if(dst==NULL){
		fatal("segment memory ", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	if(src==NULL){
		fatal("segment memory ", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	if(dst->buffer==NULL){
		fatal("cstring buffer ", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	if(src->buffer==NULL){
		fatal("cstring buffer ", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	if(dst->length <= 0){
		fatal("cstring length ", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	if(src->length<=0){
		fatal("cstring length ", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	
	ext->length = dst->length+ src->length;
	ext->buffer = (char *) emalloc( ext->length+2);
	if(ext->buffer==NULL){
		fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	strcpy(ext->buffer,dst->buffer);
	strcat(ext->buffer,src->buffer);
	
	ext->buffer[ext->length] ='\0';
	
}while(0);
    *ptr = (xcstring_t *) ext;
    		
}
uint32_t cstring_new(xcstring_t **ptr, const char *buf)
{
	xcstring_t *ext = (xcstring_t *) *ptr;
do{
	//cstring_create(&ext);
    if(ext==NULL){
    	fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
    	break;
	}
	if(buf==NULL){
		fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	
	//CLEAN(ext->buffer);
	
	ext->length = strlen(buf);
	ext->buffer = (char *) emalloc( ext->length+6);
	if(ext->buffer==NULL){
		fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	strcpy(ext->buffer, buf);
	ext->buffer[ext->length] ='\0';
	
	//printf("::%s::%d\n", ext->buffer, ext->length);
}while(0);
    *ptr = (xcstring_t *) ext;
    	
	return 0;
}
uint32_t cstring_newpad(xcstring_t **ptr, const char *buf,const char *pad)
{
	xcstring_t *ext = (xcstring_t *) *ptr;
do{
	//cstring_create(&ext);
    if(ext==NULL){
    	fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
    	break;
	}
	if(buf==NULL){
		fatal("segment memory ", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	if(pad==NULL){
		fatal("segment memory ", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	ext->length = strlen(buf)+strlen(pad);
	ext->buffer = (char *) emalloc( ext->length+4);
	if(ext->buffer==NULL){
		fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	strcpy(ext->buffer, buf);
	strcat(ext->buffer, pad);
}while(0);
    *ptr = (xcstring_t *) ext;
    	
	return 0;	
}
uint32_t cstring_renew(xcstring_t **ptr, const char *buf)
{
	xcstring_t *ext = (xcstring_t *) *ptr;
do{
    if(ext==NULL){
    	fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
    	break;
	}
	if(buf==NULL){
		fatal("segment memory ", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	CLEAN(ext->buffer);
	
	ext->length = strlen(buf);
	ext->buffer = (char *) emalloc( ext->length+2);
	if(ext->buffer==NULL){
		fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
	strcpy(ext->buffer, buf);
	ext->buffer[ext->length]='\0';
	
}while(0);
    *ptr = (xcstring_t *) ext;
    	
	return 0;
}
uint32_t cstring_remove(xcstring_t **ptr, uint32_t len)
{
	uint32_t  i=0;
	xcstring_t *ext = (xcstring_t *) *ptr;
do{
    if(ext==NULL){
    	fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
    	break;
	}
	
	//printf("%d===%d\n", ext->length, len);
	//printf("%s\n", ext->buffer);
	do{
		//printf("::%d=%d\n", i , len);
		if(i >= len) break;
		ext->buffer[ext->length-i-1] = '\0';
		//printf("::%s\n", ext->buffer);
	    i++;
	    ext->length--;
	}while(ext->length);
	
	
	//printf("%s:\n", ext->buffer);
	
}while(0);
    *ptr = (xcstring_t *) ext;
    	
	return 0;
}
uint32_t cstring_stat(xcstring_t **ptr, const char st)
{
	uint32_t   ret=0;
	xcstring_t *ext = (xcstring_t *) *ptr;
do{
    if(ext==NULL){
    	fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
    	break;
	}
	if(ext->buffer[strlen(ext->buffer)-1] == st){
		ret = 1;
	}
}while(0);
    *ptr = (xcstring_t *) ext;
    	
	return ret;	
}
uint32_t cstring_clear(xcstring_t **ptr)
{
	uint32_t   ret=0;
	xcstring_t *ext = (xcstring_t *) *ptr;
do{
    if(ext==NULL){
    	fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
    	break;
	}
	
	memset(ext->buffer, 0, ext->length);
	ext->length = 0;
	
}while(0);
    *ptr = (xcstring_t *) ext;
    	
	return ret;	
}

uint32_t cstring_delete(xcstring_t **ptr)
{
	uint32_t   ret=0;
	xcstring_t *ext = (xcstring_t *) *ptr;
do{
    if(ext==NULL){
    	fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
    	break;
	}
	
	memset(ext->buffer, 0, ext->length);
	ext->length = 0;
    CLEAN(ext->buffer);	

}while(0);
    *ptr = (xcstring_t *) ext;
    	
	return ret;	
}

uint32_t cstring_restr(xcstring_t **ptr, char *str)
{
	uint32_t   ret=0;
	xcstring_t *ext = (xcstring_t *) *ptr;
do{
    if(ext==NULL){
    	fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
    	break;
	}
	
	memset(ext->buffer, 0, ext->length);
	ext->length = 0;
    CLEAN(ext->buffer);	
    CCNEW(ptr, str);

}while(0);
    *ptr = (xcstring_t *) ext;
    	
	return ret;	
}



