/*
ex28 is basic  module with dma_alloc_coherent,dma_map_single
 implemented,based on the chapter 15 of ldd3 book
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>

static int direction = PCI_DMA_BIDIRECTIONAL;

static char *kbuf;
static dma_addr_t handle;
static size_t size = (10 * PAGE_SIZE);

static void output (char *kbuf, dma_addr_t handle, size_t size, char *string)
{
    unsigned long diff;
    diff = (unsigned long)kbuf - handle;
    printk (KERN_INFO "kbuf=%12p, handle=%12p, size = %d\n", kbuf,(unsigned long *)handle, (int)size);
    printk (KERN_INFO "(kbuf-handle)= %12p, %12lu, PAGE_OFFSET=%12lu, compare=%lu\n",
            (void *)diff, diff, PAGE_OFFSET, diff - PAGE_OFFSET);
    strcpy (kbuf, string);
    printk (KERN_INFO "string written was, %s\n", kbuf);
}

static int __init dma_module_init(void)
{

    /* dma_alloc_coherent method */
    printk (KERN_INFO "Loading DMA allocation test module\n");
    printk (KERN_INFO "\nTesting dma_alloc_coherent()..........\n\n");
    kbuf = dma_alloc_coherent (NULL, size, &handle, GFP_KERNEL);
    output (kbuf, handle, size, "This is the dma_alloc_coherent string");
    dma_free_coherent (NULL, size, kbuf, handle);

    /* dma_map/unmap_single */
    printk (KERN_INFO "\nTesting dma_map_single()................\n\n");
    kbuf = kmalloc (size, GFP_KERNEL);
    handle = dma_map_single (NULL, kbuf, size, direction);
    output (kbuf, handle, size, "This is the dma_map_single string");
    dma_unmap_single (NULL, handle, size, direction);
    kfree (kbuf);

    return 0;
}
static void __exit dma_module_exit (void)
{
    printk (KERN_INFO "DMA Module Unloading\n");
}

module_init(dma_module_init);
module_exit(dma_module_exit);

MODULE_AUTHOR ("Alex");
MODULE_DESCRIPTION ("EX:28 DMA interface test");
MODULE_LICENSE ("GPL");
