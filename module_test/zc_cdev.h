#ifndef _ZC_CDEV_H_
#define _ZC_CDEV_H_

#include <linux/ioctl.h>

#define CDEVFILE "zc_cdev"


/* 定义幻数 */
#define ZC_MAGIC  'x'

/* 定义命令 */
#define ZC_IOC       _IO(ZC_MAGIC, 1)
#define ZC_IOCGET    _IOR(ZC_MAGIC, 2, int)
#define ZC_IOCSET    _IOW(ZC_MAGIC, 3, int)

#define ZC_IOC_MAXNR 3

typedef struct _zc_ioctl_data {
	char data[512];
	unsigned int length;
} zc_ioctl_data;


#endif /* _ZC_CDEV_H_ */
