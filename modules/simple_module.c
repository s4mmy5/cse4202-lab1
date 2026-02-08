/* simple_module.c - a simple template for a loadable kernel module in Linux,
   based on the hello world kernel module example on pages 338-339 of Robert
   Love's "Linux Kernel Development, Third Edition."
 */

#include <linux/errno.h>
#include <linux/hrtimer.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/module.h>

static unsigned long log_sec = 1;
static unsigned long log_nsec = 0;
module_param(log_sec, ulong, 0);
module_param(log_nsec, ulong, 0);

static ktime_t interval;
static struct hrtimer timer;

enum hrtimer_restart timer_callback(struct hrtimer *timer_ptr) {
  printk(KERN_ALERT "timer restarted.\n");
  hrtimer_forward_now(timer_ptr, interval);
  return HRTIMER_RESTART;
}

/* init function - logs that initialization happened, returns success */
static int simple_init(void) {
  interval = ktime_set(log_sec, log_nsec);
  hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);

  timer.function = &timer_callback;
  hrtimer_start(&timer, interval, HRTIMER_MODE_REL);

  printk(KERN_ALERT "simple module initialized. log_sec=%lu, log_nsec=%lu\n", log_sec, log_nsec);
  return 0;
}

/* exit function - logs that the module is being removed */
static void simple_exit(void) {
  hrtimer_cancel(&timer);

  printk(KERN_ALERT "simple module is being unloaded\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CSE 4202 Team");
MODULE_DESCRIPTION("Lab 1");
