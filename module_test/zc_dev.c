#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kmod.h>
#include <linux/slab.h>
//#include <asm/uaccess.h>

#include "zc_dev.h"
#include "ZCLog.h"

/*
 *	sysfs stuff
 */


//static struct zc_device *idev = NULL;
static int init_cdev(struct zc_device *vdev);
static void release_cdev(struct zc_device *vdev);


/* struct zc_device* get_zc_device()
{
	return idev;
} */

struct zc_device * zc_device_alloc(void)
{
	return kzalloc(sizeof(struct zc_device), GFP_KERNEL);
}

void zc_device_release(struct zv_device *vdev)
{
	kfree(vdev);
}

static int init_cdev(struct zc_device *vdev)
{
	int ret = -1;
	struct cdev *cdev = &vdev->cdev;
    struct device *device;
	struct class *cls;

    ret = alloc_chrdev_region(&vdev->devno, 0, DEV_NUM, DEV_NAME);
    if (ret < 0)
    {
        ZCPRINT("init_cdev: register cdev error: %d", ret);
        goto err_register_chrdev_region;
    }

    cls = class_create(THIS_MODULE, DEV_NAME);
    if(IS_ERR(cls))
    {
        ret = PTR_ERR(cls);
        ZCPRINT("init_cdev: create class error: %d", ret);
        goto err_class_create;
    }
	vdev->cls = cls;

    /*初始化cdev结构*/
	/* cdev = cdev_alloc();
	if (cdev == NULL) {
		goto err_cdev_alloc;
	} */
    cdev_init(cdev, vdev->fops);
    cdev->owner = THIS_MODULE;
    cdev->ops = vdev->fops;
  
    /* 注册字符设备 */
    ret = cdev_add(cdev, vdev->devno, 1);
    if(ret < 0)
    {
        ZCPRINT("init_cdev: add cdev error: %d", ret);
        goto err_cdev_add;
    }
	//vdev->cdev = cdev;
	ZCPRINT("cdev=%p\n", cdev);
    device = device_create(cls, NULL, vdev->devno, NULL, CDEVFILE);
    if(IS_ERR(device)){
        ret = PTR_ERR(device);
        ZCPRINT("init_cdev: create device error: %d", ret);
        goto err_device_create;
    }
    vdev->dev = device;
	/* if (vdev->parent)
		vdev->dev->parent = vdev->parent; */
    return 0;
	//device_destroy(cls, devno);
err_device_create:
	//cdev_del(cdev);
err_cdev_add:
	//class_destroy(cls);
err_cdev_alloc:
	
err_class_create:
    //unregister_chrdev_region(devno, DEV_NUM);
err_register_chrdev_region:
	release_cdev(vdev);
    return ret;
}


static void release_cdev(struct zc_device *vdev)
{
	if(vdev->dev != NULL && !IS_ERR(vdev->dev))
	{
		device_destroy(vdev->cls, vdev->devno);
		vdev->dev = NULL;
	}
	if(1)
	{
		cdev_del(&vdev->cdev);
		//kfree(vdev->cdev);
		//vdev->cdev = NULL;
	}
	if(vdev->cls != NULL && !IS_ERR(vdev->cls))
	{
		class_destroy(vdev->cls);
		vdev->cls = NULL;
	}
	if(vdev->devno)
	{
		unregister_chrdev_region(vdev->devno, DEV_NUM); /*释放设备号*/
		vdev->devno = NULL;
	}
}

int zc_register_device(struct zc_device *vdev)
{
	int ret = 0;
	ret = init_cdev(vdev);
	printk(KERN_INFO "ret of init_cdev=%d", ret);
	if(ret == 0)
	{
		init_waitqueue_head(&vdev->inq);
		//idev = vdev;
	}
	printk(KERN_INFO "zc_register_device: vdev=%p", vdev);
	
	return ret;
}

void zc_unregister_device(struct zc_device *vdev)
{
	if(!vdev)
		return;
	release_cdev(vdev);
	//idev = NULL;
}

