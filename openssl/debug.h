#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <stdlib.h>
//static int opt_debug = 0;
//#define DEBUG 1
#if DEBUG
# define DBG(x...) do {                         \
        if (opt_debug)                          \
            printf("##### " __FILE__ ": " x);   \
    } while(0)
#else
# define DBG(x...) do { } while(0)
#endif

# define FATAL(x...) do {                       \
        printf(x);                              \
        exit(-1);                               \
    } while(0)
	
#endif /* _DEBUG_H_ */
