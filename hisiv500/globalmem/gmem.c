#include <linux/init.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include "gmem.h"

#define BUFSIZE 4096

static int gmemdev_major = 0;
module_param(gmemdev_major, int, S_IRUGO);

struct gmemdev {
	struct cdev cdev;
	unsigned char buf[BUFSIZE];
};

struct gmemdev *p_gmemdev = NULL;

static ssize_t gmemdev_read(struct file *filp,
						   	char __user *buf,
						   	size_t size,
						   	loff_t *ppos)
{
	unsigned long p = *ppos;
	unsigned int count = size;
	int ret = 0;
	struct gmemdev *dev = filp->private_data;

	if (p >= BUFSIZE)
		return 0;

	if (count > BUFSIZE - p)
		count = BUFSIZE - p;

	if (copy_to_user(buf, dev->buf, count)) {
		ret = -EFAULT;
	} else {
		*ppos += count;
		ret = count;
		printk(KERN_INFO "read %d bytes from %lu\n", count, p);
	}

	return ret;
}

static ssize_t gmemdev_write(struct file *filp,
							 const char __user *buf,
							 size_t size, 
							 loff_t *ppos)
{
	unsigned long p = *ppos;
	unsigned int count = size;
	int ret = 0;
	struct gmemdev *dev = filp->private_data;
	
	if (p >= BUFSIZE)
		return 0;

	if (count > BUFSIZE - p)
		count = BUFSIZE - p;

	if (copy_from_user(dev->buf + p, buf, count)) {
		ret = -EFAULT;
	} else {
		*ppos += count;
		ret = count;
		printk(KERN_INFO "write %d bytes from %lu\n", count, p);
	}


	return ret;
}

static int gmemdev_release(struct inode *inode, 
						   struct file *filp)
{
	return 0;
}

static int gmemdev_open(struct inode *inode, 
						struct file *filp)
{
	filp->private_data = p_gmemdev;
	return 0;
}

static loff_t gmemdev_llseek(struct file *filp, 
							 loff_t offset, 
							 int orig)
{
	loff_t ret = 0;

	switch (orig) {
		case SEEK_SET: /* seek relative to beginning of file */
			if (offset < 0) {
				ret = -EINVAL;
				break;
			}

			if ((unsigned int) offset > BUFSIZE) {
				ret = -EINVAL;
				break;
			}

			filp->f_pos = offset;
			ret = filp->f_pos;
			break;

		case SEEK_END: /* seek relative to end of file */
			if ((filp->f_pos + offset) > BUFSIZE) {
				ret = -EINVAL;
				break;
			}

			if ((filp->f_pos + offset) < 0) {
				ret = -EINVAL;
				break;
			}

			filp->f_pos += offset;
			ret = filp->f_pos;
			break;

		default:
			ret = -EINVAL;
			break;
	}

	return ret;
}

static long gmemdev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct gmemdev *dev = filp->private_data;

	switch (cmd) {
		case MEM_CLEAR:
			memset(dev->buf, 0, BUFSIZE);
			printk(KERN_INFO "gmemdev is set to zero\n");
			break;

		default:
			return -EINVAL;
	}

	return 0;
}

struct file_operations gmem_fops = {
	.owner = THIS_MODULE,
	.read = gmemdev_read,
	.write = gmemdev_write,
	.open = gmemdev_open,
	.release = gmemdev_release,
	.llseek = gmemdev_llseek,
	.unlocked_ioctl = gmemdev_ioctl,
};

static int __init gmem_init(void)
{
	int ret = 0;
	dev_t devno = MKDEV(gmemdev_major, 0);

	printk("%s\n", __func__);

	if (gmemdev_major) {
		ret = register_chrdev_region(devno, 1, "globalmem");
	} else {
		ret = alloc_chrdev_region(&devno, 0, 1, "globalmem");
		gmemdev_major = MAJOR(devno);
		printk("got major=%d\n", gmemdev_major);
	}
	 
	if (ret < 0) {
		return ret;
	}

	p_gmemdev = kzalloc(sizeof(struct gmemdev), GFP_KERNEL);
	if (!p_gmemdev) {
		ret = -ENOMEM;
		goto fail_malloc;
	}
	
	cdev_init(&(p_gmemdev->cdev), &gmem_fops);
	p_gmemdev->cdev.owner = THIS_MODULE;
	ret = cdev_add(&(p_gmemdev->cdev), devno, 1);
	if (ret) {
		printk(KERN_NOTICE "Error %d adding gmem\n", ret);
		goto fail_cdev_init;
	}

	return 0;

fail_cdev_init:
	kfree(p_gmemdev);

fail_malloc:
	unregister_chrdev_region(devno, 1);
	return ret;
}

module_init(gmem_init);

static void __exit gmem_exit(void)
{
	dev_t devno = MKDEV(gmemdev_major, 0);

	printk("%s\n", __func__);

	unregister_chrdev_region(devno, 1);
	cdev_del(&(p_gmemdev->cdev));
	kfree(p_gmemdev);
}

module_exit(gmem_exit);

MODULE_LICENSE("Dual BSD/GPL");

