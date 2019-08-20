/*
ex26 is basic bus type driver module based on the chapter 14 of ldd3 book
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

static struct ldd_driver sample_ldd_driver= {
	.version="1.2",
	.driver = {
		.name="ldd0",
	},
};
static int ldd_driver_module_init(void)
{
	int ret=0;
	printk("sample ldd driver module  init\n");
	ret=ldd_register_driver(&sample_ldd_driver);
	return ret;
}
static void ldd_driver_module_exit(void)
{
	printk("sample ldd driver module exit\n");
	ldd_unregister_driver(&sample_ldd_driver);
}

module_init(ldd_driver_module_init);
module_exit(ldd_driver_module_exit);
MODULE_AUTHOR("Alex");
MODULE_DESCRIPTION("Ex26 ldd bus,sample driver module");
MODULE_LICENSE("GPL");
