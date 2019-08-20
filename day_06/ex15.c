/*
ex15: timer list example
*/

#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/timer.h>

static struct timer_list my_timer;

static void my_timer_function(unsigned long data){
	printk("Time up...\n");
}

static int timer_init_module(void)
{
	printk("welcome to Timer Example\n");

	//  -- initialize the timer
	init_timer(&my_timer);
	my_timer.expires = jiffies + HZ ;
	my_timer.function = my_timer_function;
	my_timer.data = 100;

	// -- TIMER START
	add_timer(&my_timer);
	printk("Timer init module\n");
	return 0;
}

static void timer_exit_module(void)
{
	del_timer(&my_timer);
	printk("Timer example exit\n");
}

module_init(timer_init_module);
module_exit(timer_exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex");
MODULE_DESCRIPTION("A timer example.");
