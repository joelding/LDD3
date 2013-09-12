#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>

MODULE_LICENSE ( "Dual BSD/GPL" ) ; 

struct tasklet_struct tasklet;

void tasklet_fn(unsigned long data)
{
        if (printk_ratelimit()) {
                printk("%s: (%ld, %ld, %ld)\n", __func__, 
                                in_irq(), in_softirq(), in_interrupt());
                tasklet_schedule(&tasklet);
        }
}


static int sample_tasklet_init ( void ) 
{
	printk("%s\n", __func__);	
	tasklet_init(&tasklet, tasklet_fn, 0);
	tasklet_schedule(&tasklet);

        return 0;
} 

static void sample_tasklet_exit ( void ) 
{
	printk("%s\n", __func__);	
        tasklet_kill(&tasklet);

} 

module_init ( sample_tasklet_init ) ; 
module_exit ( sample_tasklet_exit ) ;
