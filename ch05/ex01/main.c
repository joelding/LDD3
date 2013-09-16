#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>

static struct _cfake {
	struct cdev cdev;
	dev_t devno;
	struct class *class;
	struct device *device;
	char *buf;
	size_t count;
} *cfake;

#define BUF_SIZE 80

//static struct cdev cfake;
//static dev_t dev;
//static struct class *cdev_class;

static ssize_t cfake_read(struct file *filp,
		char __user *buf,
		size_t count, loff_t *f_pos)
{
	ssize_t retval = 0;

	printk("%s\n", __func__);
	
	if (cfake->count == 0) {
		goto out;
	}	
	
	if (count > BUF_SIZE) {
		cfake->count = BUF_SIZE;
	} else {
		cfake->count = count;
	}

	if (copy_to_user(buf, cfake->buf, cfake->count)) {
		retval = -EFAULT;
		goto out;
	}

	retval = cfake->count;
	cfake-> count = 0;
out:
	return retval;
}

static ssize_t cfake_write(struct file *filp,
		const char __user *buf,
		size_t count, loff_t *f_pos)
{
	ssize_t retval = -1;

	printk("%s\n", __func__);
	
	if (count > BUF_SIZE) {
		cfake->count = BUF_SIZE;
	} else {
		cfake->count = count;
	}
	
	if (copy_from_user(cfake->buf, buf, cfake->count)) {
		retval = -EFAULT;
		goto exit;
	}

	retval = cfake->count;
exit:
	return retval;
}

static int cfake_open(struct inode *inode,
		struct file *filp)
{
	printk("%s\n", __func__);
	return 0;
}

static int cfake_release(struct inode *inode,
		struct file *filp)
{
	printk("%s\n", __func__);
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = cfake_read,
	.write = cfake_write,
	.open = cfake_open,
	.release = cfake_release,
};

static int __init cfake_init(void)
{
	int result = 0;

	printk("%s\n", __func__);

	/* allocate memory and set to zeo */
	cfake = kzalloc(sizeof(struct _cfake), GFP_KERNEL);
	if (!cfake) {
		printk("%s@%d Fail to alloc\n", __func__, __LINE__);
		return -ENOMEM;
	}

	cfake->buf = kzalloc(sizeof(char) * BUF_SIZE, GFP_KERNEL);
	if (!cfake->buf) {
		printk("%s@%d Fail to alloc\n", __func__, __LINE__);
		goto free_dev;
	}

	/* Allocate 1 char device number with minor starting from 0 */
	result = alloc_chrdev_region(&(cfake->devno), 0, 1, "cfake");
	if (result < 0) {
		printk("%s@%d Fail to get dev no\n", __func__, __LINE__);
		goto free_buf;
	}

	cfake->class = class_create(THIS_MODULE, "cfake");
	if (!cfake->class) {
		goto free_devno;
	}

	cdev_init(&cfake->cdev, &fops);
	cfake->cdev.owner = THIS_MODULE;
	result = cdev_add(&cfake->cdev, cfake->devno, 1);
	if (result < 0) {
		printk("%s@%d Fail to add dev\n", __func__, __LINE__);
		goto free_class;
	}
	
	cfake->device = device_create(cfake->class, NULL, cfake->devno, NULL, "cfake");
	if (!cfake->device) {
		printk("%s fail to create device\n", __func__);
		goto free_cdev;
	}

	return 0;

free_cdev:
	cdev_del(&cfake->cdev);
free_class:
	class_destroy(cfake->class);
free_devno:
	unregister_chrdev_region(cfake->devno, 1);
free_buf:
	kfree(cfake->buf);
free_dev:
	kfree(cfake);

	return result;
}

static void __exit cfake_exit(void)
{
	printk("%s\n", __func__);

	device_destroy(cfake->class, cfake->devno);
	cdev_del(&cfake->cdev);
	class_destroy(cfake->class);
	unregister_chrdev_region(cfake->devno, 1);
	kfree(cfake->buf);
	kfree(cfake);
}

module_init(cfake_init);
module_exit(cfake_exit);

MODULE_LICENSE("Dual BSD/GPL");
