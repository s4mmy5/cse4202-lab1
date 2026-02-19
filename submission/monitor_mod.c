/*
 * monitor_mod.c: This module interval parameters and prints out a log every
 * time said interval elapses. It also makes use of a single kernel thread
 * for the logging process.
 * AUTHORS:
 *  1. Arnav Sreenivasan <a.sreenivasan@wustl.edu>
 *  2. Jonathan Rodriguez Gomez <j.rodriguezgomez@wustl.edu>
 */

#include <linux/errno.h>
#include <linux/hrtimer.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/types.h>

// parameters
static unsigned long log_sec = 1;
static unsigned long log_nsec = 0;
module_param(log_sec, ulong, 0);
module_param(log_nsec, ulong, 0);

// timer values
static ktime_t interval;
static struct hrtimer timer;

// thread values
static struct task_struct *thread_task;

int thread_runner(void *data) {
  do {
    printk(KERN_ALERT "Loop reached!\n"
           "Number of Voluntary Context Switches: %ld\n"
           "Number of InVoluntary Context Switches: %ld\n",
           current->nvcsw,
           current->nivcsw);
    set_current_state(TASK_INTERRUPTIBLE);
    schedule();
  } while (!kthread_should_stop());

  printk(KERN_ALERT "Loop terminated, exiting thread!\n");
  return 0;
}

enum hrtimer_restart timer_callback(struct hrtimer *timer_ptr) {
  wake_up_process(thread_task);
  printk(KERN_ALERT "timer restarted.\n");
  hrtimer_forward_now(timer_ptr, interval);
  return HRTIMER_RESTART;
}

/* init function - logs that initialization happened, returns success */
static int monitor_init(void) {
  thread_task = NULL;
  thread_task = kthread_run(thread_runner, NULL, "thread_runner()");

  if (!thread_task) {
    return -EAGAIN;
  }

  interval = ktime_set(log_sec, log_nsec);
  hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
  timer.function = &timer_callback;
  hrtimer_start(&timer, interval, HRTIMER_MODE_REL);

  printk(KERN_ALERT "monitor module initialized. log_sec=%lu, log_nsec=%lu\n", log_sec, log_nsec);
  return 0;
}

/* exit function - logs that the module is being removed */
static void monitor_exit(void) {
  hrtimer_cancel(&timer);
  kthread_stop(thread_task);

  printk(KERN_ALERT "monitor module is being unloaded\n");
}

module_init(monitor_init);
module_exit(monitor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CSE 4202 Team");
MODULE_DESCRIPTION("Lab 1");
