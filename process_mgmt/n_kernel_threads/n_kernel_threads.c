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

#define NTHREADS 4
static struct task_struct *threads[NTHREADS];
struct my_thread_data {
    int id;
    int limit;
};
static struct my_thread_data tdata_arr[NTHREADS];


static int thread_function(void *arg) {

    struct my_thread_data *data = (struct my_thread_data *)arg;
    printk(KERN_INFO "kthread_%d: coming to life\n",data->id);
    int value = 0;

    printk(KERN_INFO "kthread_%d: starting the cpu work\n", data->id);
    while(!kthread_should_stop()) {
        //printk(KERN_INFO "kthread running: count = %d\n", count++);
        value++;
        msleep(1);
    }

    printk(KERN_INFO "kthread_%d: completed the cpu work: result: %d\n", data->id, value);
    return 0;

}


static int __init kthread_module_init(void) {
    printk(KERN_INFO "Module loaded: entered __init call\n");
   
    // NOTE: kernel gives this current task's task_struct in a macro called `current`
    // printk(KERN_INFO "Task: PID - %d  Comm - %s\n", current->pid, current->comm);
    
    printk(KERN_INFO "starting %d threads\n", NTHREADS);
    
    for (int i = 0; i < NTHREADS; i++) {
        tdata_arr[i].id = i;
        tdata_arr[i].limit = (i + 4) * 1900;

        threads[i] = kthread_run(thread_function, &tdata_arr[i], "my_kthread_%d", i);
        if (IS_ERR(threads[i])) {
            printk(KERN_ERR "failed to create thread no %d\n", i);
            threads[i] = NULL;
        }
    }

    return 0;
}

static void __exit kthread_module_exit(void) {
    printk(KERN_INFO "Module entered __exit call\n");

    for (int i = 0; i < NTHREADS; i++) {
        if (threads[i]) {
            // next if stmt checks if any thread exited voluntarily then no need to call kthread_stop on it
            int ret = kthread_stop(threads[i]);
            if (ret == 0)
                printk(KERN_INFO "thread_%d exited cleanly (maybe voluntarily)\n", i);
            else
                printk(KERN_INFO "thread_%d returned %d\n", i, ret);
            threads[i] = NULL;
        }
    }


    printk(KERN_INFO "Module unloaded\n");
}

module_init(kthread_module_init);
module_exit(kthread_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhishek Kumar");
MODULE_DESCRIPTION("A simple example creating n kthreads, taking input, doing cpu work, running and stopping");
MODULE_VERSION("0.1");
