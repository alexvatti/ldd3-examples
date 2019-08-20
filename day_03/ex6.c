/*
ex6 is create a entry to proc file systems, to access it using seq file
*/

#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
#include<linux/kernel.h>   
#include<linux/proc_fs.h>
#include<linux/uaccess.h>
#include<linux/fs.h>
#include <linux/seq_file.h>	

#define BUFSIZE  100


static struct proc_dir_entry *ent;


static void *my_seq_start(struct seq_file *s, loff_t *pos)
{
	static unsigned long counter = 0;

	/* beginning a new sequence ? */	
	if ( *pos == 0 )
	{	
		/* yes => return a non null value to begin the sequence */
		return &counter;
	}
	else
	{
		/* no => it's the end of the sequence, return end to stop reading */
		*pos = 0;
		return NULL;
	}
}

static void *my_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	unsigned long *tmp_v = (unsigned long *)v;
	(*tmp_v)++;
	(*pos)++;
	return NULL;
}

static void my_seq_stop(struct seq_file *s, void *v)
{
	/* nothing to do, we use a static value in start() */
}

static int my_seq_show(struct seq_file *s, void *v)
{
	loff_t *spos = (loff_t *) v;
	
	seq_printf(s, "%Ld\n", *spos);
	return 0;
}

static struct seq_operations my_seq_ops = {
	.start = my_seq_start,
	.next  = my_seq_next,
	.stop  = my_seq_stop,
	.show  = my_seq_show,
};

static int my_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &my_seq_ops);
};

static struct file_operations myops = 
{
	.owner = THIS_MODULE,
	.open = my_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release,
};

static int simple_init(void)
{
	ent=proc_create("mydev",0666,NULL,&myops);
	printk(KERN_ALERT "Created a entry to proc file system...\n");
	return 0;
}

static void simple_cleanup(void)
{
	proc_remove(ent);
	printk(KERN_WARNING "Removed the entry to proc file system ...\n");
}

module_init(simple_init);
module_exit(simple_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex");
MODULE_DESCRIPTION("Create a entry in Proc file system, access it");
