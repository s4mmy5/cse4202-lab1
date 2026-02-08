/* simple_module.c - a simple template for a loadable kernel module in Linux,
   based on the hello world kernel module example on pages 338-339 of Robert
   Love's "Linux Kernel Development, Third Edition."
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>

static unsigned long log_sec = 1;
static unsigned long log_nsec = 0;
module_param(log_sec,  ulong, 0);
module_param(log_nsec, ulong, 0);

/* init function - logs that initialization happened, returns success */
static int
simple_init(void)
{
    printk(KERN_ALERT "simple module initialized with error\n");
    return 0;
}

/* exit function - logs that the module is being removed */
static void
simple_exit(void)
{
    printk(KERN_ALERT "simple module is being unloaded\n");
    printk(KERN_ALERT "%lu %lu\n", log_sec, log_nsec);
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("CSE 4202 Team");
MODULE_DESCRIPTION ("Lab 1");
