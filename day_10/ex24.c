/*
ex24 is basic bus type driver module based on the chapter 14 of ldd3 book
*/

#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/usb.h>
#include<linux/interrupt.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/uaccess.h>
#include "lddbus.h"

static char *Version = "1.9";

static void ldd_bus_release(struct device *dev)
{
        printk(KERN_DEBUG "lddbus release\n");
}
struct device ldd_bus = {
        .init_name = "ldd0",
        .release  = ldd_bus_release
};

static void ldd_dev_release(struct device *dev)
{ }

int ldd_register_device(struct ldd_device *ldddev)
{
	ldddev->dev.bus = &ldd_bus_type;
	ldddev->dev.parent = &ldd_bus;
	ldddev->dev.release = ldd_dev_release;
	dev_set_name(&ldddev->dev, "ldd0");
	return device_register(&ldddev->dev);
}
EXPORT_SYMBOL_GPL(ldd_register_device);


void ldd_unregister_device(struct ldd_device *ldddev)
{
	device_unregister(&ldddev->dev);
}
EXPORT_SYMBOL_GPL(ldd_unregister_device);

static ssize_t show_version(struct device_driver *driver, char *buf)
{
	struct ldd_driver *ldriver = to_ldd_driver(driver);

	sprintf(buf, "%s\n", ldriver->version);
	return strlen(buf);
}

int ldd_register_driver(struct ldd_driver *driver)
{
	int ret;
	
	driver->driver.bus = &ldd_bus_type;
	ret = driver_register(&driver->driver);
	if (ret)
		return ret;
	driver->version_attr.attr.name = "version";
	driver->version_attr.attr.mode = S_IRUGO;
	driver->version_attr.show = show_version;
	driver->version_attr.store = NULL;
	return driver_create_file(&driver->driver, &driver->version_attr);
}

void ldd_unregister_driver(struct ldd_driver *driver)
{
	driver_remove_file(&driver->driver, &driver->version_attr);
	driver_unregister(&driver->driver);
}
EXPORT_SYMBOL_GPL(ldd_register_driver);
EXPORT_SYMBOL_GPL(ldd_unregister_driver);



/*
 * Respond to udev events.
 */
static int ldd_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	if (add_uevent_var(env, "LDDBUS_VERSION=%s", Version))
		return -ENOMEM;

	return 0;
}

/*
 * Match LDD devices to drivers.  Just do a simple name test.
 */
static int ldd_match(struct device *dev, struct device_driver *driver)
{
	return !strncmp(dev_name(dev), driver->name, strlen(driver->name));
}

/*
 * Export a simple attribute.
 */
static ssize_t show_bus_version(struct bus_type *bus, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%s\n", Version);
}

static BUS_ATTR(version, S_IRUGO, show_bus_version, NULL);
/*
 * And the bus type.
 */
struct bus_type ldd_bus_type = {
	.name = "ldd",
	.match = ldd_match,
	.uevent  = ldd_uevent,
};

static int bus_module_init(void)
{
	int ret=0;
	printk("sample bus module driver init\n");
	ret=bus_register(&ldd_bus_type);
	if (ret)
		return ret;
	if (bus_create_file(&ldd_bus_type, &bus_attr_version))
		printk(KERN_NOTICE "Unable to create version attribute\n");

	ret = device_register(&ldd_bus);
	if (ret)
		printk(KERN_NOTICE "Unable to register ldd0\n");
	return ret;
}
static void bus_module_exit(void)
{
	printk("sample bus module driver exit\n");
	device_unregister(&ldd_bus);
	bus_remove_file(&ldd_bus_type, &bus_attr_version);
	bus_unregister(&ldd_bus_type);
}

module_init(bus_module_init);
module_exit(bus_module_exit);
MODULE_AUTHOR("Alex");
MODULE_DESCRIPTION("Ex24 bus type sample module");
MODULE_LICENSE("GPL");
