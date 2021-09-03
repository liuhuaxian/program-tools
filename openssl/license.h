#ifndef _LICENSE_H 
#define _LICENSE_H

#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/times.h>
#include <errno.h>
#include <assert.h>

struct license_info { // the struct to input
    const char * license_path;//license file path
    int product_name;
    int version;
    int release;
    unsigned int period_days;
};

struct license_state { // the struct to ouput
    int state;
    unsigned int lefttime;
    const char * err;
};

// enum the states of license
enum STATE{
    STATE_LICENSE_FORMAL       = 0,
    STATE_LICENSE_TRIAL        = 1,
    STATE_LICENSE_EXPIRED      = 2,
    STATE_LICENSE_INVALID      = 3,
    STATE_LICENSE_NONE         = 4,
    STATE_LICENSE_ERR_PRODUCT  = 5,
    STATE_LICENSE_UNKNOWN      = 6,
    STATE_LICENSE_NO_MEMORY    = 7,
};

void get_license_state(struct license_info *license_in, struct license_state *license_out);
int gen_license(const char* serial, const char* license_path);
#endif
