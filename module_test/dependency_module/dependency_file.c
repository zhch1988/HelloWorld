#include <linux/init.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZhengChen");
MODULE_VERSION("1.0");


static int __init dependency_init(void)
{
    printk(KERN_INFO "dependency inited. Linux kernel version %s\n", UTS_RELEASE);
	printk(KERN_INFO "Calling process is \"%s\" (pid %i)\n", current->comm, current->pid);

    return 0;
}

static int dependency_sum(int a, int b)
{
	return a + b;
}

EXPORT_SYMBOL(dependency_sum);


static void __exit dependency_exit(void)
{
    printk(KERN_INFO "dependency exit\n");
	printk(KERN_INFO "Calling process is \"%s\" (pid %i)\n", current->comm, current->pid);

}

module_init(dependency_init);
module_exit(dependency_exit);