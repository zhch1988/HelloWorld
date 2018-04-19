#ifndef _ZC_DEV_H
#define _ZC_DEV_H

#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/mutex.h>

#include "zc_cdev.h"

#define DEV_NAME "zc_dev"
#define DEV_NUM 1

struct zc_device
{
	/* device ops */
	const struct file_operations *fops;

	/* sysfs */
	struct device *dev;		/* zc device */
	struct cdev *cdev;		/* character device */
	dev_t devno;
	struct class *cls;

	/* Set either parent or v4l2_dev if your driver uses v4l2_device */
	//struct device *parent;		/* device parent */

	/* callbacks */
	//void (*release)(struct ifly_device *idev);

	char name[32];
	/* serialization lock */
	//struct mutex *lock;
	bool have_data;
	zc_ioctl_data received_data;
	wait_queue_head_t inq, outq;
};

int __must_check zc_register_device(struct zc_device *vdev);

void zc_unregister_device(struct zc_device *vdev);

struct zc_device * __must_check zc_device_alloc(void);

void zv_device_release(struct zv_device *vdev);

struct zc_device* get_zc_device(void);

static inline void *zc_get_drvdata(struct zc_device *vdev)
{
	return dev_get_drvdata(vdev->dev);
}

static inline void zc_set_drvdata(struct zc_device *vdev, void *data)
{
	dev_set_drvdata(vdev->dev, data);
}

#endif /* _ZC_DEV_H */

