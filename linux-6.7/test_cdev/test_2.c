#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#define Device_Name "test"

dev_t dev_num;
static struct file_operations test_fops=
{
  .owner = THIS_MODULE
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
    printk("alloc register faild");
  }

  printk("alloc register success");
  cdev_init(&test_cdev, &test_fops);  //初始化、添加  注意顺序
  cdev_add(&test_cdev, dev_num, 1);

  return 0;
}

static void charactor_exit(void)
{
  cdev_del(&test_cdev);          //注意顺序
  unregister_chrdev_region(dev_num, 1);
  printk("unregister finished");
}

module_init(charactor_init);
module_exit(charactor_exit);
MODULE_LICENSE("GPL");