/*
ex1 is basic modulde based on the chapter 2 of ldd3 book
*/
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>

static int ex1_module_init(void)
{
	printk("welcome to hello world ex module\n");
	return 0;
}

static void ex1_module_exit(void)
{
	printk("exit the hello world ex module\n");
	return;
}
module_init(ex1_module_init);
module_exit(ex1_module_exit);
MODULE_AUTHOR("Alex");
MODULE_DESCRIPTION("Ex1 module");
MODULE_LICENSE("GPL");
