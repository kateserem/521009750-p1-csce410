

/* (1UL << #) give each category a unique bit position. proc is 2^0= value 1, scheduler 2^1= value 2 , syscall 2^2= value 4  and so on*/
#define DBG_PROC     (1UL << 0)   /* process lifecycle              */ 
#define DBG_SCHED    (1UL << 1)   /* context switching              */ 
#define DBG_SYSCALL  (1UL << 2)   /* system call entry/exit         */ 
#define DBG_TRAP     (1UL << 3)   /* traps, interrupts, page faults */ 
#define DBG_VM       (1UL << 4)   /* virtual memory                 */ 
#define DBG_FS       (1UL << 5)   /* file system                    */ 
#define DBG_DEV      (1UL << 6)   /* drivers                        */ 

/*~0UL everything is on by turning on all category bits*/
#define DBG_ALL      (~0UL)

/*determining what type of message we want to see*/
#define DBG_ERR    1 
#define DBG_WARN   2 
#define DBG_INFO   3 
#define DBG_TRACE  4

/*let program read and change the mask and level at runtime*/
#define DBGCTL_GETMASK   0 
#define DBGCTL_SETMASK   1 
#define DBGCTL_GETLEVEL  2 
#define DBGCTL_SETLEVEL  3 

uint64 debugctl(int op, uint64 arg);   /* returns the previous value and sets 
* the new value; returns -1 on invalid op */ 

/*for holding the current category and level turned on*/
extern uint64 current_mask;
extern int current_level;

/*function needing to build to: convert category value to strings*/
char *debug_category_value_to_string(uint64 category);

/*getting the current process ID*/
int debug_current_pid(void);

/*only print when the category is turne*/
#ifdef DEBUG_LOGGING

/* (current_mask & category) is this category on? */ \
/* (level <= current_level) severity of how much we want to see*/ \
#define dprintf(category, level, format, ...) \
    do { \
        if ((current_mask & category) && (level <= current_level)) { \
            printk("[%s] [pid %d] [%s]: " format, \
                debug_category_value_to_string(category), \
                debug_current_pid(), \
                __func__, \
                ##__VA_ARGS__); \
        } \
    } while(0)

#else

#define dprintf(category, level, format, ...)

#endif
