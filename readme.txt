export LD_LIBRARY_PATH=$(pwd):LD_LIBRARY_PATH

sudo chmod 777 /dev/crlx0

#depend program
ldd ./redq

#include <dlfcn.h> // dlopen, RTLD_LAZY, dlsym
char *librarypath = "support.dylib";
    void *libhandle = dlopen(librarypath, RTLD_LAZY);
float (*km_to_deg) (float);
                km_to_deg = dlsym(libhandle, "convertKMtoDegree");
dlclose(libhandle);

echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:'${PWD}'/' >> test.run
