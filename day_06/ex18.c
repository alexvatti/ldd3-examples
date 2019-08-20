/*
ex18: tasklet example */

#include<linux/module.h>
#include<linux/init.h>
#include<linux/interrupt.h>
#include<linux/kernel.h>

static void my_tasklet_handler(unsigned long flag);
static struct tasklet_struct my_tasklet;

static void my_tasklet_handler(unsigned long flag)
{
	tasklet_disable(&my_tasklet);
	printk("my_tasklet run: do what the tasklet want to do....\n");
	tasklet_enable(&my_tasklet);
}

static int tasklet_init_module(void)
{
	printk("module init start. \n");
	printk("Hello tasklet!\n");
	tasklet_init(&my_tasklet,my_tasklet_handler,0);
	tasklet_schedule(&my_tasklet);
	printk("module init end.\n");
	return 0;
}

static void tasklet_exit_module(void)
{
	tasklet_kill(&my_tasklet);
	printk("Goodbye, tasklet!\n");
}

module_init(tasklet_init_module);
module_exit(tasklet_exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex");
