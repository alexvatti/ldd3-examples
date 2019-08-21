/*
ex31: char drv module is basic module based on adc module in S3C2410 SoC
This Example Moulde will run on S3C240 SoC: Pls Make a Note of It.
dynamic allocation of major and minor numbers
*/
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<asm/uaccess.h>
#include<linux/slab.h>
#include<linux/semaphore.h>
#include<linux/io.h>
#include<linux/ioport.h>
#include<linux/delay.h>

//step 1:
#define ADC_BASE_ADDRESS 0x58000000
#define ADC_ADDRESS_LEN   0x20
#define ADCCON	0x0000   
#define ADCTSC  0x0004 
#define ADCDLY  0x0008
#define ADCDAT0 0x000C
#define ADCDAT1 0x0010

//BITS
/* Bit definitions common for ADC*/
#define ADC_CON_EN_START	(1u << 0)
#define ADC_CON_SELMUX(x)       (((x) & 7) << 3)
#define ADC_CON_END(x)          (x << 15)
#define ADC_DAT0_MASK		0xFFF
#define ADC_DAT1_MASK		0xFFF

int major_num=0,minor_num=0; //dynamic
dev_t char_dev;
struct cdev *adc_char_cdev;
int count=1;
char *buffer=NULL;
int buffer_len=4000;
static struct semaphore adc_lock;
void *adc_virtual_address;

static int adc_char_drv_open(struct inode *inode, struct file *fp)
{
		
	return 0;
}

static int adc_char_drv_release(struct inode *inode, struct file *fp)
{
	return 0;
}
static ssize_t adc_char_drv_read(struct file *fp, char __user *user_ptr, size_t count, loff_t *off)
{
	size_t len=0,ret_len=0;
        unsigned int adc_value,adc_control_value;

 	if (down_interruptible(&adc_lock))  
        	return -ERESTARTSYS; 

	//step 3:
	//default channel is 1
	adc_control_value=(ADC_CON_SELMUX(1) | ADC_CON_EN_START);
	writel(adc_control_value,(adc_virtual_address +ADCCON));

	mdelay(3);

	while((readl(adc_virtual_address + ADCCON) & (1<<15)) !=1); //check for adc done

	adc_value=readl(adc_virtual_address + ADCDAT0) & 0x3ff;
	
	if(count > buffer_len)
		len=buffer_len;
	else
		len=count;

	sprintf(buffer,"adc value=%d\n",adc_value);
        len=strlen(buffer); 
	ret_len=copy_to_user(user_ptr,buffer,len);
	up(&adc_lock);
	return (len-ret_len);
}



static struct file_operations adc_char_drv_fops = 
{
	.owner = THIS_MODULE,
	.open = adc_char_drv_open,
	.release = adc_char_drv_release,
	.read = adc_char_drv_read,
	
};
static int adc_drv_module_init(void)
{
	int ret=0;
	printk("welcome to char drv module\n");
	
	ret=alloc_chrdev_region(&char_dev,0,count,"sample char drv");
	if(ret < 0)
		goto fail0;
	adc_char_cdev=cdev_alloc();
	if(adc_char_cdev==NULL)
		goto fail1;
	cdev_init(adc_char_cdev,&adc_char_drv_fops);
	ret=cdev_add(adc_char_cdev,char_dev,count);
	if(ret < 0)
		goto fail2;

	buffer=kmalloc(GFP_KERNEL,buffer_len);
	if(buffer==NULL)
		goto fail3;

	major_num=MAJOR(char_dev);
	minor_num=MINOR(char_dev);

	printk("char drv module loaded suceessfully..major=%d minor=%d\n",major_num,minor_num);

	sema_init(&adc_lock,1);
	//step 2:
	adc_virtual_address=ioremap(ADC_BASE_ADDRESS,ADC_ADDRESS_LEN); 
	if(adc_virtual_address==NULL)
		goto fail3;
		
	return 0;

fail3:
fail2:
	cdev_del(adc_char_cdev);
fail1:
	unregister_chrdev_region(char_dev,count);
fail0:
	return -1;
}

static void adc_drv_module_exit(void)
{
	printk("exit the adc char drv module\n");
	iounmap(adc_virtual_address);
	cdev_del(adc_char_cdev);
	unregister_chrdev_region(char_dev,count);
	kfree(buffer);
	buffer=NULL;
	return;
}

module_init(adc_drv_module_init);
module_exit(adc_drv_module_exit);
MODULE_AUTHOR("Alex");
MODULE_DESCRIPTION("adc char drv module");
MODULE_LICENSE("GPL");
