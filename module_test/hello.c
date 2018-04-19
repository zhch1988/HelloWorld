#include <linux/init.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/slab.h>

#include "helper.h"
#include "ZCLog.h"
#include "zc_dev.h"
#include "zc_cdev.h"

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

static int zc_open(struct inode *inode, struct file *filp)
{
	struct zc_device* idev = (struct zc_device*) get_zc_device();
	
	ZCPRINT("zc_open\n");
	if (!idev) 
        return -ENODEV;
	filp->private_data = idev;
    return 0;
}

static int zc_release(struct inode *inode, struct file *filp)
{
	ZCPRINT("zc_release\n");
    return 0;
}

static ssize_t zc_write(struct file *filp, const char __user *buf,
		size_t sz, loff_t *off)
{
	size_t sended = 0, realSended = 0;
	size_t ksize = 64;
	int i = 0, err = 0;
	int ret = 0;
	char *tmpbuf;

	// struct audio_device *audio;
	// struct audio_transfer *transfer;
	struct zc_device* idev = filp->private_data;
	
	ZCPRINT("zc_write, size=%u, offset=%d\n", sz, *off);
	
	err = !access_ok(VERIFY_READ, (void *)buf, sz);
    if (err)
	{
		printk(KERN_ERR "zc_write access error\n");
		return -EFAULT;
	}
        
	
	tmpbuf = kzalloc(sz, GFP_KERNEL);
	if(copy_from_user(tmpbuf, buf, sz))  
	{
		kfree(tmpbuf);
		return -EFAULT;  
	}
	/* audio = (struct audio_device *)zc_get_drvdata(idev);
	if(!audio)
		return -EFAULT;
	transfer = &audio->transfer;
	if(!transfer)
		return -EFAULT;
	//sendDataTest(transfer, tmpbuf, sz);
	audio_send(tmpbuf, sz, transfer); */
	
#if 0
	tmpbuf = kzalloc(64, GFP_KERNEL);
	while(sended < sz)
	{
		if(sz - sended <= 64)
			ksize = sz - sended;
		else
			ksize = 64;
		
		if(copy_from_user(tmpbuf, buf + sended, ksize))  
		{  
			return -EFAULT;  
		}
#ifdef ZCDEBUG
		sprintf(printBuf, "");
		for(i = 0; i < ksize ; i++)
		{
			sprintf(printBuf, "%s0x%02x ", printBuf, tmpbuf[i]);
		}
		ZCPRINT("printBuf: %s\n", printBuf);
#endif
		if(idev)
		{
			audio = (struct audio_device *)zc_get_drvdata(idev);
			transfer = &audio->transfer;
			ret = sendData(transfer, tmpbuf, ksize);
			//ZCPRINT("sendData returns %d", ret);
			if(ret)
			{
				break;
			}
		}
		sended += ksize;
	}
#endif
	kfree(tmpbuf);

	return sz;
}

