/*
ex27 is basic char driver module with mmap implemented,based on the chapter 15 of ldd3 book
*/

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <asm/current.h>
#include <linux/device.h>
#include<linux/slab.h>
#include <linux/mm.h>
#include <linux/highmem.h>

#define CHAR_DEV_NAME "sample_cdrv"
#define MAX_LENGTH 4000
#define SUCCESS 0

static char *char_device_buf;
struct cdev *sample_cdev;
dev_t mydev;
int count=1;
static struct class *sample_class;
static unsigned long addr;
static int counter = 0;

static int char_dev_open(struct inode *inode, struct file  *file)
{
	
	counter++;
	printk(KERN_INFO "Number of times open() was called: %d\n", counter);
	printk (KERN_INFO "MAJOR number = %d, MINOR number = %d\n",imajor (inode), iminor (inode));
	printk(KERN_INFO "Current process ID: %d\n",current->pid );
	return SUCCESS;
}

static int char_dev_release(struct inode *inode, struct file *file)
{
	return SUCCESS;
}

static ssize_t char_dev_read(struct file *file, char *buf, size_t lbuf, loff_t *ppos)
{
	int maxbytes; /* number of bytes from ppos to MAX_LENGTH */
	int bytes_to_do; /* number of bytes to read */
	int nbytes; /* number of bytes actually read */

	maxbytes = MAX_LENGTH - *ppos;
	
	if( maxbytes > lbuf ) bytes_to_do = lbuf;
	else bytes_to_do = maxbytes;
	
	if( bytes_to_do == 0 ) {
		printk("Reached end of device\n");
		return -ENOSPC; /* Causes read() to return EOF */
	}
	
	nbytes = bytes_to_do -  copy_to_user( buf, char_device_buf + *ppos, bytes_to_do ); /* how many bytes */
	*ppos += nbytes;
	return nbytes;	
}

static ssize_t char_dev_write(struct file *file, const char *buf, size_t lbuf,loff_t *ppos)
{
	int nbytes; /* Number of bytes written */
	int bytes_to_do; /* Number of bytes to write */
	int maxbytes; /* Maximum number of bytes that can be written */

	maxbytes = MAX_LENGTH - *ppos;

	if( maxbytes > lbuf ) bytes_to_do = lbuf;
	else bytes_to_do = maxbytes;

	if( bytes_to_do == 0 ) {
		printk("Reached end of device\n");
		return -ENOSPC; /* Returns EOF at write() */
	}

	nbytes = bytes_to_do -copy_from_user( char_device_buf + *ppos,buf,bytes_to_do ); /* how many bytes */
	*ppos += nbytes;
	return nbytes;
}


static void char_vm_open(struct vm_area_struct *vma)
{
        /* Nothing To Do */
}
static void char_vm_close(struct vm_area_struct *vma)
{
}

/*VM_OPERATIONS for our char device */
struct vm_operations_struct char_vm_ops ={
        open:char_vm_open,
        close:char_vm_close,
};

static int char_dev_mmap(struct file *filp, struct vm_area_struct *vma)
{
        addr  = (unsigned long)char_device_buf;
        addr = __pa(addr);
        if (remap_pfn_range(vma, vma->vm_start,addr >> PAGE_SHIFT,PAGE_ALIGN(MAX_LENGTH),vma->vm_page_prot))
                   return -EAGAIN;
	vma->vm_ops = &char_vm_ops;
        return SUCCESS;
}

static struct file_operations char_dev_fops = {
	.owner = THIS_MODULE,
	.read = char_dev_read,
	.write = char_dev_write,
	.open = char_dev_open,
	.release = char_dev_release,
	.mmap = char_dev_mmap  
};

static __init int char_dev_init(void)
{
	int ret;

	if (alloc_chrdev_region (&mydev, 0, count, CHAR_DEV_NAME) < 0) {
            printk (KERN_ERR "failed to reserve major/minor range\n");
            return -1;
    }

        if (!(sample_cdev = cdev_alloc ())) {
            printk (KERN_ERR "cdev_alloc() failed\n");
            unregister_chrdev_region (mydev, count);
            return -1;
 	}
	cdev_init(sample_cdev,&char_dev_fops);

	ret=cdev_add(sample_cdev,mydev,count);
	if( ret < 0 ) {
		printk(KERN_INFO "Error registering device driver\n");
	        cdev_del (sample_cdev);
                unregister_chrdev_region (mydev, count); 	
		return -1;
	}

	sample_class = class_create (THIS_MODULE, "sample");
        device_create (sample_class, NULL, mydev, NULL, "%s", "sample_cdrv");

	printk(KERN_INFO"\nDevice Registered: %s\n",CHAR_DEV_NAME);
	printk (KERN_INFO "Major number = %d, Minor number = %d\n", MAJOR (mydev),MINOR (mydev));

	char_device_buf =(char *)kmalloc(MAX_LENGTH,GFP_ATOMIC);
	return 0;
}

static __exit void  char_dev_exit(void)
{
	 device_destroy (sample_class, mydev);
         class_destroy (sample_class);
	 cdev_del(sample_cdev);
	 unregister_chrdev_region(mydev,1);
	 kfree(char_device_buf);
	 printk(KERN_INFO "\n Driver unregistered \n");
}
module_init(char_dev_init);
module_exit(char_dev_exit);

MODULE_AUTHOR("Alex");
MODULE_DESCRIPTION("Ex27: Char Device Driver with mmap");
MODULE_LICENSE("GPL");
