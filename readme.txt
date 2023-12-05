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


mkdir hello 
mkdir hello\DEBIAN
touch hello\DEBIAN\control


Package: helloworld
Version: 1.0
Maintainer: Your-Name
Architecture: all
Description: hello world

dpkg-deb --build hello

tar -czvf casenium_carroll.tar.gz casenium_carroll
.
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

