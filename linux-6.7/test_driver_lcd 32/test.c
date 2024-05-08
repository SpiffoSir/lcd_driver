#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/clk.h>
#include <linux/fb.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/pm_runtime.h>
#include <linux/platform_data/video_s3c.h>

static struct fb_info *my_fbinfo;
static struct fb_ops myfb_ops = 
{
  .owner = THIS_MODULE,
  .fb_fillrect = cfb_fillrect,  //通过软件实现操作函数 
  .fb_copyarea = cfb_copyarea,
  .fb_imageblit = cfb_imageblit,
}

/*入口*/
int __init lcd_drv_init(void)
{
  dma_addr_t phy_addr;
  /*分配*/
  myfb_info = framebuffer_alloc(0, BULL);
  /*设置fb_info*/
  /*设置var*/
  myfb_info->var.xres = 1024;	
  myfb_info->var.yres = 600;
  myfb_info->var.bits_per_pixel = 16; //RGB565
  myfb_info->var.red.offset = 11; //RGB,从右到左B开始，所以R的第一位需要偏移 5+6=11位
  myfb_info->var.red.length = 5;
  
  myfb_info->var.green.offset = 5;
  myfb_info->var.green.length = 6;

  myfb_info->var.blue.offset = 0;
  myfb_info->var.blue.length = 5;
  	
  /*设置fix*/
  myfb_info->fix.smem_len = (myfb_info->var.xres * myfb_info->var.yres) * 2  //需要十六位表示，而一个字节8位
  if(mybd_info->var.bits_per_pixel == 23)
  {
    mybd_info->fix.smem_len = (myfb_info->var.xres * myfb_info->var.yres) * 4  //24bpp用32位表示
  }
  myfb_info->fix.type = FB_TYPE_PACKED_PIXELS;
  mgfb_info->fix.visual = FB_VISUAL_TRUECOLOR;
  myfb_info->screen_base = dma_alloc_wc(NULL , myfb_info->fix.smem_len , &phy_addr , GFP_KERNEL);
  myfb_info->fix.smem_start = phy_addr;

  /*设置fbops*/
  myfb_info->fbops = &myfb_ops;
  /*注册*/
  register_framebuffer(myfb_info);
  return 0;
}

/*出口*/
static void __exit lcd_drv_exit(void)
{
  /*取消注册*/
  unregister_framebuffer(myfb_info);
  /*取消分配*/
  framebuffer_release(myfb_info);
  ;
}

module_init(lcd_drv_init);
module_exit(lcd_drv_exit);
MODULE_AUTHOR("SpiffoSir");
MODULE_DESCRIPTION("lcd driver test");
MODULE_LICENSE("GPL");