/*
ex16: work queue example 
*/

#include<linux/module.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/time.h>
#include<linux/delay.h>
#include<linux/wait.h>
#include<linux/workqueue.h>

/* declare a work queue*/
struct work_struct workq;
void my_workqueue_handler(struct work_struct *work)
{
	printk("WORK QUEUE: Entered into handler. \n");
	msleep(10000);  /* sleep */
	printk("WORK QUEUE: Exit of the handler. \n");
}

static int work_queue_init_module(void)
{
	printk("work queue example ....\n");

	// -- initialize the work queue
	INIT_WORK(&workq, my_workqueue_handler);
	schedule_work(&workq);

	return 0;
}

void work_queue_exit_module(void)
{
	printk("work queue exit module \n");

}
module_init(work_queue_init_module);
module_exit(work_queue_exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ALEX");
MODULE_DESCRIPTION("A Work queue example");
