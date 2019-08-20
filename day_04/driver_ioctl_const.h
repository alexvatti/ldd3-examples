#ifndef __DRIVER_IOCTL_CONST_H_
#define __DRIVER_IOCTL_CONST_H_

#include <asm/ioctl.h>
#define MAGIC_NUM 'm'

#define CLEAR_BUFFER   _IO(MAGIC_NUM,1)
#define FILL_BUFFFER   _IOW(MAGIC_NUM,2,char)

#define IOCTL_NUMS 3

#endif
