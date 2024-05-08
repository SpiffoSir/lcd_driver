#include <linux/module.h>
#include <linux/fs.h>

static int major = 0;
static struct file_operations test_2_fops =
{
  .owner = THIS_MODULE
};

int __init HelloWrold_Init(void)
{
  printk("hello-init\n");
  major = register_chrdev(0,"test_2",&test_2_fops);
  return 0;
}

void __exit HelloWrold_Exit(void)
{
  printk("hello-clean\n");
}

module_init(HelloWrold_Init);
module_exit(HelloWrold_Exit);


MODULE_LICENSE("GPL");