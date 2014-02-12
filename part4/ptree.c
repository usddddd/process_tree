#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>

    
    static void print_task_info(struct task_struct *t, int level){
        struct task_struct *task;
        struct list_head *list;
        //char *buffer = kmalloc((sizeof(char)*500), GFP_KERNEL);

        // construct offset for tree display
        //int i;
        //for(i = 0; i < level && i < 20; i++){
          //  tab_offset[i] = '\t';
        //}
        
        // set null terminator
        //tab_offset[i] = '\0';

        // write task info to buffer
        //if(level > 0){
        //snprintf(buffer, sizeof(buffer), "\\_Name: %s  State: %ld  PID: %ld \n",t->comm,t->state,(long)t->pid);
        //}
        //else{
        //snprintf(buffer, sizeof(buffer), "--Name: %s  State: %ld  PID: %ld \n",t->comm,t->state,(long)t->pid);
        //}
        
        // char array to hold offset of child processes [limited to 19 levels with 1 for null terminator]
        char offset[20] = "         ";
        offset[level+1] = '\0';

        // print tree to kernel log
        if(level > 0){
        printk(KERN_INFO "%s[%ld]\\__ Name: %s  State: %ld  PID: %ld \n",offset,(long)t->real_parent->pid,t->comm,t->state,(long)t->pid);
        }
        else{
        printk(KERN_INFO "%s -- Name: %s  State: %ld  PID: %ld \n",offset,t->comm,t->state,(long)t->pid);
        }
        // iterate through children of init process
        // init_task is list_head not a &list_head
        // book is wrong! use '.' not '->'
        list_for_each(list, &t->children){
            task = list_entry(list, struct task_struct, sibling);
            print_task_info(task, level+2);
        }
    }


/* This function is called when the module is loaded. */
int simple_init(void)
{


    // print init info as first entry in process tree
    printk(KERN_INFO "-- ");
    print_task_info(&init_task, 0);

    printk(KERN_INFO "Done Loading Module\n");

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

