#include "types.h"

#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h" /*added so we can access process info*/
#include "defs.h" /*added for access to myproc()*/

#include "debug.h"



uint64 current_mask = 0; /* default turn off all categories */
int current_level = DBG_INFO; /* default level */

char * debug_category_value_to_string(uint64 category) {
    /*convert category value to string*/
    switch (category) {
        case DBG_PROC: return "proc";
        case DBG_SCHED: return "sched";
        case DBG_SYSCALL: return "syscall";
        case DBG_TRAP: return "trap";
        case DBG_VM: return "vm";
        case DBG_FS: return "fs";
        case DBG_DEV: return "dev";
        default: return "UNKNOWN";
    }
}

int debug_current_pid(void) {
    struct proc *p = myproc(); /*calling existing function to find the current pid*/

    if (p == 0) {
        return -1; /*no current process */
    }

    return p->pid; /*return the process ID*/
}

/*operation: do this operation*/
/*argument: set to this value*/
uint64 debugctl(int operation, uint64 argument) {
    uint64 previous_mask = current_mask; /*store the previous mask value*/
    int previous_level = current_level; /*store the previous level value*/

    switch(operation) {
        case DBGCTL_GETMASK: 
            return current_mask;

        case DBGCTL_SETMASK: 
            current_mask = argument;
            return previous_mask;

        case DBGCTL_GETLEVEL: 
            return current_level;

        case DBGCTL_SETLEVEL:
            current_level = argument;
            return previous_level; 

        default:
            return -1; /*invalid operation*/
    }
}