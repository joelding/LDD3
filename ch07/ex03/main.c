#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

MODULE_LICENSE ( "GPL v3" ) ; 

#define TIMEOUT_VALUE (5 * HZ)

static struct timer_list timerfn;


static void timer_timeout(unsigned long arg)
{
        struct timer_list *ptr = (struct timer_list *)arg;

        printk("ptr %p (%s)\n", ptr, __func__);

	mod_timer(ptr, jiffies + TIMEOUT_VALUE);
}


static int timer_init ( void ) 
{
	printk("%s\n", __func__);

        init_timer(&timerfn);
        timerfn.function = timer_timeout;
        timerfn.data = (unsigned long)&timerfn;
        timerfn.expires = jiffies + TIMEOUT_VALUE;
        add_timer(&timerfn);
 
	return 0;
} 

static void timer_exit ( void ) 
{
	int ret;
	printk("%s\n", __func__);
        ret = del_timer_sync(&timerfn);
} 

module_init ( timer_init ) ; 
module_exit ( timer_exit ) ;
