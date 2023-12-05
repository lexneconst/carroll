#include "liblog.h"

char path_file[256];

int l00_libmod_init(char *path_dev, struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str)
{
    //printf("%s:: %s \n", __FUNCTION__, path_dev);

    time_t        result=0;
	struct tm *   local=NULL;
    char buffer[256];
    unsigned int length = 0;

	result = time(NULL);
	local = (struct tm *)localtime((time_t*)&result);

	sprintf(buffer, "%02d%02d%04d%02d%02d%02d",
        local->tm_mday, local->tm_mon+1, local->tm_year+1900, 
        local->tm_hour, local->tm_min, local->tm_sec);
            
    strcpy(path_file, path_dev);
    strcat(path_file, "/");
    strcat(path_file, buffer);
    strcat(path_file, ".log");
    length = strlen(path_file);

    path_file[length] = '\0';
    
    #ifdef DEBUG_TEST
    printf("%s:: %s \n", __FUNCTION__, path_file);
    #endif

    libcrt_create(path_file);

    do{
        
    }while(0);
    return 0;
}
int l00_libmod_exit(struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str)
{

    return 0;
}
int l00_libmod_update( char *buffer, unsigned int length)
{   
    printf("%s:: %x , %d \n", __FUNCTION__, buffer, length);
    do{
        if(buffer==NULL || length <= 0) break;

        libcrt_update(path_file, buffer, length);
        libcrt_update(path_file, "\r\n", 2);

    }while(0);
    return 0;
}
int l00_libmod_find(char *str)
{
    return 0;
}
int l00_libmod_xread(unsigned int id, cstring_t *str)
{
    return 0;
}
int l00_libmod_xwrite(unsigned int id, cstring_t *str)
{
    return 0;
}
int l00_libmod_xfree(cstring_t *str)
{
    return 0;
}

