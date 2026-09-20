#include "kernel/types.h" /*defines xv6 data types (need uint64)  */

#include "user/user.h"
#include "kernel/debug.h"

/*helper function to convert category name to its corresponding bit*/
uint64 convert_category_name_to_bit(char * category_name) {
    if (strcmp(category_name, "proc") == 0) {
        return DBG_PROC;
    }
    if (strcmp(category_name, "sched") == 0) {
        return DBG_SCHED;
    }
    if (strcmp(category_name, "syscall") == 0) {
        return DBG_SYSCALL;
    }
    if (strcmp(category_name, "trap") == 0) {
        return DBG_TRAP;
    }
    if (strcmp(category_name, "vm") == 0) {
        return DBG_VM;
    }
    if (strcmp(category_name, "fs") == 0) {
        return DBG_FS;
    }
    if (strcmp(category_name, "dev") == 0) {
        return DBG_DEV;
    }
    if (strcmp(category_name, "all") == 0) {
        return DBG_ALL;
    }
    return 0;
}

/*helper function to convert level name to its corresponding value*/
int convert_level_name_to_value(char *level_name) {
    if (strcmp(level_name, "err") == 0) {
        return DBG_ERR;
    }
    if (strcmp(level_name, "warn") == 0) {
        return DBG_WARN;
    }
    if (strcmp(level_name, "info") == 0) {
        return DBG_INFO;
    }
    if (strcmp(level_name, "trace") == 0) {
        return DBG_TRACE;
    }

    return 0;
}

int main (int argc, char *argv[]) {
    uint64 current_mask;
    uint64 current_level;

    /*entered "dbg" only, get and print the current debug mask (what is turned on) and level*/
    if (argc == 1) {
        current_mask = debugctl(DBGCTL_GETMASK, 0); /*zero ignores*/
        current_level = debugctl(DBGCTL_GETLEVEL, 0);

        printf("mask: %lu\n", current_mask);
        printf("level: %lu\n", current_level);

        exit(0);
    }

    /*entered "dbg on", turn on the specified categories*/
    if (strcmp(argv[1], "on") == 0) {
        uint64 categories_to_turn_on = 0;

        /*check if user provided at least one category*/
        if (argc < 3) {
            printf("dbg requires at least one category to turn on\n");
            exit(1);
        }

        /*iterate through each category listed by user and convert to its corresponding bit*/
        for (int i = 2; i < argc; i++) {
            uint64 category_bit = convert_category_name_to_bit(argv[i]);

            if (category_bit == 0) {
                printf("Invalid category: %s\n", argv[i]);
                exit(1);
            }

            /*add the category bit to the current mask to turn it on (using bitwise OR to add the bits together)*/
            categories_to_turn_on |= category_bit;
        }

        /*get the current mask (categories turned on)*/
        current_mask = debugctl(DBGCTL_GETMASK, 0); 

        /*bitwise OR to add the bits together*/
        current_mask |= categories_to_turn_on;

        /*update current mask with the newly added turned on categories*/
        debugctl(DBGCTL_SETMASK, current_mask);
        
        exit(0);
    }


    if (strcmp(argv[1], "off") == 0) {
        uint64 categories_to_turn_off = 0;

        /*check if user provided at least one category*/
        if (argc < 3) {
            printf("dbg requires at least one category to turn off\n");
            exit(1);
        }

        /*iterate through each category listed by user and convert to its corresponding bit*/
        for (int i = 2; i < argc; i++) {
            uint64 category_bit = convert_category_name_to_bit(argv[i]);

            if (category_bit == 0) {
                printf("Invalid category: %s\n", argv[i]);
                exit(1);
            }

            /*add the category bit to the current mask to turn it off (using bitwise OR to add the bits together)*/
            categories_to_turn_off |= category_bit;
        }

        /*get the current mask (categories turned on)*/
        current_mask = debugctl(DBGCTL_GETMASK, 0); 

        /*bitwise AND with NOT to remove the bits from the current mask*/
        current_mask &= ~categories_to_turn_off;

        /*update current mask with the newly removed turned off categories*/
        debugctl(DBGCTL_SETMASK, current_mask);

        exit(0);
    }

    if (strcmp(argv[1], "only") == 0) {
        uint64 categories_to_turn_on = 0;

        /*check if user provided at least one category*/
        if (argc < 3) {
            printf("dbg requires at least one category to turn on\n");
            exit(1);
        }

        /*iterate through each category listed by user and convert to its corresponding bit*/
        for (int i = 2; i < argc; i++) {
            uint64 category_bit = convert_category_name_to_bit(argv[i]);

            if (category_bit == 0) {
                printf("Invalid category: %s\n", argv[i]);
                exit(1);
            }

            /*add the category bit to the current mask to turn it on (using bitwise OR to add the bits together)*/
            categories_to_turn_on |= category_bit;
        }

        /*replace current mask with the newly added turned on categories*/
        debugctl(DBGCTL_SETMASK, categories_to_turn_on);

        exit(0);
    }

    if (strcmp(argv[1], "level") == 0) {
        /*check if user provided a level*/
        if (argc != 3) {
            printf("dbg requires exactly one level to set\n");
            exit(1);
        }

        int level_value = convert_level_name_to_value(argv[2]);

        if (level_value == 0) {
            printf("Invalid level: %s\n", argv[2]);
            exit(1);
        }

        /*set the debug level to the specified value*/
        debugctl(DBGCTL_SETLEVEL, level_value);

        exit(0);
    }

    printf("Invalid command. Usage: dbg [on|off|only|level] [categories/level]\n");
    exit(1);
}