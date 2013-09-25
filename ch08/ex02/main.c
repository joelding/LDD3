#include <linux/init.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/timer.h>

MODULE_LICENSE ( "Dual BSD/GPL" ) ; 

#define TIMEOUT_VALUE (5*HZ)
static struct timer_list tickfn;

static void memleak_timeout(unsigned long arg)
{
        struct timer_list *ptr = (struct timer_list *)arg;
        void *memptr;

        printk("ptr %p (%s)\n", ptr, __func__);

	/* Bug! Correct: GFP_ATOMIC */
        memptr = kmalloc(1*1024* 1024, GFP_KERNEL);
        if (ptr) {
                printk("memptr %p\n", memptr);
                kfree(memptr);
        }
}

static int memleak_init(void)
{

        printk("memleak driver installed.\n");

        init_timer(&tickfn);
        tickfn.function = memleak_timeout;
        tickfn.data = (unsigned long)&tickfn;
        tickfn.expires = jiffies + TIMEOUT_VALUE;
        add_timer(&tickfn);

        return 0;
}

static void memleak_exit(void)
{
	int ret;
        printk("memleak driver removed.\n");

	ret = del_timer_sync(&tickfn);

        printk(KERN_ALERT "driver unloaded (%d)\n", ret);
}

module_init(memleak_init);
module_exit(memleak_exit);
