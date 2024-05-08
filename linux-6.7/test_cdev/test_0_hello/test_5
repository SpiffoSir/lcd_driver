#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#define Device_Name "my_chrdev"

static int major = 0;
static int minor = 0;

module_param(major, int, S_IRUGO);   //S_IRUGO  只读权限
module_param(minor, int, S_IRUGO);

dev_t dev_num;

static int moduleparam_init(void)
{
  int ret;

  if(major)
  {
    printk("major is %d\n", major);
    printk("minor is %d\n", minor);
    dev_num = MKDEV(major, minor);    //组合设备号,此时相当于设备号以及手动设置好了
    ret = register_chrdev_region(dev_num, 1, Device_Name);   //静态申请设备号 设备号，申请几个，设备名
    if(ret < 0)
    {
      printk("register failed");
    }
  }
  else
  {
    ret = alloc_chrdev_region(&dev_num, 0, 1, Device_Name);   //动态申请设备号  设备号（地址），次设备号基址，个数，设备名
    if(ret < 0)
    {
      printk("alloc register faild");
    }
    else
    {
      printk("alloc register success");
      major = MAJOR(dev_num);
      minor = MINOR(dev_num);
      printk("alloc major is %d\n", major);
      printk("alloc minor is %d\n", minor);
      
    }
  }

  return 0;
}

static void moduleparam_exit(void)
{
  unregister_chrdev_region(dev_num, 1);
  printk(KERN_INFO "unregister finished");
}

module_init(moduleparam_init);
module_exit(moduleparam_exit);
MODULE_LICENSE("GPL");