#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE ( "Dual BSD/GPL" ) ; 

void *memptr;

static int kmalloc_init(void)
{
        void *ptr;
	int i;

        printk("kmalloc driver installed.\n");
	/* allocate 10 bytes */
        memptr = ptr = kmalloc(10, GFP_KERNEL);
        printk("ptr %p\n", ptr);

	if (memptr == NULL) {
		printk("failed kmalloc\n");
		return -1;
	}

	printk("before memset\n");
	for (i = 0; i < 10; ++i)
		printk("0x%02x\n", *(unsigned char *)(ptr + i));

	memset(ptr, 0, 10);

	printk("after memset\n");
	for (i = 0; i < 10; ++i)
		printk("0x%02x\n", *(unsigned char *)(ptr + i));

	kfree(ptr);
	memptr = ptr = kzalloc(10, GFP_KERNEL);
	if (memptr == NULL) {
		printk("failed kzalloc\n");
		return -1;
	
	}

	printk("kzalloc\n");
	for (i = 0; i < 10; ++i)
		printk("0x%02x\n", *(unsigned char *)(ptr + i));

	printk("ksize=%d\n", ksize(memptr));

        return 0;
}

static void kmalloc_exit(void)
{
        printk("kmalloc driver removed.\n");

        kfree(memptr);
}

module_init(kmalloc_init);
module_exit(kmalloc_exit);
