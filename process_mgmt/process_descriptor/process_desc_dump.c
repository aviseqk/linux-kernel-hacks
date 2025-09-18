#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/pid.h>

static void display_task_details(pid_t pid) {
    struct task_struct *task;

    task = pid_task(find_get_pid(pid), PIDTYPE_PID);
    if (!task) {
        printk(KERN_INFO "PID %d not found\n", pid);
        return;
    }

    printk(KERN_INFO "------TASK INFO DUMP------\n");
    printk(KERN_INFO "\tPID: %d | TGID: %d | PPID: %d\n",
           task->pid, task->tgid,
           task->real_parent ? task->real_parent->pid : -1);

    printk(KERN_INFO "\tComm: %s | State: %d\n", task->comm, task->__state);
    printk(KERN_INFO "\tPriority: %d | Static_Priority: %d | Normal_Priority: %d\n",
           task->prio, task->static_prio, task->normal_prio);

}

static int __init my_module_init(void) {
    printk(KERN_INFO "module says hi to kernel\n");
   
    // NOTE: kernel gives this current task's task_struct in a macro called `current`
    printk(KERN_INFO "Task: PID - %d  Comm - %s\n", current->pid, current->comm);

    display_task_details(current->pid);
    printk(KERN_INFO "###### init task ######\n");
    display_task_details(1);
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
