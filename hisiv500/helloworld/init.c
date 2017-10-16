#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL v3");
MODULE_AUTHOR("Joel Ding <joel.ding@gmail.com>");
MODULE_VERSION("0.0.1");
MODULE_DESCRIPTION("dummy driver");

static int __init helloworld_init(void)
{
	printk("%s\n", __func__);
	return 0;
}

module_init(helloworld_init);

static void __exit helloworld_exit(void)
{
	printk("%s\n", __func__);
}

module_exit(helloworld_exit);

