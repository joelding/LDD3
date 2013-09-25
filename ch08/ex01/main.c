#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE ( "Dual BSD/GPL" ) ; 

void *memptr;

static int kmalloc_init(void)
{
        void *ptr;
	int i;

        printk("kmalloc driver installed.\n");

        memptr = ptr = kmalloc(10, GFP_KERNEL);
        printk("ptr %p\n", ptr);

	printk("before memset\n");
	for (i = 0; i < 10; ++i)
		printk("0x%02x\n", *(unsigned char *)(ptr + i));

	memset(ptr, 0, 10);

	printk("after memset\n");
	for (i = 0; i < 10; ++i)
		printk("0x%02x\n", *(unsigned char *)(ptr + i));

        return 0;
}

static void kmalloc_exit(void)
{
        printk("kmalloc driver removed.\n");

        kfree(memptr);
}

module_init(kmalloc_init);
module_exit(kmalloc_exit);