static long zc_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
    int err = 0;
	struct zc_device* idev = filp->private_data;
	// struct audio_device *audio;
	// struct audio_transfer *transfer;
    char *ioarg;
	zc_ioctl_data data;

	ZCPRINT("zc_ioctl\n");
    /* 检测命令的有效性 */
    if (_IOC_TYPE(cmd) != ZC_MAGIC) 
        return -EINVAL;
    if (_IOC_NR(cmd) > ZC_IOC_MAXNR) 
        return -EINVAL;

    /* 根据命令类型，检测参数空间是否可以访问 */
    if (_IOC_DIR(cmd) & _IOC_READ)
        err = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
        err = !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));
    if (err) 
        return -EFAULT;

    /* 根据命令，执行相应的操作 */
    switch(cmd) {

      /* 打印当前设备信息 */
      case ZC_IOC:
        break;
      /* 读取参数 */
      case ZC_IOCGET:
        //ret = put_user(bias, (int *)arg);
		if(!idev->have_data)
			return -EAGAIN;
		if(ret = copy_to_user((zc_ioctl_data *)arg, &idev->received_data, sizeof(zc_ioctl_data)))
		{
			return -EFAULT;  
		}
		idev->have_data = false;
        break;
      /* 设置参数 */
      case ZC_IOCSET: 
        //ret = get_user(ioarg, (int *)arg);
		memset(&data, 0, sizeof(zc_ioctl_data));
		if(ret = copy_from_user(&data, arg, sizeof(zc_ioctl_data)))
		{
			return -EFAULT;
		}
		/* audio = (struct audio_device *)zc_get_drvdata(idev);
		if(!audio)
			return -EFAULT;
		transfer = &audio->transfer;
		if(!transfer)
			return -EFAULT;
		if(data.length > transfer->in_ep->maxpacket)
			return -EINVAL;
		memset(transfer->in_req->buf, 0 , transfer->in_ep->maxpacket);
		memcpy(transfer->in_req->buf, &data.data, data.length);
		transfer->in_req->length = data.length;
		//transfer->in_req->length = 64;
		if ((ret = usb_ep_queue(transfer->in_ep, transfer->in_req, GFP_ATOMIC)) < 0) {
			ZCPRINT("Failed to requeue request (%d).\n", ret);
			usb_ep_set_halt(transfer->in_ep);
		} */
        break;

      default:  
        return -EINVAL;
    }

    return ret;
}

/*读函数*/
static ssize_t zc_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
  // unsigned long p =  *ppos;
  // unsigned int count = size;
  // int ret = 0;
  // struct zc_device* idev = filp->private_data;
    
  // while (!idev->have_data) /* 没有数据可读，考虑为什么不用if，而用while */
  // {
        // if (filp->f_flags & O_NONBLOCK)
            // return -EAGAIN;
    
    // wait_event_interruptible(dev->inq,have_data);
  // }

  // /*读数据到用户空间*/
  // if (copy_to_user(buf, (void*)(dev->data + p), count))
  // {
    // ret =  - EFAULT;
  // }
  // else
  // {
    // *ppos += count;
    // ret = count;
   
    // printk(KERN_INFO "read %d bytes(s) from %d\n", count, p);
  // }
  
  // have_data = false; /* 表明不再有数据可读 */
  // /* 唤醒写进程 */
  return size;
}

unsigned int zc_poll(struct file *filp, poll_table *wait)
{
    struct zc_device* idev = filp->private_data;
    unsigned int mask = 0;
    
	ZCPRINT("zc_poll\n");
   /*将等待队列添加到poll_table */
    poll_wait(filp, &idev->inq,  wait);
 
    
    if (idev->have_data)         
		mask |= POLLIN | POLLRDNORM;  /* readable */

    return mask;
}


struct file_operations zc_fops = {
	.owner = THIS_MODULE,
	.open = zc_open,
	.write = zc_write,
	.unlocked_ioctl = zc_ioctl,
	.release = zc_release,
	.poll = zc_poll,
};

static int __init hello_init(void)
{
    int i = 0;
    char c[256];
	struct zc_device *zdev = NULL;
	
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
	
	zdev = kzalloc(sizeof(struct zc_device), GFP_KERNEL);
	if (zdev == NULL)
		return -ENOMEM;
	zdev->fops = &zc_fops;
	zdev->have_data = false;
	
	zc_register_device(zdev);
	
	
    return 0;
}


static void __exit hello_exit(void)
{
    char c[256];
	struct zc_device *zdev = NULL;
    sprintf(c, "Hello World exit with: name=%s, count=%d\n", name, count);
    myPrint(c);
	printk(KERN_INFO "Calling process is \"%s\" (pid %i)\n", current->comm, current->pid);
//    myPrint("Hello World exit\n");
//    printk(KERN_ALERT "Hello World exit\n");
	zdev = get_zc_device();
	printk(KERN_INFO "zdev=%p", zdev);
	if(zdev)
	{
		zc_unregister_device(zdev);
	}
	kfree(zdev);
}

module_init(hello_init);
module_exit(hello_exit);
