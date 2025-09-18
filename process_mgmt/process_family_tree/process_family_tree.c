/*
 * The purpose of this program/module is to use and explain how the traversals of process tree / process list happens in Linux Kernel
 *
 * The macros provided are:
 * 1. for_each_process() -> iterate over all processes system wide, the global task list
 * 2. next_task() and prec_task() -> manual low-level way to traverse through list
 * 3. list_for_each_entry(child, &p->children, sibling) -> iterate over children of a process
 * 4. list_for_each_entry(sibling, &p->parent->children, sibling) -> traverse siblings of a task
 * 5. for_each_thread() -> find all threads of a process
 *
 * */

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/pid.h>

// static void reach_init() {
// struct task_struct *task;
// }

static void has_init_as_parent(void) {
    struct task_struct *task;
    
    printk(KERN_INFO "init has following children\n");
    for_each_process(task) {
                if (task->parent == &init_task) {
                    printk(KERN_INFO "PID %d\n", task->pid);
        }
    }
}

static void print_threads_of_process(pid_t pid) {
    struct task_struct *task;
    struct task_struct *thread;

    task = pid_task(find_get_pid(pid), PIDTYPE_PID);
    for_each_thread(task, thread) {
        printk(KERN_INFO "Thread TID: %d | Comm: %s\n", thread->pid, thread->comm);
    }
}

static void print_task_children(pid_t pid) {
    struct task_struct *task;
    struct task_struct *child;

    task = pid_task(find_get_pid(pid), PIDTYPE_PID);
    list_for_each_entry(child, &task->children, sibling) {
            printk(KERN_INFO "Child PID %d | Comm %s\n", child->pid, child->comm);
    }
}

static void print_inittask_children(void) {
    struct task_struct *child;

    list_for_each_entry(child, &init_task.children, sibling) {
            printk(KERN_INFO "Child PID %d | Comm %s\n", child->pid, child->comm);
    }
}

static int __init my_module_init(void) {
    printk(KERN_INFO "module says hi to kernel\n");
   
    // NOTE: kernel gives this current task's task_struct in a macro called `current`
    printk(KERN_INFO "Task: PID - %d  Comm - %s\n", current->pid, current->comm);

    has_init_as_parent();
    //print_inittask_children();
    //print_task_children(1);
    print_threads_of_process(1);
    return 0;
}

static void __exit my_module_exit(void) {
    printk(KERN_INFO "kernel says goodbye to module\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhishek Kumar");
MODULE_DESCRIPTION("A module calling the process tree traversal macros provided by linux kernel");
MODULE_VERSION("0.1");
