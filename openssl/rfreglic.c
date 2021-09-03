#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h> 
#include "prod_config.h"
#include "lic_error.h"
#include "oplicense.h"


static void usage()
{
    printf("Useage: rfreglic -p file_path -s serial_number\n");
    exit(1);
}

int main (int argc, char ** argv)
{
    int err, opt;
    char * serial = NULL;
    char * license_path = NULL;
    struct product_info info;

    while ((opt = getopt(argc, argv, "p:s:")) != -1) {
        switch(opt) {
            case 'p':
                license_path = optarg;
                break;
            case 's':
                serial = optarg;
                break;
            default:
                usage();
        }
    }

    if (serial == NULL || license_path == NULL) {
        usage();
    }

    if(get_product_info(serial, &info)) {
        printf("Register License  [\e[31m Failure\e[0m ]\n");
        exit(1);
    }

    err = reglic(license_path, serial, &info); // generate the license file
    if((err != LICENSE_TEST_NOEXPIRED) && (err != LICENSE_FORMAL)) {
        printf("Register License  [\e[31m Failure\e[0m ]\n");
        exit(1);
    }
    printf("Register License [\e[32m Success\e[0m ]\n");
    exit(0);
}
