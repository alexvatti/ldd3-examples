/*
ex23 is basic usb driver module based on the chapter 13 of ldd3 book
https://sysplay.github.io/books/LinuxDrivers/book/Content/Part14.html
*/
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/usb.h>
#include<linux/interrupt.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/uaccess.h>

#define VID 0XABCD //change here for VID
#define PID 0XABCD //change here for PID


#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define BULK_EP_OUT 0x01
#define BULK_EP_IN 0x82
#define MAX_PKT_SIZE 512

static struct usb_device *device;
static struct usb_class_driver class;
static unsigned char bulk_buf[MAX_PKT_SIZE];


static int pen_drive_open(struct inode *i, struct file *f)
{
    return 0;
}

static int pen_drive_close(struct inode *i, struct file *f)
{
    return 0;
}

static ssize_t pen_drive_read(struct file *f, char __user *buf, size_t cnt, loff_t *off)
{
    int retval;
    int read_cnt;

    /* Read the data from the bulk endpoint */
    retval = usb_bulk_msg(device, usb_rcvbulkpipe(device, BULK_EP_IN),
            bulk_buf, MAX_PKT_SIZE, &read_cnt, 5000);
    if (retval)
    {
        printk(KERN_ERR "Bulk message returned %d\n", retval);
        return retval;
    }
    if (copy_to_user(buf, bulk_buf, MIN(cnt, read_cnt)))
    {
        return -EFAULT;
    }

    return MIN(cnt, read_cnt);
}

static ssize_t pen_drive_write(struct file *f, const char __user *buf, size_t cnt,
                                    loff_t *off)
{
    int retval;
    int wrote_cnt = MIN(cnt, MAX_PKT_SIZE);

    if (copy_from_user(bulk_buf, buf, MIN(cnt, MAX_PKT_SIZE)))
    {
        return -EFAULT;
    }

    /* Write the data into the bulk endpoint */
    retval = usb_bulk_msg(device, usb_sndbulkpipe(device, BULK_EP_OUT),
            bulk_buf, MIN(cnt, MAX_PKT_SIZE), &wrote_cnt, 5000);
    if (retval)
    {
        printk(KERN_ERR "Bulk message returned %d\n", retval);
        return retval;
    }

    return wrote_cnt;
}

static struct file_operations pen_drive_file_ops =
{
    .owner = THIS_MODULE,
    .open = pen_drive_open,
    .release = pen_drive_close,
    .read = pen_drive_read,
    .write = pen_drive_write,
};

static int sample_usb_driver_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i,retval=0;

    iface_desc = interface->cur_altsetting;
    printk(KERN_INFO "Pen i/f %d now probed: (%04X:%04X)\n",
            iface_desc->desc.bInterfaceNumber,
            id->idVendor, id->idProduct);
    printk(KERN_INFO "ID->bNumEndpoints: %02X\n",
            iface_desc->desc.bNumEndpoints);
    printk(KERN_INFO "ID->bInterfaceClass: %02X\n",
            iface_desc->desc.bInterfaceClass);

    for (i = 0; i < iface_desc->desc.bNumEndpoints; i++)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n",
                i, endpoint->bEndpointAddress);
        printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n",
                i, endpoint->bmAttributes);
        printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n",
                i, endpoint->wMaxPacketSize,
                endpoint->wMaxPacketSize);
    }

    device = interface_to_usbdev(interface);

    class.name = "usb/pen%d";
    class.fops = &pen_drive_file_ops;
    if ((retval = usb_register_dev(interface, &class)) < 0)
    {
        /* Something prevented us from registering this driver */
        printk(KERN_ERR "Not able to get a minor for this device.");
    }
    else
    {
        printk(KERN_INFO "Minor obtained: %d\n", interface->minor);
    }

    return retval;
    
}

static void sample_usb_driver_remove(struct usb_interface *interface)
{
    printk(KERN_INFO "Pen i/f %d now disconnected\n",
            interface->cur_altsetting->desc.bInterfaceNumber);
    usb_deregister_dev(interface, &class);
}
static struct usb_device_id ids[]= {

	{USB_DEVICE(VID,PID)},
	{ 0, },
};

MODULE_DEVICE_TABLE(usb,ids);

static struct usb_driver sample_usb_driver = {
	.name = "usb sample driver",
	.probe = sample_usb_driver_probe,
	.disconnect = sample_usb_driver_remove,
	.id_table = ids,

};
static int usb_driver_module_init(void)
{
	printk("sample usb driver init\n");
	return(usb_register(&sample_usb_driver));

}
static void usb_driver_module_exit(void)
{
	printk("sample usb driver exit\n");
	usb_deregister(&sample_usb_driver);
}

module_init(usb_driver_module_init);
module_exit(usb_driver_module_exit);
MODULE_AUTHOR("Alex");
MODULE_DESCRIPTION("Ex23 usb driver sample module");
MODULE_LICENSE("GPL");
