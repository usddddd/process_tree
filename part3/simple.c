#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/slab.h>

/*Illegal functions that need to be replaced
 *
 * list_add_tail()
   list_for_each_entry()
   list_del()
   list_for_each_entry_safe()
 * */

// offsetof implementation measures offset of member from address 0 thus giving offset of member
#define offset(type, member) ((size_t) &((type *)0)->member)


// my container of implementation specific for birthday struct
#define get_container(ptr) ({\
        const typeof(((struct birthday*)0)->list) *_mptr = (ptr);\
        (struct birthday*)((char*)_mptr - offset(struct birthday, list));})

// my list for each implementation specific for birthday struct
#define for_each(pos, head)\
    for(pos = get_container((head)->next);\
    &pos->list != (head);\
    pos = get_container(pos->list.next))

// my list for each safe implementation specific for birthday struct
#define for_each_safe(pos, n, head)\
    for(pos = get_container((head)->next),n = get_container(pos->list.next) ;\
    &pos->list != (head);\
    pos = n, n = get_container(n->list.next))

struct birthday {
    int day;
    int month;
    int year;
    struct list_head list;
};

// my list add tail implementation
void add_tail(struct list_head *new, struct list_head *head){
    struct list_head *hprev = head->prev;
    hprev->next = new;
    new->prev = hprev;
    new->next = head;
    head->prev = new;
}

// my list del implementation
static inline void list_delete(struct list_head *prev, struct list_head *next){
    next->prev = prev;
    prev->next = next;
}

// declare list head for handle
static LIST_HEAD(birthday_list);
// print birthday info to kernel log
void print_birthday_to_kernel(struct birthday *p){
    printk(KERN_INFO "day: %d month: %d year: %d",p->day,p->month,p->year);
}

/* This function is called when the module is loaded. */
int simple_init(void)
{
    // pointer to point to each element and to manipulate created birthdays
    struct birthday *person, *ptr; 
   
    // create 5 bday structs and add to list
    int i;
    for(i=0; i<5; i++){
        // make up a bday
        person = kmalloc(sizeof(*person), GFP_KERNEL);
        person->day = i+2; // day can't be zero
        person->month = i+4; // nor can month
        person->year = 1990+i; // everyone is a 90s baby
        // init list in element 
        INIT_LIST_HEAD(&person->list);

        // add element to list
       add_tail(&person->list, &birthday_list);
    }
    
    // traverse list and print output to kernel log
    for_each(ptr, &birthday_list){
        print_birthday_to_kernel(ptr);   
    }

    printk(KERN_INFO "Loading Module\n");

       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
    struct birthday *next, *ptr;
    
    // delete elems of list and return mem to kernel
    for_each_safe(ptr, next, &birthday_list){
        list_delete(ptr->list.prev, ptr->list.next);
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

