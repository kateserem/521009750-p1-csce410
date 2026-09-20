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