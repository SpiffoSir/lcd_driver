#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#define Device_Name "test"
#define GPIO_DR 0x300B04C
#define GPIO_DAT 0x300B058

struct device_test
{
  dev_t dev_num;
  struct cdev test_cdev;
  struct class *class;
  struct device *device;
  char kbuf[32];
  unsigned int *vir_gpio_dr;
  unsigned int *vir_gpio_dat;
};

struct device_test dev1 = 
{
  .kbuf = "?"
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

  if(((struct device_test *)file->private_data)->kbuf[0] == 1)
  {
    *(((struct device_test *)file->private_data)->vir_gpio_dr) |= (1 << 8);
    *(((struct device_test *)file->private_data)->vir_gpio_dr) &= ~(1 << 9);
    *(((struct device_test *)file->private_data)->vir_gpio_dr) &= ~(1 << 10);
    *(((struct device_test *)file->private_data)->vir_gpio_dat) |= (1 << 10);
    printk("%d",233);
  }
  else if(((struct device_test *)file->private_data)->kbuf[0] == 0)
  {
    *(((struct device_test *)file->private_data)->vir_gpio_dr) &= ~(1 << 8);
    *(((struct device_test *)file->private_data)->vir_gpio_dr) &= ~(1 << 9);
    *(((struct device_test *)file->private_data)->vir_gpio_dr) &= ~(1 << 10);
    *(((struct device_test *)file->private_data)->vir_gpio_dat) &= ~(1 << 10);
  }

  printk("get %d\n",((struct device_test *)file->private_data)->kbuf[0]);
  
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
  
  ret = alloc_chrdev_region(&dev1.dev_num, 0, 1, Device_Name);   //动态申请设备号  设备号（地址），次设备号基址，个数，设备名
  if(ret < 0)
  {
    printk("alloc register faild\n");
    goto err_chrdev;      //goto相当于汇编的jump跳转，到指定位置继续往下执行
  }
  printk("alloc register success\n");

  cdev_init(&dev1.test_cdev, &test_fops);  //初始化、添加  注意顺序
  ret = cdev_add(&dev1.test_cdev, dev1.dev_num, 1);
  if(ret < 0)
  {
    printk("cdev adding faild\n");
    goto err_chr_add;
  }

  dev1.class = class_create("test");   //创建设备节点
  if(IS_ERR(dev1.class))
  {
    ret = PTR_ERR(dev1.class);
    goto err_device_create;
  }
  dev1.device = device_create(dev1.class, NULL, dev1.dev_num, NULL, "test");
  if(IS_ERR(dev1.device))
  {
    ret = PTR_ERR(dev1.device);
    goto err_class_create;
  }

  dev1.vir_gpio_dr = ioremap(GPIO_DR,4);  //虚拟地址转换,四个字节就是32位
  if(IS_ERR(dev1.vir_gpio_dr))
  {
    ret = PTR_ERR(dev1.vir_gpio_dr);
    goto err_ioremap;
  }

  dev1.vir_gpio_dat = ioremap(GPIO_DR,4);
  if(IS_ERR(dev1.vir_gpio_dat))
  {
    ret = PTR_ERR(dev1.vir_gpio_dat);
    goto err_ioremap;
  }


  return 0;

  err_ioremap:
    iounmap(dev1.vir_gpio_dr);
  err_class_create:  //相当于按步撤销之前的操作
    class_destroy(dev1.class);
  err_device_create:          
    cdev_del(&dev1.test_cdev);
  err_chr_add:
    unregister_chrdev_region(dev1.dev_num, 1);
  err_chrdev:
    return ret;
}


static void charactor_exit(void)
{
  cdev_del(&dev1.test_cdev);          //注意顺序
  unregister_chrdev_region(dev1.dev_num, 1);
  device_destroy(dev1.class, dev1.dev_num);
  class_destroy(dev1.class);
  printk("deleting finished\n");
}


module_init(charactor_init);
module_exit(charactor_exit);
MODULE_LICENSE("GPL");
