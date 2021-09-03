#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rfhareglic.h"
#include "prod_config.h"
#include "lic_error.h"
#include "oplicense.h"

int checkha(PCS_HA_INFO ha, struct product_info *prod)
{
    CS_HA_INFO ha_tmp;
    PCS_HA_INFO p;

    if (prod == NULL)
        return LICENSE_ERR_PRODUCT;;

    if (ha == NULL) {
        ha_tmp.ver.major = THISVERSION;
        ha_tmp.ver.minor = THISRElEASE;
        p = &ha_tmp;
    }
    else {
        ha->ver.major = THISVERSION;
        ha->ver.minor = THISRElEASE;
        p = ha;
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

    printf("Register HA License #");
    if (!fgets(serial, 64, stdin))
        return -1;

    serial[S_SERIAL+4] = '\0';
    if(get_product_info(serial, &info)) {
        printf("Register HA License [\e[31m Failure\e[0m ]\n");
        return -1;
    }
	
    if ((err = checkha(NULL,&info)) != 0) {
        printf("Register HA License [\e[31m Failure\e[0m ]\n");
        return -1;
    }
	
    err = reglic( "/etc/.rfhalicense", serial, &info);
    if((err != LICENSE_TEST_NOEXPIRED) && (err != LICENSE_FORMAL )) {
        printf("Register HA License [\e[31m Failure\e[0m ]\n");
        return -1;
    }
    //print_lic(&info);
    printf("Register HA License [\e[32m Success\e[0m ]\n");
    return 0;
}


