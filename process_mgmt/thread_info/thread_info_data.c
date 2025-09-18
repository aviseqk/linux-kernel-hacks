/* This program is to show the use case of `current_thread_info()` macro to get current process' thread_info struct
 *
 * thread_info struct is generally not required unless low-level thread control or debugging of scheduling flags or preemption is in the picture.
 *
 * otherwise all things process and thread are managed through the `current` value that CPU provides at all time for the calling process
 * */


/* NOTE:  the struct thread_info is now just a low-level per thread control block which has fields like flags, scheduling, cpu and status
 * post kernel version > 3.9 | 5.0, even the support for `task` field which was a pointer to the process' task_struct is removed, it now mostly only has fields like cpu, status, flags (preempt_count on some archs, not on x86)
 * */


#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <asm/thread_info.h>

static int __init my_module_init(void) {
    printk(KERN_INFO "module says hi to kernel\n");
   
    // NOTE: kernel gives this current task's task_struct in a macro called `current`
    printk(KERN_INFO "Task: PID - %d  Comm - %s\n", current->pid, current->comm);

    struct thread_info *thr_info = current_thread_info();
    printk(KERN_INFO "Thread Info: status %u cpu: %u flags:  0x%lx\n", thr_info->status, thr_info->cpu, thr_info->flags);
    return 0;
}

static void __exit my_module_exit(void) {
    printk(KERN_INFO "kernel says goodbye to module\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhishek Kumar");
MODULE_DESCRIPTION("A simple task_struct module detail info module");
MODULE_VERSION("0.1");
