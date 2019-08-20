/*
ex19: shows the memory allocation using kmalloc, vmalloc
*/
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/slab.h>
#include<linux/vmalloc.h>

char *kptr=NULL;
char *vptr=NULL;

static int memory_module_init(void)
{
	vptr=(char*)vmalloc(sizeof(char)*1000);
	if(vptr==NULL)
		return -ENOMEM;
	printk("virtual address=%x physical address=%x\n",(unsigned long)vptr,(unsigned long)virt_to_phys(vptr));
	printk("memory allocation passed...vmalloc\n");

	kptr=(char*)kmalloc(sizeof(char)*1000,GFP_KERNEL);
	if(kptr==NULL)
		return -ENOMEM;
	printk("virtual address=%x physical address=%x\n",(unsigned long)kptr,(unsigned long)virt_to_phys(kptr));
	printk("memory allocation passed..kmalloc\n");
	return 0;
}

static void memory_module_exit(void)
{
	vfree(vptr);
	kfree(kptr);
	printk("memory module exit...\n");
	
}

module_init(memory_module_init);
module_exit(memory_module_exit);

MODULE_AUTHOR("Alex");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Memory allocation example");




