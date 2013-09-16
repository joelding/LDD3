#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>

#define PROCFS_NAME "test"

struct proc_dir_entry *my_proc_file;

static int proc_init(void)
{
	int ret = 0;

	printk("%s\n", __func__);
	my_proc_file = create_proc_entry(PROCFS_NAME, 
			0, 
			NULL);

	if (my_proc_file == NULL) {
		ret = -ENOMEM;
		printk(KERN_ALERT "fail to create %s\n", PROCFS_NAME);
	} else {
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
