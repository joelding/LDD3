#include <linux/init.h>
#include <linux/module.h>
#include <linux/hardirq.h>
#include <linux/workqueue.h>

MODULE_LICENSE ( "Dual BSD/GPL" ) ; 

struct workqueue_struct *wqueue;
struct work_struct work;

void work_fn(struct work_struct *work)
{
        if (printk_ratelimit()) {
                printk("%s: (%ld, %ld, %ld)\n", __func__, 
                                in_irq(), in_softirq(), in_interrupt());
		schedule_work(work);
        }
}


static int sample_wqueue_init ( void ) 
{
	printk("%s\n", __func__);

	wqueue = create_workqueue("wqueue");	
	INIT_WORK(&work, work_fn);

	queue_work(wqueue, &work);

        return 0;
} 

static void sample_wqueue_exit ( void ) 
{
	printk("%s\n", __func__);	
	flush_workqueue(wqueue);
	destroy_workqueue(wqueue);
} 

module_init ( sample_wqueue_init ) ; 
module_exit ( sample_wqueue_exit ) ;
