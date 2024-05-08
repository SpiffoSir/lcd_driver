#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define DEVICE_NAME "my_char_device"
static dev_t dev_num;
static struct cdev my_cdev;

static struct file_operations my_fops = {
    .owner = THIS_MODULE,
};

static int __init my_char_device_init(void) {
    alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    cdev_init(&my_cdev, &my_fops);
    cdev_add(&my_cdev,dev_num,1);
    return 0;
}

static void __exit my_char_device_exit(void) {
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
}

module_init(my_char_device_init);
module_exit(my_char_device_exit);
MODULE_LICENSE("GPL");
