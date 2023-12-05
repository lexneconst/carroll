#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#include <linux/module.h>
#if defined(MODVERSIONS)
#include <linux/modversions.h>
#endif


#include <devctl10.h>

//#undef  QTDEVICE_NAME
//#ifndef QTDEVICE_NAME
//#define QTDEVICE_NAME "crtx0"
//#endif



