
/*  
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for macros */
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/processor.h>
#include <linux/version.h>

#include "constants.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ebunix");
MODULE_DESCRIPTION("DMX512 dummy driver");

#define DRIVER_MAJOR 100 // TODO: make this dynamic by reading /proc/devices
#define MAX_UNIVERSES 16

struct dmx512data {
	struct cdev cdev;
	struct device* device;
	unsigned char slots[512];
};

struct dmx512data universeData[MAX_UNIVERSES];
struct class* moduleClass;

static int dmx_open(struct inode *inode, struct file *file)
{
    struct dmx512data *data;
    data = container_of(inode->i_cdev, struct dmx512data, cdev);
    file->private_data = data;
	return 0;
}

static ssize_t dmx_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset)
{
    struct dmx512data *data = (struct dmx512data*)file->private_data;
	ssize_t length = min((loff_t)512 - *offset, (loff_t)size);
	if (length <= 0 || *offset >= 512) {
		return 0;
	}
	if (copy_to_user(user_buffer, data->slots + *offset, length)) {
		return -EFAULT;
	}
	return length;
}

static ssize_t dmx_write(struct file *file, const char __user *user_buffer, size_t size, loff_t * offset)
{
    struct dmx512data *data = (struct dmx512data*)file->private_data;
	ssize_t length = min((loff_t)512 - *offset, (loff_t)size);
	if (length <= 0 || *offset >= 512) {
		return 0;
	}
	if (copy_from_user(data->slots + *offset, user_buffer, length)) {
		return -EFAULT;
	}
	return length;
}

static int dmx_release(struct inode* inode, struct file* file)
{
	return 0;
}

const struct file_operations dmx_fops = {
    .owner = THIS_MODULE,
    .open = dmx_open,
    .read = dmx_read,
    .write = dmx_write,
    .release = dmx_release,
};

static int __init init_dmx_module(void)
{
	int err;
	int i;

	printk(KERN_INFO "[" DRIVER_NAME "] loading\n");

    err = register_chrdev_region(MKDEV(DRIVER_MAJOR, 0), MAX_UNIVERSES, "/dev/dmx");
	if (err) {
		printk(KERN_ERR "[" DRIVER_NAME "] register_chrdev_region failed\n");
		return err;
	}

	moduleClass = class_create(THIS_MODULE, "dmxclass");
    for(i = 0; i < MAX_UNIVERSES; i++) {
		dev_t dev = MKDEV(DRIVER_MAJOR, i);
        cdev_init(&universeData[i].cdev, &dmx_fops);
        err = cdev_add(&universeData[i].cdev, dev, 1);
		if (err) {
			printk(KERN_ERR "[" DRIVER_NAME "] cdev_add failed\n");
			return err;
		}
		universeData[i].device = device_create(moduleClass, 0, dev, 0, "dmx%i", i);
    }

	return 0;
}

static void __exit cleanup_dmx_module(void)
{
	int i;
	
	printk(KERN_INFO "[" DRIVER_NAME "] unloading\n");

    for(i = 0; i < MAX_UNIVERSES; i++) {
		dev_t dev = MKDEV(DRIVER_MAJOR, i);
        cdev_del(&universeData[i].cdev);
		device_destroy(moduleClass, dev);
    }
	unregister_chrdev_region(MKDEV(DRIVER_MAJOR, 0), MAX_UNIVERSES);
	class_destroy(moduleClass);

	printk(KERN_INFO "[" DRIVER_NAME "] goodbye\n");
}


module_init(init_dmx_module);
module_exit(cleanup_dmx_module);
