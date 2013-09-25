#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE ( "Dual BSD/GPL" ) ; 

void *memptr;

static int kmalloc_dfree_init(void)
{
        void *ptr;

        printk("kmalloc dfree driver installed.\n");

        memptr = ptr = kmalloc(10, GFP_KERNEL);
        printk("ptr %p\n", ptr);

        return 0;
}

static void kmalloc_dfree_exit(void)
{
	/* double free */
        kfree(memptr);
        kfree(memptr);
        
	printk("kmalloc driver removed.\n");
}

module_init(kmalloc_dfree_init);
module_exit(kmalloc_dfree_exit);
