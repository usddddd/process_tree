#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>


struct birthday {
    int day;
    int month;
    int year;
    struct list_head list;
}

// print birthday info to kernel log
void print_birthday_to_kernel(struct birthday *p){
    printk(KERN_INFO "day: %d month: %d year: %d",p->day,p->month,p->year);
}

/* This function is called when the module is loaded. */
int simple_init(void)
{
    static LIST_HEAD(birthday_list);
    struct birthday *person; 
   
    // create 5 bday structs and add to list
    for(int i=0; i<5; i++){
        person = kmalloc(sizeof(*person), GFP_KERNEL);
        person->day = i+2; // day can't be zero
        person->month = i+4; // nor can month
        person->year = 1990+i // everyone is a 90s baby
        INIT_LIST_HEAD(&person->list);
        list_add_tail(&person->list, &birthday_list);
    }

    // pointer to point to each element
    struct birthday *ptr;
    
    // traverse list and print output to kernel log
    list_for_each_entry(ptr, &birthday_list, list){
        print_birthday_to_kernel(ptr);   
    }

    printk(KERN_INFO "Loading Module\n");

       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
    struct birthday *next, *ptr;
    
    // delete elems of list and return mem to kernel
    list_for_each_entry_safe(ptr, next, &birthday_list, list){
        list_del(&ptr->list);
        kfree(ptr);
    }
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

