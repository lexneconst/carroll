
PROJECTNAME=white_queen
PROJECTCODE=carroll

PROJECT_DIR=$(shell pwd)

MAJOR=1
MINOR=10

XBUILD_DATE="10/04/2023"
XBUILD_TIME="02:04:31"

XBUILD_FILE= $(PROJECT_DIR)/version.txt
XBUILD_DATA:= $(shell echo $$(($$(cat $(XBUILD_FILE))+1)))
XBUILD_VER=echo $(XBUILD_DATA) > $(XBUILD_FILE)

RELEASE=$(XBUILD_DATA)

XBUILD_ALL=build.cmd

LIBC=-lasound
CC= gcc

LIBERR= -Werror=unused-const-variable=0
DFLAGS=-c -Wall -fpic
LIBDBG= -O0 -ggdb3 -g
CFLAGS= $(LIBDBG) -DEXEC_PROC=1 $(LIBERR)
LIBCRT= -lcrt0 -pthread -lasound -ldl -lm
LIBRMD= -lrmc0 -ldev0 -lcda0 -lsnd0
LIBS0 = -lmic0 $(LIBCRT)

LIBMOD_EXP=module

SHELL_EXT = mo
SHELLW_EXT = cb
LIBC_EXT  = so

LIBCRTPROJ=libcrt0.$(LIBC_EXT)
LIBMICPROJ=libmic0.$(LIBC_EXT)
LIBRMCPROJ=librmc0.$(LIBC_EXT)
LIBDEVPROJ=libdev0.$(LIBC_EXT)
LIBCDAPROJ=libcda0.$(LIBC_EXT)
LIBSNDPROJ=libsnd0.$(LIBC_EXT)
LIBQTWPROJ=libqtw0.$(LIBC_EXT)

PROJECT=xbin
PROJECTM=redw
PROJECTO=offw
PROJECTT=testw

MODDBI0ROJ=dbi0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODSEC0ROJ=sec0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODGAT0ROJ=gat0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODCOR0ROJ=cor0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODLOG0ROJ=log0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODDEV0ROJ=dev0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODICE0ROJ=ice0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODCTL0ROJ=ctl0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODVID0ROJ=vid0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODBIO0ROJ=bio0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODHOL0ROJ=hol0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODDBO0ROJ=dbo0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODSTK0ROJ=stk0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODMOD0ROJ=mod0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODPOP0ROJ=pop0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODRWX0ROJ=rwx0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODTHI0ROJ=thi0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODMAP0ROJ=map0-$(MAJOR).$(MINOR).$(RELEASE).$(SHELL_EXT)
MODBCDXROJ=bcdx-$(MAJOR).$(MINOR).$(RELEASE).$(SHELLW_EXT)

OUTPUT_DIR=bin
MODULE_DIR=module

all: xall $(XBUILD_ALL) $(LIBCRTPROJ) $(LIBMICPROJ) $(LIBRMCPROJ) $(LIBQTWPROJ) $(LIBDEVPROJ) $(LIBCDAPROJ) $(LIBSNDPROJ) $(PROJECT) $(PROJECTM) $(PROJECTO) $(PROJECTT) $(MODDBI0ROJ) $(MODSEC0ROJ) $(MODGAT0ROJ) $(MODCOR0ROJ) $(MODLOG0ROJ) $(MODDEV0ROJ) $(MODICE0ROJ) $(MODCTL0ROJ) $(MODVID0ROJ) $(MODBIO0ROJ) $(MODHOL0ROJ) $(MODDBO0ROJ) $(MODSTK0ROJ) $(MODMOD0ROJ) $(MODPOP0ROJ) $(MODRWX0ROJ) $(MODTHI0ROJ) $(MODMAP0ROJ) $(MODBCDXROJ)

##	$(CC) -o $(PROJECT) $(PROJECT).c $(LIBC)

$(XBUILD_ALL): 
	$(XBUILD_VER)
###################################################################################################################################################


#                                      LIBRARY PROJECT


###################################################################################################################################################


