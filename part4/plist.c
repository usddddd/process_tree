#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>



/* This function is called when the module is loaded. */
int simple_init(void)
{
    struct task_struct *task;
    //iterate through processes and output name, state, and process id to kernel log
    for_each_process(task){
        printk("Name: %s  State: %ld  PID: %ld \n", task->comm,task->state,(long)task->pid);
    }
       printk(KERN_INFO "Loading Module\n");

       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

