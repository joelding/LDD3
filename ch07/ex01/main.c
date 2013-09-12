#include <linux/init.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/time.h>

MODULE_LICENSE("Dual BSD/GPL");

static unsigned long prev;

static int jiffies_init(void)
{
	struct timeval val = {0};
	struct tm t = {0};

	printk("%s jiffies=%lu\n", __func__, (prev = jiffies));
	printk("HZ=%d\n", HZ);
	do_gettimeofday(&val);
	printk("%ld.%.6ld\n", val.tv_sec, val.tv_usec);
	time_to_tm(val.tv_sec, 0, &t);
	printk("%ld-%d-%d %d:%d:%d\n", t.tm_year + 1990, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	return 0;
}

static void jiffies_exit(void)
{
	unsigned long now = jiffies;
	struct timespec spec = {0};
	struct timeval val = {0};
	unsigned long timeout;
	printk("%s jiffies=%lu\n", __func__, now);
	printk("time_after=%d\n", time_after(now, prev));
	printk("time_before=%d\n", time_before(now, prev));
	jiffies_to_timespec(now - prev, &spec);
	printk("%ld.%.9ld\n", spec.tv_sec, spec.tv_nsec);
	jiffies_to_timeval(now - prev, &val);
	printk("%ld.%.6ld\n", val.tv_sec, val.tv_usec);
	
	timeout = 10;
	now = 2147483658;
	printk("wrap-around=%d\n", time_before(timeout, now));
	timeout = -2146483000;
	printk("wrap-around=%d\n", time_before(timeout, now));
//	printk("wrap-around=%d\n", time_after(now, now + 1));
}

module_init(jiffies_init);
module_exit(jiffies_exit);
