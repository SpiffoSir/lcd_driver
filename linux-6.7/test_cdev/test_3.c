#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#define Device_Name "test"

dev_t dev_num;

struct class *class;
struct device *device;

static int test_cdev_open (struct inode *inode, struct file *file)
{
  printk("opening success\n");
  return 0;
}

static ssize_t test_cdev_read (struct file *file, char __user *buf, size_t size, loff_t *loff)
{
  printk("reading success\n");
  return 0;
}

static ssize_t test_cdev_write (struct file *file, const char __user *buf, size_t size, loff_t *loff)
{
  printk("writting success\n");
  return 0;
}

int test_cdev_release (struct inode *inode, struct file *file)
{
  printk("releasing success\n");
  return 0;
}

static struct file_operations test_fops=
{
  .owner = THIS_MODULE,
  .open = test_cdev_open,
  .read = test_cdev_read,
  .write = test_cdev_write,
  .release = test_cdev_release

};
static struct cdev test_cdev=
{
  .owner = THIS_MODULE
};

static int charactor_init(void)    //初始化函数
{
  int ret;
  
  ret = alloc_chrdev_region(&dev_num, 0, 1, Device_Name);   //动态申请设备号  设备号（地址），次设备号基址，个数，设备名
  if(ret < 0)
  {
    printk("alloc register faild\n");
  }
  printk("alloc register success\n");

  cdev_init(&test_cdev, &test_fops);  //初始化、添加  注意顺序
  cdev_add(&test_cdev, dev_num, 1);

  class = class_create("test");   //创建设备节点
  device = device_create(class, NULL, dev_num, NULL, "test");

  return 0;
}

static void charactor_exit(void)
{
  cdev_del(&test_cdev);          //注意顺序
  unregister_chrdev_region(dev_num, 1);
  device_destroy(class, dev_num);
  class_destroy(class);
  printk("deleting finished\n");
}

module_init(charactor_init);
module_exit(charactor_exit);
MODULE_LICENSE("GPL");