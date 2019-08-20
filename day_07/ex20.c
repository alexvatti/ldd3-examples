/*
ex20: how to register irq with handler
caution: NOTE: this will not run on desktop
Please find the proper irq number to run on desktop,any embedded
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#define SHARED_IRQ 19 //sample irq number
static int irq = SHARED_IRQ, my_dev_id, irq_counter = 0;
module_param (irq, int, S_IRUGO);

static irqreturn_t my_interrupt (int irq, void *dev_id)
{
    irq_counter++;
    printk (KERN_INFO "In the ISR: counter = %d\n", irq_counter);
    return IRQ_NONE;            /* we return IRQ_NONE because we are just observing */
}

static int __init my_irq_init (void)
{
    if (request_irq(irq, my_interrupt, IRQF_SHARED, "my_interrupt", &my_dev_id))
        return -1;
    printk (KERN_INFO "Successfully loading ISR handler\n");
    return 0;
}

static void __exit my_irq_exit (void)
{
    synchronize_irq (irq);
    free_irq (irq, &my_dev_id);
    printk (KERN_INFO "Successfully unloading \n");
}

module_init (my_irq_init);
module_exit (my_irq_exit);

MODULE_AUTHOR ("Alex");
MODULE_DESCRIPTION ("Interrupt Handler Test");
MODULE_LICENSE ("GPL");

