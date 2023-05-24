#ifndef CTL10_H
#define CTL10_H

#define CDDEVICE_NAME "crlx0"
#define CDEVICE_NAME  "/dev/crlx0"

#define QTDEVICE_NAME "crlx1"
#define QDEVICE_NAME  "/dev/crlx1"

#define ICDEVICE_NAME "icex0"
#define IDEVICE_NAME  "/dev/icex0"

#define CTL10_DISABLE_VALUE 2

#define CTL10_DEVINIT       0x10    
#define CTL10_PROGWRITE     0x20
#define CTL10_PROGREAD      0x30
#define CTL10_RESTART       0x40
#define CTL10_POWEROFF      0x50
#define CTL10_STATEOFF      0x60

#endif

