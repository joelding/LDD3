#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>

static struct cdev chrdev;
static dev_t dev;
static struct class *cdev_class;

static ssize_t chrdev_read(struct file *filp,
		char __user *buf,
		size_t count, loff_t *f_pos)
{
	ssize_t retval = 0;

	printk("%s\n", __func__);
	return retval;
}

static ssize_t chrdev_write(struct file *filp,
		const char __user *buf,
		size_t count, loff_t *f_pos)
{
	ssize_t retval = -1;

	printk("%s\n", __func__);
	return retval;
}

static int chrdev_open(struct inode *inode,
		struct file *filp)
{
	printk("%s\n", __func__);
	return 0;
}

static int chrdev_release(struct inode *inode,
		struct file *filp)
{
	printk("%s\n", __func__);
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = chrdev_read,
	.write = chrdev_write,
	.open = chrdev_open,
	.release = chrdev_release,
};

static int __init chrdev_init(void)
{
	int result = 0;
	struct device *device = NULL;

	printk("%s\n", __func__);

	/* Allocate a range of char device number */
	result = alloc_chrdev_region(&dev, 0, 1, "chrdev_test");
	printk("%d:%d\n", MAJOR(dev), MINOR(dev));
	if (result < 0) {
		return result;
	}

	cdev_class = class_create(THIS_MODULE, "chrdev_test");
	if (!cdev_class) {
		unregister_chrdev_region(dev, 1);
		return -1;
	}

	cdev_init(&chrdev, &fops);
	chrdev.owner = THIS_MODULE;
	cdev_add(&chrdev, dev, 1);
	device = device_create(cdev_class, NULL, dev, NULL, "chrdev_test");
	if (!device) {
		printk("%s fail to create device\n", __func__);
	}
	return 0;
}

static void __exit chrdev_exit(void)
{
	printk("%s\n", __func__);
	device_destroy(cdev_class, dev);
	cdev_del(&chrdev);
	class_destroy(cdev_class);
	unregister_chrdev_region(dev, 1);
}

module_init(chrdev_init);
module_exit(chrdev_exit);

MODULE_LICENSE("Dual BSD/GPL");
