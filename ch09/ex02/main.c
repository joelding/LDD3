#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/ioport.h>
#include <asm/io.h>

#define PARAPORT 0x378

static struct _ppfake {
	struct cdev cdev;
	dev_t devno;
	struct class *class;
	struct device *device;
	struct resource *res;
} *ppfake;

static ssize_t ppfake_write(struct file *filp,
		const char __user *buf,
		size_t count, loff_t *f_pos)
{
	unsigned char uc = 0;

	get_user(uc, buf); 
	printk("%s 0x%x\n", __func__, uc);
	
	outb(uc, PARAPORT);	
	printk("in = 0x%x\n", inb(PARAPORT));

	return sizeof(unsigned char); 
}

static int ppfake_open(struct inode *inode,
		struct file *filp)
{
	printk("%s\n", __func__);
	return 0;
}

static int ppfake_release(struct inode *inode,
		struct file *filp)
{
	printk("%s\n", __func__);
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = ppfake_write,
	.open = ppfake_open,
	.release = ppfake_release,
};

static int __init ppfake_init(void)
{
	int result = 0;

	printk("%s\n", __func__);

	/* allocate memory and set to zeo */
	ppfake = kzalloc(sizeof(struct _ppfake), GFP_KERNEL);
	if (!ppfake) {
		printk("%s@%d Fail to alloc\n", __func__, __LINE__);
		return -ENOMEM;
	}

	ppfake->res = request_region(PARAPORT, 3, "ppfake");
	if (!ppfake->res) {
		printk("%s@%d Fail to request region\n", __func__, __LINE__);
		goto free_dev;
	}

	/* Allocate 1 char device number with minor starting from 0 */
	result = alloc_chrdev_region(&(ppfake->devno), 0, 1, "ppfake");
	if (result < 0) {
		printk("%s@%d Fail to get dev no\n", __func__, __LINE__);
		goto free_region;
	}

	ppfake->class = class_create(THIS_MODULE, "ppfake");
	if (!ppfake->class) {
		goto free_devno;
	}

	cdev_init(&ppfake->cdev, &fops);
	ppfake->cdev.owner = THIS_MODULE;
	result = cdev_add(&ppfake->cdev, ppfake->devno, 1);
	if (result < 0) {
		printk("%s@%d Fail to add dev\n", __func__, __LINE__);
		goto free_class;
	}
	
	ppfake->device = device_create(ppfake->class, NULL, ppfake->devno, NULL, "ppfake");
	if (!ppfake->device) {
		printk("%s@%d fail to create device\n", __func__, __LINE__);
		goto free_cdev;
	}

	outb(0xf, PARAPORT + 2);
	printk("addr 0x%x = 0x%x\n", PARAPORT + 2, inb(PARAPORT + 2));
	outb(0x00, PARAPORT);	

	return 0;

free_cdev:
	cdev_del(&ppfake->cdev);
free_class:
	class_destroy(ppfake->class);
free_devno:
	unregister_chrdev_region(ppfake->devno, 1);
free_region:
	release_region(PARAPORT, 3);
free_dev:
	kfree(ppfake);

	return result;
}

static void __exit ppfake_exit(void)
{
	printk("%s\n", __func__);

	device_destroy(ppfake->class, ppfake->devno);
	cdev_del(&ppfake->cdev);
	class_destroy(ppfake->class);
	unregister_chrdev_region(ppfake->devno, 1);
	outb(0x0, PARAPORT);
	release_region(PARAPORT, 3);
	kfree(ppfake);
}

module_init(ppfake_init);
module_exit(ppfake_exit);

MODULE_LICENSE("Dual BSD/GPL");
