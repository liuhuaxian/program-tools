#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rfvmreglic.h"
#include "prod_config.h"
#include "lic_error.h"
#include "oplicense.h"

int checkvm(PCS_VM_INFO vm, struct product_info *prod)
{
    CS_VM_INFO vm_tmp;
    PCS_VM_INFO p;

    if (prod == NULL)
        return LICENSE_ERR_PRODUCT;;

    if (vm == NULL) {
        vm_tmp.ver.major = THISVERSION;
        vm_tmp.ver.minor = THISRElEASE;
        p = &vm_tmp;
    }
    else {
        vm->ver.major = THISVERSION;
        vm->ver.minor = THISRElEASE;
        p = vm;
    }

    /* product name */
    if((int)prod->name != PRODUCTNAME)
        return LICENSE_ERR_PRODUCT;

    /* product version*/
    if (((int)prod->version != p->ver.major) ||
        ((int)prod->release != p->ver.minor))
        return LICENSE_ERR_PRODUCT;    
                                  
    /* mac address */
    if (check_mac(prod) != 0)
        return LICENSE_ERR_PRODUCT;

    return 0;
}


int main()                                                              //test for this function
{
    int err;
    char serial[S_SERIAL+4+1];
    struct product_info info;

    printf("Register VM License #");
    if (!fgets(serial, 64, stdin))
        return -1;

    serial[S_SERIAL+4] = '\0';
    if(get_product_info(serial, &info)) {
        printf("Register VM License [\e[31m Failure\e[0m ]\n");
        return -1;
    }
	
    if ((err = checkvm(NULL,&info)) != 0) {
        printf("Register VM License [\e[31m Failure\e[0m ]\n");
        return -1;
    }
	
    err = reglic( "/etc/.rfvmlicense", serial, &info);
    if((err != LICENSE_TEST_NOEXPIRED) && (err != LICENSE_FORMAL )) {
        printf("Register VM License [\e[31m Failure\e[0m ]\n");
        return -1;
    }
    //print_lic(&info);
    printf("Register VM License [\e[32m Success\e[0m ]\n");
    return 0;
}