OBJDIRCRT_SRC=libcrt
OBJLIBCRT_SRC=$(shell find $(OBJDIRCRT_SRC) -name '*.c')
OBJLIBCRT_OBJ=$(OBJLIBCRT_SRC:%.c=%.o)

%.o:%.c
	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ $< 

#	echo $@
#	rm -rf $@    
#OBJLIBCRT0=libcrt/main.o libcrt/mod.o libcrt/md5.o libcrt/sha2.o libcrt/cache.o libcrt/logs.o libcrt/ctime.o libcrt/sysnode.o
#libcrt/sysnode.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libcrt/sysnode.c
#libcrt/ctime.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libcrt/ctime.c
#libcrt/cache.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libcrt/cache.c
#libcrt/logs.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libcrt/logs.c
#libcrt/md5.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libcrt/md5.c
#libcrt/sha2.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libcrt/sha2.c
#libcrt/mod.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libcrt/mod.c
#libcrt/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libcrt/main.c
$(LIBCRTPROJ): $(OBJLIBCRT_OBJ)
	$(CC) -shared $(OBJLIBCRT_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR) -L"$(OUTPUT_DIR)/$(OUTOUT_DIR)" $(CFLAGS)  -o $(OUTPUT_DIR)/$@

OBJDIRMIC_SRC=libmic
OBJLIBMIC_SRC=$(shell find $(OBJDIRMIC_SRC) -name '*.c')
OBJLIBMIC_OBJ=$(OBJLIBMIC_SRC:%.c=%.o)


#OBJLIBMIC0=libmic/main.o\
#    libmic/detection.o
#libmic/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmic/main.c
#libmic/detection.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmic/detection.c
$(LIBMICPROJ): $(OBJLIBMIC_OBJ)
	$(CC) -shared $(OBJLIBMIC_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR) -L"$(OUTPUT_DIR)/$(OUTOUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$@


OBJDIRRMC_SRC=librmc
OBJLIBRMC_SRC=$(shell find $(OBJDIRRMC_SRC) -name '*.c')
OBJLIBRMC_OBJ=$(OBJLIBRMC_SRC:%.c=%.o)

#OBJLIBRMC0=librmc/main.o	
#librmc/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ librmc/main.c
$(LIBRMCPROJ): $(OBJLIBRMC_OBJ)
	$(CC) -shared $(OBJLIBRMC_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR) -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$@

