#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
//#include <linux/jiffies.h>

MODULE_LICENSE ( "GPL v3" ) ; 

int delay = HZ;

enum jit_files {
        JIT_BUSY,
        JIT_SCHED,
        JIT_QUEUE,
        JIT_SCHEDTO
};

int jit_fn(char *buf, char **start, off_t offset,
              int len, int *eof, void *data)
{
        unsigned long j0, j1; /* jiffies */
        wait_queue_head_t wait;

        init_waitqueue_head (&wait);
        j0 = jiffies;
        j1 = j0 + delay;

        switch((long)data) {
                case JIT_BUSY:
                        while (time_before(jiffies, j1))
                                cpu_relax();
                        break;
                case JIT_SCHED:
                        while (time_before(jiffies, j1)) {
                                schedule();
                        }
                        break;
                case JIT_QUEUE:
                        wait_event_interruptible_timeout(wait, 0, delay);
                        break;
                case JIT_SCHEDTO:
                        set_current_state(TASK_INTERRUPTIBLE);
                        schedule_timeout (delay);
                        break;
        }
        j1 = jiffies; /* actual value after we delayed */
	len = sprintf(buf, "%9li %9li\n", j0, j1);
        *start = buf;
        return len;
}


#if 0
int jit_currentime(char *buf, char **start, off_t offset,
                   int len, int *eof, void *data)
{
        struct timeval tv1;
        struct timespec tv2;
        unsigned long j1;


        /* get them four */
        j1 = jiffies;
        do_gettimeofday(&tv1);
        tv2 = current_kernel_time();

        /* print */
        len=0;
        len += sprintf(buf,"0x%08lx %10i.%06i\n"
                       "%40i.%09i\n",
                       j1,
                       (int) tv1.tv_sec, (int) tv1.tv_usec,
                       (int) tv2.tv_sec, (int) tv2.tv_nsec);


        *start = buf;  /*if you want currentime to output only onec ,disable this line!*/

        return len;
}
#endif

static int long_delay_init ( void ) 
{

        printk("%s\n", __func__);
//        create_proc_read_entry("currentime", 0, NULL, jit_currentime, NULL);
        create_proc_read_entry("jitbusy", 0, NULL, jit_fn, (void *)JIT_BUSY);
        create_proc_read_entry("jitsched",0, NULL, jit_fn, (void *)JIT_SCHED);
        create_proc_read_entry("jitqueue",0, NULL, jit_fn, (void *)JIT_QUEUE);
        create_proc_read_entry("jitschedto", 0, NULL, jit_fn, (void *)JIT_SCHEDTO);

	return 0;
} 

static void long_delay_exit ( void ) 
{
        printk("%s\n", __func__);
//	remove_proc_entry("currentime", NULL);
        remove_proc_entry("jitbusy", NULL);
        remove_proc_entry("jitsched", NULL);
        remove_proc_entry("jitqueue", NULL);
        remove_proc_entry("jitschedto", NULL);
} 

module_init ( long_delay_init ) ; 
module_exit ( long_delay_exit ) ;
