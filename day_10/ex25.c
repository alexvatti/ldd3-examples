/*
ex25 is basic bus type driver module based on the chapter 14 of ldd3 book
*/
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/usb.h>
#include<linux/interrupt.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/uaccess.h>
#include "lddbus.h"

static struct ldd_device sample_ldd_device= {
	.name="ldd sample",
};
static int ldd_device_module_init(void)
{
	int ret=0;
	printk("sample ldd device module  init\n");
	ret=ldd_register_device(&sample_ldd_device);
	return ret;
}
static void ldd_device_module_exit(void)
{
	printk("sample ldd device module exit\n");
	ldd_unregister_device(&sample_ldd_device);
}

module_init(ldd_device_module_init);
module_exit(ldd_device_module_exit);
MODULE_AUTHOR("Alex");
MODULE_DESCRIPTION("Ex25 ldd sample module");
MODULE_LICENSE("GPL");
