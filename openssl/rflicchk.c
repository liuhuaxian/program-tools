#include <stdio.h>
#include <stdlib.h>
#include "prod_config.h"
#include "oplicense.h"

int main(int argc, char* argv[])
{
    struct product_info info;

    if (argc != 2 || argv[1] == NULL)
        return -1;
	
    if(license_check(argv[1],&info) != 0) {
        printf("License_check [\e[31m Failure\e[0m ]\n");
        return -1;
    }
    else {
        /* passed */
        print_lic(&info);
        return 0;
    }

}

