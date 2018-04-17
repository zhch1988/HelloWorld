#include <linux/init.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/sched.h>
#include "helper.h"

//MODULE_LICENSE("Dual BSD/GPL");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZhengChen");
MODULE_VERSION("1.0");


static int count = 1;
static char* name = "world";
static int a = 1, b = 2;
// parameters are in /sys/module/<modulename>/parameters/xxxx
module_param(name, charp, S_IRUGO | S_IWUSR);
module_param(count, int, S_IRUGO | S_IWUSR);
module_param(a, int, S_IRUGO | S_IWUSR);
module_param(b, int, S_IRUGO | S_IWUSR);


extern int dependency_sum(int a, int b);

static int __init hello_init(void)
{
    int i = 0;
    char c[256];
    sprintf(c,"Hello World inited. Linux kernel version %s\n", UTS_RELEASE);
    myPrint((const char*)c);
	printk(KERN_INFO "Calling process is \"%s\" (pid %i)\n", current->comm, current->pid);
//    printk(KERN_ALERT "Hello World inited\n");
    
    for(i = 0; i < count; i++)
    {
        sprintf(c,"  Hello %s\n", name);
        myPrint((const char*)c);
//        printk(KERN_ALERT "    Hello %s\n", name);
    }
	
	printk(KERN_INFO "a=%d, b=%d, dependency_sum=%d\n", a, b, dependency_sum(a, b));
    return 0;
}





static void __exit hello_exit(void)
{
    char c[256];
    sprintf(c, "Hello World exit with: name=%s, count=%d\n", name, count);
    myPrint(c);
	printk(KERN_INFO "Calling process is \"%s\" (pid %i)\n", current->comm, current->pid);
//    myPrint("Hello World exit\n");
//    printk(KERN_ALERT "Hello World exit\n");
}

module_init(hello_init);
module_exit(hello_exit);
