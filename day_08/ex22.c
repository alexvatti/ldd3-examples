/*
ex22: is basic PCI driver module based on the chapter 12 of ldd3 book
*/
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/pci.h>
#include<linux/interrupt.h>
#include <linux/pci_ids.h>

#define VID 0XABCD  //change here VID 
#define PID 0XABCD  //change here PID
static long iobase[6],iosize[6];
static unsigned int *VirtualBaseAddr[6];


static int sample_pci_driver_probe(struct pci_dev *dev, const struct pci_device_id *id)
{
	int i,bar;

	//step 1: Enable pci device
	pci_enable_device(dev);

	//step 2: get BAR information
	for(i=0;i<6;i++) {
		bar=i;
		iobase[i] = pci_resource_start (dev, bar);
		iosize[i] = pci_resource_len (dev, bar);

		if (!iobase[i] || ((pci_resource_flags (dev, bar) & IORESOURCE_IO) == 0)) 
		{
        		printk("no I/O resource at PCI BAR %d\n",bar);
		}
		if (!iobase[i] || ((pci_resource_flags (dev, bar) & IORESOURCE_MEM) == 0)) 
		{
        		printk("no MEM resource at PCI BAR %d\n",bar);
		} 
		if(pci_request_region (dev, bar, "bar")){
        		printk("Request PCI BAR Region :%d failed\n",bar);
		} else {
			VirtualBaseAddr[i]=(unsigned int *)ioremap_nocache(iobase[i],iosize[i]);
		}

	}
	return 0;
}

static void sample_pci_driver_remove(struct pci_dev *dev)
{
	int i,bar;
	for(i=0;i<6;i++) {
		if(iobase[i]){
			//pcim_iounmap(dev, void __iomem *addr);
			bar=i;
			pci_release_region(dev, bar);
		}
	}
	pci_disable_device(dev);
	return;
}
static struct pci_device_id ids[]= {

	{PCI_DEVICE(VID,PID)},
	{ 0, },
};

MODULE_DEVICE_TABLE(pci,ids);
static struct pci_driver sample_pci_driver = {
	.name = "pci sample driver",
	.probe = sample_pci_driver_probe,
	.remove = sample_pci_driver_remove,
	.id_table = ids,

};
static int pci_driver_module_init(void)
{
	printk("sample pci driver init\n");
	return(pci_register_driver(&sample_pci_driver));

}
static void pci_driver_module_exit(void)
{
	printk("sample pci driver exit\n");
	pci_unregister_driver(&sample_pci_driver);
}

module_init(pci_driver_module_init);
module_exit(pci_driver_module_exit);
MODULE_AUTHOR("Alex");
MODULE_DESCRIPTION("Ex22 pci driver sample module");
MODULE_LICENSE("GPL");
