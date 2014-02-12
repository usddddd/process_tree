#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

    // stack handle
    static LIST_HEAD(process_stack);
    
    static void print_task_info(struct task_struct *t){
        printk(KERN_INFO "Name: %s  State: %ld  PID: %ld \n", t->comm,t->state,(long)t->pid);
    }

    // push onto stack represented by process_stack
    void push_to_stack(struct list_head *new){
        list_add(new, &process_stack);
    }

    // pop from process_stack
    struct task_struct *pop_stack(void){
        struct task_struct *task;
        // get first elem from stack
        struct list_head *l = process_stack.next;
        task = list_entry(l, struct task_struct, sibling);
        // remove entry from stack
        list_del(l);
        //return task_struct popped from stack
        return task;
    }

/* This function is called when the module is loaded. */
int simple_init(void)
{
    struct task_struct *task;
    struct list_head *list;

    // char array to hold tab offset of child processes [limited to 20 levels with 1 for null terminator]
    char tab_offset[21] = "\t";
    // char array to hold symbol for children
    char child[] = "\\_";

    // print init info as first entry in process tree
    printk(KERN_INFO "-- ");
    print_task_info(&init_task);

    // iterate through children of init process
    // init_task is list_head not a &list_head
    // book is wrong! use '.' not '->'
    list_for_each(list, &init_task.children){
        // push children of init onto stack for DFS
        push_to_stack(list);
    }

    // DFS through children of init and their children
    while(!list_empty(&process_stack)){
        task = pop_stack();
        print_task_info(task);
        // add children of task to stack
        list_for_each(list, &task->children){
            push_to_stack(list);
        }
     }

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

