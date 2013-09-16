#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>

#define PROCFS_NAME "test"

struct proc_dir_entry *my_proc_file;

static int proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int len = 0;
	static int cnt = 0;
	
	if (off > 0) {
		len = 0;
	} else {
		len = sprintf(page, "read the %dth time\n", cnt++);
	}

	return len;
}

static int proc_init(void)
{
	int ret = 0;

	printk("%s\n", __func__);
	my_proc_file = create_proc_entry(PROCFS_NAME, 
			0, /* default: 0444 */ 
			NULL); /* default: /proc/test */

	if (my_proc_file == NULL) {
		ret = -ENOMEM;
		printk(KERN_ALERT "fail to create %s\n", PROCFS_NAME);
	} else {
	my_proc_file->read_proc = proc_read;
	}

	return ret;
}

static void proc_exit(void)
{
	printk("%s\n", __func__);
	remove_proc_entry(PROCFS_NAME, NULL);
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
