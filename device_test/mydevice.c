#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include "ZCLog.h"

//MODULE_LICENSE("Dual BSD/GPL");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZhengChen");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("My Device Test");


static void device_release(struct device *dev)  
{
	ZCPRINT("device_release\n");
	return ;
}

static struct platform_device my_device=
{
	.name = "ZhengChen's platform_device",
	.id = -1,
	.dev.release = device_release,
};


static int __init device_init(void)
{
	int i = 0;
	char c[256];
	//struct zc_device *zdev = NULL;
	ZCPRINT("device_init\n");

	return platform_device_register(&my_device);
}


static void __exit device_exit(void)
{
	char c[256];
	// struct zc_device *zdev = NULL;
	ZCPRINT("device_exit\n");
	platform_device_unregister(&my_device);
}

module_init(device_init);
module_exit(device_exit);