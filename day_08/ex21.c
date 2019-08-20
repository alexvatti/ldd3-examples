/*
ex21: is basic module based on the chapter 11 of ldd3 book
data types,it's size
*/
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>

static int ex_module_init(void)
{
	printk("welcome to hello world ex module\n");
	printk("char=%ld\n",sizeof(char));
	printk("int=%ld\n",sizeof(int));
	printk("short=%ld\n",sizeof(short));
	printk("long=%ld\n",sizeof(long));
	printk("long long=%ld\n",sizeof(long long));
	printk("size_t=%ld\n",sizeof(size_t));
	printk("loff_t=%ld\n",sizeof(loff_t));
	printk("pid_t=%ld\n",sizeof(pid_t));
	return 0;	
}

static void ex_module_exit(void)
{
	printk("exit the hello world ex module\n");
	return;
}
module_init(ex_module_init);
module_exit(ex_module_exit);
MODULE_AUTHOR("Alex");
MODULE_DESCRIPTION("Ex module to know the data type size(memory)");
MODULE_LICENSE("GPL");
