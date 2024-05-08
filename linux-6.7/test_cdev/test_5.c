#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#define Device_Name "test"


struct device_test
{
  dev_t dev_num;
  struct cdev test_cdev;
  struct class *class;
  struct device *device;
  char kbuf[32];
};

struct device_test dev1 = 
{
  .kbuf = "1"
};
struct device_test dev2 = 
{
  .kbuf = "2"
};


static int test_cdev_open (struct inode *inode, struct file *file)
{
  file->private_data = container_of(inode->i_cdev, struct device_test, test_cdev);
  printk("opening success\n");
  return 0;
}


static ssize_t test_cdev_read (struct file *file, char __user *buf, size_t size, loff_t *loff)
{

  if(copy_to_user(buf, ((struct device_test *)file->private_data)->kbuf, size) != 0)       //直接引用成员是可以的
  { 
    printk("copy fialed");
    return -1;
  }
  printk("reading success\n");
  return 0;
}


static ssize_t test_cdev_write (struct file *file, const char __user *buf, size_t size, loff_t *loff)
{
  if(copy_from_user(((struct device_test *)file->private_data)->kbuf, buf, size) != 0)
  {
    printk("write failed");
    return -1;
  }
  printk("get %s\n",((struct device_test *)file->private_data)->kbuf);
  printk("writting success\n");
  return 0;
}


int test_cdev_release (struct inode *inode, struct file *file)
{
  printk("releasing %s success\n", ((struct device_test *)file->private_data)->kbuf);
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


static int charactor_init(void)    //初始化函数
{
  int ret;
  
  ret = alloc_chrdev_region(&dev1.dev_num, 0, 2, Device_Name);   //动态申请设备号  设备号（地址），次设备号基址，个数，设备名
  printk("alloc register success\n");

  cdev_init(&dev1.test_cdev, &test_fops);  //初始化、添加  注意顺序
  cdev_add(&dev1.test_cdev, dev1.dev_num, 1);

  dev1.class = class_create("test1");   //创建设备节点
  dev1.device = device_create(dev1.class, NULL, dev1.dev_num, NULL, "test1");


  cdev_init(&dev2.test_cdev, &test_fops);  //初始化、添加  注意顺序
  cdev_add(&dev2.test_cdev, dev1.dev_num + 1, 1);

  dev2.class = class_create("test2");   //创建设备节点
  dev2.device = device_create(dev1.class, NULL, dev1.dev_num + 1, NULL, "test2");

  return 0;
}


static void charactor_exit(void)
{
  cdev_del(&dev1.test_cdev);          //注意顺序
  cdev_del(&dev2.test_cdev);
  unregister_chrdev_region(dev1.dev_num, 1);
  unregister_chrdev_region(dev1.dev_num + 1, 1);
  device_destroy(dev1.class, dev1.dev_num);
  device_destroy(dev2.class, dev1.dev_num + 1);
  class_destroy(dev1.class);
  class_destroy(dev2.class);
  printk("deleting finished\n");
}


module_init(charactor_init);
module_exit(charactor_exit);
MODULE_LICENSE("GPL");