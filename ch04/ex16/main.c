#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/seq_file.h>

#define PROCFS_NAME "testseq"

struct proc_dir_entry *my_proc_file;
#define SIZE 256
static unsigned char buf[SIZE];

static void *seq_start(struct seq_file *m, 
		loff_t *pos)
{
	printk("%s\n", __func__);

	if (*pos > SIZE)
		*pos = 0;

	return pos;
}

static void *seq_next(struct seq_file *m,
		void *v,
		loff_t *pos)
{
	printk("%s\n", __func__);

	(*pos)++;

	if (*pos > SIZE)
		*pos = 0;

	return pos;
}

static void seq_stop(struct seq_file *m,
		void *v)
{
	printk("%s\n", __func__);
}

static int seq_show(struct seq_file *m,
		void *v)
{
	loff_t *pos = (loff_t *) v;

	seq_printf(m, "%c", buf[*pos]);

	return 0;
}

static struct seq_operations seq_ops = {
	.start = seq_start,
	.next = seq_next,
	.stop = seq_stop,
	.show = seq_show
};

static int proc_open(struct inode *inode,
		struct file *file)
{
	printk("%s\n", __func__);

	return seq_open(file, &seq_ops);
}

static struct file_operations proc_ops = {
	.owner = THIS_MODULE,
	.open = proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release
};

static int proc_init(void)
{
	int ret = 0, i;

	printk("%s\n", __func__);
	my_proc_file = create_proc_entry(PROCFS_NAME, 
			0, /* default: 0444 */ 
			NULL); /* default: /proc/test */

	if (my_proc_file == NULL) {
		ret = -ENOMEM;
		printk(KERN_ALERT "fail to create %s\n", PROCFS_NAME);
	} else {
		my_proc_file->proc_fops = &proc_ops;
	}

	for (i = 0; i < SIZE; ++i) {
		buf[i] = (unsigned char) i;
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