OBJDIRDEV_SRC=libdev
OBJLIBDEV_SRC=$(shell find $(OBJDIRDEV_SRC) -name '*.c')
OBJLIBDEV_OBJ=$(OBJLIBDEV_SRC:%.c=%.o)
#OBJLIBDEV0=libdev/main.o	
#libdev/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libdev/main.c
$(LIBDEVPROJ): $(OBJLIBDEV_OBJ)
	$(CC) -shared $(OBJLIBDEV_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$@

OBJDIRCDA_SRC=libcda
OBJLIBCDA_SRC=$(shell find $(OBJDIRCDA_SRC) -name '*.c')
OBJLIBCDA_OBJ=$(OBJLIBCDA_SRC:%.c=%.o)
#OBJLIBCDA0=libcda/main.o	
#libcda/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libcda/main.c
$(LIBCDAPROJ): $(OBJLIBCDA_OBJ)
	$(CC) -shared $(OBJLIBCDA_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR) -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$@

OBJDIRSND_SRC=libsnd
OBJLIBSND_SRC=$(shell find $(OBJDIRSND_SRC) -name '*.c')
OBJLIBSND_OBJ=$(OBJLIBSND_SRC:%.c=%.o)
#OBJLIBSND0=libsnd/main.o	
#libsnd/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libsnd/main.c
$(LIBSNDPROJ): $(OBJLIBSND_OBJ)
	$(CC) -shared $(OBJLIBSND_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR) -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$@


OBJDIRQTW_SRC=libqtw
OBJLIBQTW_SRC=$(shell find $(OBJDIRQTW_SRC) -name '*.c')
OBJLIBQTW_OBJ=$(OBJLIBQTW_SRC:%.c=%.o)
#OBJLIBSND0=libsnd/main.o	
#libsnd/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libsnd/main.c
$(LIBQTWPROJ): $(OBJLIBQTW_OBJ)
	$(CC) -shared $(OBJLIBQTW_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR) -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$@
###################################################################################################################################################


#                                      EXEC PROJECT


###################################################################################################################################################
OBJDIRRED_SRC=reds
OBJLIBRED_SRC=$(shell find $(OBJDIRRED_SRC) -name '*.c')
OBJLIBRED_OBJ=$(OBJLIBRED_SRC:%.c=%.o)
#OBJREDS=reds/main.o	
#reds/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ reds/main.c
$(PROJECT): $(OBJLIBRED_OBJ)
	$(CC) $(OBJLIBRED_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) $(LIBS0) $(LIBRMD) -o $(OUTPUT_DIR)/$@

OBJDIRUDS_SRC=redm
OBJLIBUDS_SRC=$(shell find $(OBJDIRUDS_SRC) -name '*.c')
OBJLIBUDS_OBJ=$(OBJLIBUDS_SRC:%.c=%.o)
#OBJREDM=redm/main.o	
#redm/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ redm/main.c
$(PROJECTM): $(OBJLIBUDS_OBJ)
	$(CC) $(OBJLIBUDS_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) $(LIBS0) $(LIBRMD) -o $(OUTPUT_DIR)/$@

OBJDIROFF_SRC=offs
OBJLIBOFF_SRC=$(shell find $(OBJDIROFF_SRC) -name '*.c')
OBJLIBOFF_OBJ=$(OBJLIBOFF_SRC:%.c=%.o)
#OBJOFFW=offs/main.o	
#offs/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ offs/main.c
$(PROJECTO): $(OBJLIBOFF_OBJ)
	$(CC) $(OBJLIBOFF_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) $(LIBS0) $(LIBRMD) -o $(OUTPUT_DIR)/$@

OBJDIRTEW_SRC=test
OBJLIBTEW_SRC=$(shell find $(OBJDIRTEW_SRC) -name '*.c')
OBJLIBTEW_OBJ=$(OBJLIBTEW_SRC:%.c=%.o)
#OBJTESTW=test/main.o	
#test/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ test/main.c
$(PROJECTT): $(OBJLIBTEW_OBJ)
	$(CC) $(OBJLIBTEW_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) $(LIBS0) $(LIBRMD) -o $(OUTPUT_DIR)/$@
#$@
###################################################################################################################################################


#                                      MODULE PROJECT


###################################################################################################################################################
OBJDIRDBI00_SRC=$(LIBMOD_EXP)/dbi0
OBJLIBDBI00_SRC=$(shell find $(OBJDIRDBI00_SRC) -name '*.c')
OBJLIBDBI00_OBJ=$(OBJLIBDBI00_SRC:%.c=%.o)
#OBJMODDBI=libmod/dbi0/main.o libmod/dbi0/init.o libmod/dbi0/state.o
#libmod/dbi0/state.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/dbi0/state.c
#libmod/dbi0/init.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/dbi0/init.c
#libmod/dbi0/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/dbi0/main.c
$(MODDBI0ROJ): $(OBJLIBDBI00_OBJ)
	$(CC) -shared $(OBJLIBDBI00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@

OBJDIRDBO00_SRC=$(LIBMOD_EXP)/dbo0
OBJLIBDBO00_SRC=$(shell find $(OBJDIRDBO00_SRC) -name '*.c')
OBJLIBDBO00_OBJ=$(OBJLIBDBO00_SRC:%.c=%.o)
#OBJMODDBO=libmod/dbo0/main.o libmod/dbo0/init.o libmod/dbo0/state.o
#libmod/dbo0/state.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/dbo0/state.c
#libmod/dbo0/init.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/dbo0/init.c
#libmod/dbo0/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/dbo0/main.c
$(MODDBO0ROJ): $(OBJLIBDBO00_OBJ)
	$(CC) -shared $(OBJLIBDBO00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@


OBJDIRSEC00_SRC=$(LIBMOD_EXP)/sec0
OBJLIBSEC00_SRC=$(shell find $(OBJDIRSEC00_SRC) -name '*.c')
OBJLIBSEC00_OBJ=$(OBJLIBSEC00_SRC:%.c=%.o)
#OBJMODSEC=libmod/sec0/main.o	
#libmod/sec0/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/sec0/main.c
$(MODSEC0ROJ): $(OBJLIBSEC00_OBJ)
	$(CC) -shared $(OBJLIBSEC00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@

OBJDIRGAT00_SRC=$(LIBMOD_EXP)/gat0
OBJLIBGAT00_SRC=$(shell find $(OBJDIRGAT00_SRC) -name '*.c')
OBJLIBGAT00_OBJ=$(OBJLIBGAT00_SRC:%.c=%.o)
#OBJMODGAT=libmod/gat0/main.o	
#libmod/gat0/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/gat0/main.c
$(MODGAT0ROJ): $(OBJLIBGAT00_OBJ)
	$(CC) -shared $(OBJLIBGAT00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@

OBJDIRCOR00_SRC=$(LIBMOD_EXP)/cor0
OBJLIBCOR00_SRC=$(shell find $(OBJDIRCOR00_SRC) -name '*.c')
OBJLIBCOR00_OBJ=$(OBJLIBCOR00_SRC:%.c=%.o)
#OBJMODCOR=libmod/cor0/main.o libmod/cor0/init.o
#libmod/cor0/init.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/cor0/init.c
#libmod/cor0/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/cor0/main.c
$(MODCOR0ROJ): $(OBJLIBCOR00_OBJ)
	$(CC) -shared $(OBJLIBCOR00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@

OBJDIRLOG00_SRC=$(LIBMOD_EXP)/log0
OBJLIBLOG00_SRC=$(shell find $(OBJDIRLOG00_SRC) -name '*.c')
OBJLIBLOG00_OBJ=$(OBJLIBLOG00_SRC:%.c=%.o)
#OBJMODLOG=libmod/log0/main.o libmod/log0/init.o	
#libmod/log0/init.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/log0/init.c
#libmod/log0/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/log0/main.c
$(MODLOG0ROJ): $(OBJLIBLOG00_OBJ)
	$(CC) -shared $(OBJLIBLOG00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@

OBJDIRDEV00_SRC=$(LIBMOD_EXP)/dev0
OBJLIBDEV00_SRC=$(shell find $(OBJDIRDEV00_SRC) -name '*.c')
OBJLIBDEV00_OBJ=$(OBJLIBDEV00_SRC:%.c=%.o)
#OBJMODDEV=libmod/dev0/main.o	
#libmod/dev0/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/dev0/main.c
$(MODDEV0ROJ): $(OBJLIBDEV00_OBJ)
	$(CC) -shared $(OBJLIBDEV00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@

OBJDIRICE00_SRC=$(LIBMOD_EXP)/ice0
OBJLIBICE00_SRC=$(shell find $(OBJDIRICE00_SRC) -name '*.c')
OBJLIBICE00_OBJ=$(OBJLIBICE00_SRC:%.c=%.o)
#OBJMODICE=libmod/ice0/main.o	
#libmod/ice0/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/ice0/main.c
$(MODICE0ROJ): $(OBJLIBICE00_OBJ)
	$(CC) -shared $(OBJLIBICE00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@

OBJDIRCTL00_SRC=$(LIBMOD_EXP)/ctl0
OBJLIBCTL00_SRC=$(shell find $(OBJDIRCTL00_SRC) -name '*.c')
OBJLIBCTL00_OBJ=$(OBJLIBCTL00_SRC:%.c=%.o)
#OBJMODCTL=libmod/ctl0/main.o	
#libmod/ctl0/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/ctl0/main.c
$(MODCTL0ROJ): $(OBJLIBCTL00_OBJ)
	$(CC) -shared $(OBJLIBCTL00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@

OBJDIRVID00_SRC=$(LIBMOD_EXP)/vid0
OBJLIBVID00_SRC=$(shell find $(OBJDIRVID00_SRC) -name '*.c')
OBJLIBVID00_OBJ=$(OBJLIBVID00_SRC:%.c=%.o)
#OBJMODVID=libmod/vid0/main.o	
#libmod/vid0/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/vid0/main.c
$(MODVID0ROJ): $(OBJLIBVID00_OBJ)
	$(CC) -shared $(OBJLIBVID00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@

OBJDIRBIO00_SRC=$(LIBMOD_EXP)/bio0
OBJLIBBIO00_SRC=$(shell find $(OBJDIRBIO00_SRC) -name '*.c')
OBJLIBBIO00_OBJ=$(OBJLIBBIO00_SRC:%.c=%.o)
#OBJMODBIO=libmod/bio0/main.o	
#libmod/bio0/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/bio0/main.c
$(MODBIO0ROJ): $(OBJLIBBIO00_OBJ)
	$(CC) -shared $(OBJLIBBIO00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@

OBJDIRHOL00_SRC=$(LIBMOD_EXP)/hol0
OBJLIBHOL00_SRC=$(shell find $(OBJDIRHOL00_SRC) -name '*.c')
OBJLIBHOL00_OBJ=$(OBJLIBHOL00_SRC:%.c=%.o)
#OBJMODHOL=libmod/hol0/main.o	
#libmod/hol0/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/hol0/main.c
$(MODHOL0ROJ): $(OBJLIBHOL00_OBJ)
	$(CC) -shared $(OBJLIBHOL00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@

OBJDIRSTK00_SRC=$(LIBMOD_EXP)/stk0
OBJLIBSTK00_SRC=$(shell find $(OBJDIRSTK00_SRC) -name '*.c')
OBJLIBSTK00_OBJ=$(OBJLIBSTK00_SRC:%.c=%.o)
#OBJMODHOL=libmod/stk0/main.o	
#libmod/stk0/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/stk0/main.c
$(MODSTK0ROJ): $(OBJLIBSTK00_OBJ)
	$(CC) -shared $(OBJLIBSTK00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@

OBJDIRMOD00_SRC=$(LIBMOD_EXP)/mod0
OBJLIBMOD00_SRC=$(shell find $(OBJDIRMOD00_SRC) -name '*.c')
OBJLIBMOD00_OBJ=$(OBJLIBMOD00_SRC:%.c=%.o)
#OBJMODHOL=libmod/mod0/main.o	
#libmod/mod0/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/mod0/main.c
$(MODMOD0ROJ): $(OBJLIBMOD00_OBJ)
	$(CC) -shared $(OBJLIBMOD00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@

OBJDIRPOP00_SRC=$(LIBMOD_EXP)/pop0
OBJLIBPOP00_SRC=$(shell find $(OBJDIRPOP00_SRC) -name '*.c')
OBJLIBPOP00_OBJ=$(OBJLIBPOP00_SRC:%.c=%.o)
#OBJMODHOL=libmod/pop0/main.o	
#libmod/pop0/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/pop0/main.c
$(MODPOP0ROJ): $(OBJLIBPOP00_OBJ)
	$(CC) -shared $(OBJLIBPOP00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@

OBJDIRRWX00_SRC=$(LIBMOD_EXP)/rwx0
OBJLIBRWX00_SRC=$(shell find $(OBJDIRRWX00_SRC) -name '*.c')
OBJLIBRWX00_OBJ=$(OBJLIBRWX00_SRC:%.c=%.o)
#OBJMODHOL=libmod/rwx0/main.o	
#libmod/rwx0/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/rwx0/main.c
$(MODRWX0ROJ): $(OBJLIBRWX00_OBJ)
	$(CC) -shared $(OBJLIBRWX00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@

OBJDIRTHI00_SRC=$(LIBMOD_EXP)/thi0
OBJLIBTHI00_SRC=$(shell find $(OBJDIRTHI00_SRC) -name '*.c')
OBJLIBTHI00_OBJ=$(OBJLIBTHI00_SRC:%.c=%.o)
#OBJMODHOL=libmod/bcdx/main.o	
#libmod/bcdx/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/bcdx/main.c
$(MODTHI0ROJ): $(OBJLIBTHI00_OBJ)
	$(CC) -shared $(OBJLIBTHI00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@

OBJDIRMAP00_SRC=$(LIBMOD_EXP)/map0
OBJLIBMAP00_SRC=$(shell find $(OBJDIRMAP00_SRC) -name '*.c')
OBJLIBMAP00_OBJ=$(OBJLIBMAP00_SRC:%.c=%.o)
#OBJMODHOL=libmod/bcdx/main.o	
#libmod/bcdx/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/bcdx/main.c
$(MODMAP0ROJ): $(OBJLIBMAP00_OBJ)
	$(CC) -shared $(OBJLIBMAP00_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@


OBJDIRBCDX0_SRC=$(LIBMOD_EXP)/bcdx
OBJLIBBCDX0_SRC=$(shell find $(OBJDIRBCDX0_SRC) -name '*.c')
OBJLIBBCDX0_OBJ=$(OBJLIBBCDX0_SRC:%.c=%.o)
#OBJMODHOL=libmod/bcdx/main.o	
#libmod/bcdx/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ libmod/bcdx/main.c
$(MODBCDXROJ): $(OBJLIBBCDX0_OBJ)
	$(CC) -shared $(OBJLIBBCDX0_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) -o $(OUTPUT_DIR)/$(MODULE_DIR)/$@
###################################################################################################################################################


#                                      CLEAN PROJECT


###################################################################################################################################################
#%.l:%.o
#	echo $<

OBJDIRCLRMOD_SRC=$(LIBMOD_EXP)
OBJLIBCLRMOD_SRC=$(shell find $(OBJDIRCLRMOD_SRC) -name '*.o')
OBJLIBCRLMOD_OBJ=$(OBJLIBCLRMOD_SRC:%.o=%.x)


clean: xclean
	rm -rf $(OUTPUT_DIR)/$(PROJECT)   reds/*.o
	rm -rf $(OUTPUT_DIR)/$(PROJECTM)  redm/*.o
	rm -rf $(OUTPUT_DIR)/$(PROJECTO)  offs/*.o
	rm -rf $(OUTPUT_DIR)/$(PROJECTT)  test/*.o
	rm -rf $(OUTPUT_DIR)/$(LIBCRTPROJ) libcrt/*.o
	rm -rf $(OUTPUT_DIR)/$(LIBMICPROJ) libmic/*.o
	rm -rf $(OUTPUT_DIR)/$(LIBRMCPROJ) librmc/*.o
	rm -rf $(OUTPUT_DIR)/$(LIBDEVPROJ) libdev/*.o
	rm -rf $(OUTPUT_DIR)/$(LIBCDAPROJ) libcda/*.o
	rm -rf $(OUTPUT_DIR)/$(LIBSNDPROJ) libsnd/*.o
	rm -rf $(OUTPUT_DIR)/$(LIBQTWPROJ) libqtw/*.o
	rm -rf $(OBJLIBCLRMOD_SRC)
#	rm -rf libmod/dbi0/*.o
#	rm -rf libmod/sec0/*.o
#	rm -rf libmod/gat0/*.o
#	rm -rf libmod/cor0/*.o
#	rm -rf libmod/log0/*.o
#	rm -rf libmod/dev0/*.o
#	rm -rf libmod/ice0/*.o
#	rm -rf libmod/ctl0/*.o
#	rm -rf libmod/vid0/*.o
#	rm -rf libmod/bio0/*.o
#	rm -rf libmod/hol0/*.o
#	rm -rf libmod/dbo0/*.o
	rm -rf $(OUTPUT_DIR)/module/*.mod $(OUTPUT_DIR)/module/*.$(LIBC_EXT) $(OUTPUT_DIR)/module/*.$(SHELLW_EXT) $(OUTPUT_DIR)/module/*.$(SHELL_EXT)
	rm -rf $(OUTPUT_DIR)/tmp/*.tmp
exp:
	LD_LIBRARY_PATH=$(PROJECT_DIR)/bin:$(LD_LIBRARY_PATH)
	export LD_LIBRARY_PATH
	echo $(LD_LIBRARY_PATH)





CMODULE_NAME = xctl10
QMODULE_NAME = xqtl10

obj-m =$(CMODULE_NAME).o 

ARCH := $(shell uname -m)
KERNELVER = $(shell uname -r)
MODDESTDIR := /lib/modules/$(KERNELVER)/kernel/drivers/usb/

CC=gcc

DEVFLAGS=-w
INCLUDE=include
SOURCE=driver/ctl10
TARGET=../../bin/driver/xctl10
OUTPUT=driver/ctl10
SOURCES=$(wildcard $(SOURCE)/*.c)

XSOURCE=driver/ice10
XTARGET=../../bin/driver/xice10
XOUTPUT=driver/ice10
XSOURCES=$(wildcard $(XSOURCE)/*.c)

QSOURCE=driver/qtl10
QTARGET=../../bin/driver/xqtl10
QOUTPUT=driver/qtl10
QSOURCES=$(wildcard $(QSOURCE)/*.c)

EXPORT=package

# Create a symlink from src to bin
$(OUTPUT)/%: $(SOURCE)/%
	ln -s ../$< $@

# Generate a Makefile with the needed obj-m and mymodule-objs set
$(OUTPUT)/Makefile:
	echo "obj-m += $(TARGET).o\n ccflags-y := "$(DEVFLAGS)" -I"$(PROJECT_DIR)/include"\n $(TARGET)-objs := $(subst $(TARGET).o,, $(subst .c,.o,$(subst $(SOURCE)/,,$(SOURCES))))" > $@

# Create a symlink from src to bin
$(XOUTPUT)/%: $(XSOURCE)/%
	ln -s ../$< $@

# Generate a Makefile with the needed obj-m and mymodule-objs set
$(XOUTPUT)/Makefile:
	echo "obj-m += $(XTARGET).o\n ccflags-y := "$(DEVFLAGS)" -I"$(PROJECT_DIR)/include"\n $(XTARGET)-objs := $(subst $(XTARGET).o,, $(subst .c,.o,$(subst $(XSOURCE)/,,$(XSOURCES))))" > $@

# Create a symlink from src to bin
$(QOUTPUT)/%: $(QSOURCE)/%
	ln -s ../$< $@

# Generate a Makefile with the needed obj-m and mymodule-objs set
$(QOUTPUT)/Makefile:
	echo "obj-m += $(QTARGET).o\n ccflags-y := "$(DEVFLAGS)" -I"$(PROJECT_DIR)/include"\n $(QTARGET)-objs := $(subst $(QTARGET).o,, $(subst .c,.o,$(subst $(QSOURCE)/,,$(QSOURCES))))" > $@


#/lib/modules/$(KERNEL)/kernel/drivers/net/
xall: $(INCLUDE) $(subst $(SOURCE),$(OUTPUT),$(SOURCES)) $(OUTPUT)/Makefile $(subst $(XSOURCE),$(XOUTPUT),$(XSOURCES)) $(XOUTPUT)/Makefile $(subst $(QSOURCE),$(QOUTPUT),$(QSOURCES)) $(QOUTPUT)/Makefile
	make -j$(NPROC) ARCH=$(ARCH) -C /lib/modules/$(KERNELVER)/build M=$(PWD)/$(OUTPUT) -I"$(PROJECT_DIR)/include" modules
#	make -j$(NPROC) ARCH=$(ARCH) -C /lib/modules/$(KERNELVER)/build M=$(PWD)/$(XOUTPUT) -I"$(PROJECT_DIR)/include" modules
#	make -j$(NPROC) ARCH=$(ARCH) -C /lib/modules/$(KERNELVER)/build M=$(PWD)/$(QOUTPUT) -I"$(PROJECT_DIR)/include" modules
#	$(CC) -w $(MODULE_PROJECT)c$(MODULE_SERIAL).c -o $(MODULE_PROJECT)c
#	$(CC) -w $(MODULE_PROJECT)srv.c -o $(MODULE_PROJECT)srv
#install:
#	make -C /lib/modules/$(KERNEL)/build M=$(PWD) modules_install 
xclean:	
#	make -C /lib/modules/$(KERNEL)/build M=$(PWD) clean
	rm -fr driver/ctl10/Module.symvers ; rm -fr driver/ctl10/Module.markers ; rm -fr driver/ctl10/modules.order
	rm -fr driver/ctl10/*.mod.c driver/ctl10/*.mod driver/ctl10/*.o driver/ctl10/*.cmd driver/ctl10/*.ko driver/ctl10/*~
	rm -fr driver/ctl10/.tmp_versions driver/ctl10/Makefile
	rm -fr driver/ice10/Module.symvers ; rm -fr driver/ice10/Module.markers ; rm -fr driver/ice10/modules.order
	rm -fr driver/ice10/*.mod.c driver/ice10/*.mod driver/ice10/*.o driver/ice10/*.cmd driver/ice10/*.ko driver/ice10/*~
	rm -fr driver/ice10/.tmp_versions driver/ice10/Makefile
	rm -fr driver/qtl10/Module.symvers ; rm -fr driver/qtl10/Module.markers ; rm -fr driver/qtl10/modules.order
	rm -fr driver/qtl10/*.mod.c driver/qtl10/*.mod driver/qtl10/*.o driver/qtl10/*.cmd driver/qtl10/*.ko driver/qtl10/*~
	rm -fr driver/qtl10/.tmp_versions driver/qtl10/Makefile
	rm -fr bin/driver/*ko bin/driver/*.c bin/driver/*.o bin/driver/*.mod bin/driver/*.cmd
#	rm -rf $(MODULE_PROJECT)c
#	rm -rf $(MODULE_PROJECT)srv
#644
	rm -rf $(SOURCE)/Makefile
	rm -rf $(XSOURCE)/Makefile
install:
	install -v -p -m 644 $(OUTPUT_DIR)/driver/$(CMODULE_NAME).ko  $(MODDESTDIR)
	insmod $(MODDESTDIR)$(CMODULE_NAME).ko
	/sbin/depmod -a ${KERNELVER}
#	install -v -p -m 644 $(OUTPUT_DIR)/driver/$(QMODULE_NAME).ko  $(MODDESTDIR)
#	insmod $(MODDESTDIR)$(QMODULE_NAME).ko
	/sbin/depmod -a ${KERNELVER}
	chmod 777 /dev/crlx0
#	chmod 777 /dev/crlx1	
#	cp -rf $(MODULE_PROJECT)srv.service /lib/systemd/system/$(MODULE_PROJECT)srv.service
#	ln -s /lib/systemd/system/$(MODULE_PROJECT)srv.service /etc/systemd/system/$(MODULE_PROJECT)srv.service
#	mkdir /etc/lsb
#	cp -rf lsbdev /etc/lsb/lsbdev
#	cp -rf $(MODULE_PROJECT)srv /usr/bin/$(MODULE_PROJECT)srv
#	cp -rf $(MODULE_PROJECT)c /usr/bin/$(MODULE_PROJECT)c
#	systemctl enable $(MODULE_PROJECT)srv
#	systemctl start $(MODULE_PROJECT)srv
uninstall: unstall
unstall:
#	systemctl stop $(MODULE_PROJECT)srv
#	systemctl disable $(MODULE_PROJECT)srv
#	rm -rf /usr/bin/$(MODULE_PROJECT)c
#	rm -rf /usr/bin/$(MODULE_PROJECT)srv
#	rm -rf /etc/lsb/*.*
#	rm -rf /etc/lsb
#	rm -rf /etc/systemd/system/$(MODULE_PROJECT)srv.service
#	rm -rf /lib/systemd/system/$(MODULE_PROJECT)srv.service
#	rmmod $(QMODULE_NAME)
#	rm -f $(MODDESTDIR)$(QMODULE_NAME).ko
#	/sbin/depmod -a ${KERNELVER}
	rmmod $(CMODULE_NAME)
	rm -f $(MODDESTDIR)$(CMODULE_NAME).ko
	/sbin/depmod -a ${KERNELVER}


