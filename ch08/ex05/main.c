#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE ( "Dual BSD/GPL" ) ; 

void *ptr;
int order = 5;

void *ptr2;
int order2;

static int gpages_init(void)
{
        printk("gpages driver installed.\n");

        ptr = (void *)__get_free_pages(GFP_KERNEL, order);
        if (ptr) {
                printk("2^%d * %ld bytes allocated\n", order, PAGE_SIZE);
        }

        order2 = get_order(8000);
        ptr2 = (void *)__get_free_pages(GFP_KERNEL, order2);
        if (ptr2) {
                printk("2^%d * %ld bytes allocated\n", order2, PAGE_SIZE);
        }

        return 0;

}

static void gpages_exit(void)
{
        printk("gpages driver removed.\n");

        if (ptr)
                free_pages((unsigned long)ptr, order);

        if (ptr2)
                free_pages((unsigned long)ptr2, order2);

}

module_init(gpages_init);
module_exit(gpages_exit);
