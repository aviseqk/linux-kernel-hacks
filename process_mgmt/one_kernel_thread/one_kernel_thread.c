/* Module: kernel_threads
 *
 * The purpose of this program is to use kthread macros namely `kthread_create`, `wakeup_process` and `kthread_run` to implement the creation of kernel threads
 * The msleep call is used to mimic the thread doing some task on some CPU (_can be replaced with fibonacci calculation work_)
 * The running threads are either killed by the thread themselves calling do_exit() or by parent thread pulling the plug on them by calling kthread_stop().
 *
 * */


#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static struct task_struct *example_kthread;

static int thread_function(void *data) {
    
    printk(KERN_INFO "kthread: coming to life\n");
    int count = 0;
    printk(KERN_INFO "kthread: starting the cpu work\n");
    while(!kthread_should_stop()) {
        printk(KERN_INFO "kthread running: count = %d\n", count++);
        msleep(1);
    }

    printk(KERN_INFO "thread: completed the cpu work\n");
    return 0;

}

/* 
static void create_n_threads(int n, struct task_struct *thread_arr[]) {
    int i = 1;
    struct task_struct *task;


    for (i; i <= n; i++){
        printk(KERN_INFO "creating the thread number %d\n", i);
        task = kthread_create(thread_function, i, "thread_%d", i);

        &thread_arr[i] = task;
    }
}
*/

static int __init my_module_init(void) {
    printk(KERN_INFO "Module loaded: entered __init call\n");
   
    // NOTE: kernel gives this current task's task_struct in a macro called `current`
    // printk(KERN_INFO "Task: PID - %d  Comm - %s\n", current->pid, current->comm);
    
  //  struct task_struct thread_arr[4];
    example_kthread = kthread_run(thread_function, NULL, "my_example_kthread");
    if (IS_ERR(example_kthread)) {
        printk(KERN_ERR "Failed to create the kthread\n");
        return PTR_ERR(example_kthread);
    }
    return 0;
}

static void __exit my_module_exit(void) {
    printk(KERN_INFO "Module entered __exit call\n");

    if (example_kthread) {
        kthread_stop(example_kthread);
        printk(KERN_INFO "kthread: stopped\n");
    }
    printk(KERN_INFO "Module unloaded\n");
}

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

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhishek Kumar");
MODULE_DESCRIPTION("A simple example creating n kthreads, running and stopping");
MODULE_VERSION("0.1");
