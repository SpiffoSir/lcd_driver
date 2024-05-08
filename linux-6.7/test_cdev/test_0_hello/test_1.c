#include <linux/module.h>
#include <linux/fs.h>

static struct file_operations test_1_fops =
{
  .owner = THIS_MODULE
};

int init_module(void)
{
  ;
  return 0;
}

void cleanup_module(void)
{
  ;
}

MODULE_LICENSE("GPL");