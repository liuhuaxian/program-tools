#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rfaxreglic.h"
#include "prod_config.h"
#include "lic_error.h"
#include "oplicense.h"

int checkax(PCS_AX_INFO ax, struct product_info *prod)
{
    CS_AX_INFO ax_tmp;
    PCS_AX_INFO p;

    if (prod == NULL)
	return LICENSE_ERR_PRODUCT;;

    if (ax == NULL) {
	ax_tmp.ver.major = THISVERSION;
	ax_tmp.ver.minor = THISRELEASE;
	p = &ax_tmp;
    }
    else {
	ax->ver.major = THISVERSION;
	ax->ver.minor = THISRELEASE;
	p = ax;
    }
    
    /* product name */
    if((int)prod->name != PRODUCTNAME)
	return LICENSE_ERR_PRODUCT;

    /* product version*/
    if (((int)prod->version != p->ver.major) ||
        ((int)prod->release != p->ver.minor))
	return LICENSE_ERR_PRODUCT;  

    /* mac address  */
    if (check_mac(prod) != 0)
        return LICENSE_ERR_PRODUCT;

    /* platform */
    if (check_platform(prod) != 0)
        return LICENSE_ERR_PRODUCT;

    return 0;
}

int main(int argc, char* argv[])
{
    /* ignore some troublesome signals */
    /* signal(SIGINT, SIG_IGN); */
    /* signal(SIGQUIT, SIG_IGN);*/
    /* signal(SIGTERM, SIG_IGN);*/
    /* signal(SIGSTOP, SIG_IGN);*/
    /* signal(SIGTSTP, SIG_IGN);*/
    /* signal(SIGTTIN, SIG_IGN);*/
    /* signal(SIGTTOU, SIG_IGN);*/

    int err;
    char serial[S_SERIAL+4+1];
    struct product_info info;

    if (argc==2) {
	strncpy(serial, argv[1], sizeof(serial));
    } else {
	printf("Register AX License #");
	if (!fgets(serial, 64, stdin))
	    return -1;
    }

    serial[S_SERIAL+4] = '\0';
    if(get_product_info(serial, &info)) {
	printf("Register AX License [\e[31m Failure\e[0m ]\n");
	return -1;
    }

    if ((err = checkax(NULL,&info)) != 0) {
	printf("Register AX License [\e[31m Failure\e[0m ]\n");
	return -1;
    }

    err = reglic( "/etc/.rfaxlicense", serial, &info);
    if((err != LICENSE_TEST_NOEXPIRED) && (err != LICENSE_FORMAL )) {
	printf("Register AX License [\e[31m Failure\e[0m ]\n");
	return -1;
    }
    printf("Register AX License [\e[32m Success\e[0m ]\n");

    /* printf("name: %d\n", info.name); */
    /* printf("version: %d\n", info.version); */
    /* printf("release: %d\n", info.release); */
    /* printf("platform: %d\n", info.platform); */
    /* printf("type: %d\n", info.type); */
    /* printf("relyear: %d\n", info.relyear); */
    /* printf("life: %d\n", info.life); */
    /* printf("region: %d\n", info.region); */
    /* printf("yearversion: %d\n", info.yearversion); */
    /* printf("istest: %d\n", info.istest); */

    return 0;
}
