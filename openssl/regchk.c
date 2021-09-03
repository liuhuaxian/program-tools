#include <stdlib.h>
#include <stdio.h>
#include "debug.h"
#include "regchk.h"
#include "oplicense.h"
#include "prod_config.h"
#include "lic_error.h"

/*
* check license from direct input serial 
*/
static int chklic( char* serial, struct product_info *info)
{
	int err;
	if(get_product_info(serial, info)) {
            return LICENSE_ERR_EVAL;
	}

	err = lic_is_state(info);
	return err;

}

/*
* register license into license file 
*/
int reglic( char *lic_file, char* serial, struct product_info *info)
{
	int err;

	err =  chklic( serial, info);
	if((err == LICENSE_FORMAL) || (err == LICENSE_TEST_NOEXPIRED)) {
		 dump_license(lic_file, serial);
	}

    return err;
}
/*
 * license checking when  HA daemon  start
 */
int  license_check(char* lic_file,struct product_info *info)
{
	int err;

        if(info == NULL)
                return -1;

        if (no_license_file(lic_file))
        {
                DBG("License File Not Exit\n");
                return -1;
        }
        /* normal boot checking */
        if (load_license(lic_file, info))
        {
                DBG("License Is Not Invalud.\n");
                return -1;
        }
	
	/* license file looks good, check if license expired. */
	err = lic_is_state(info);
	if (err == LICENSE_TEST_EXPIRED)
		return -1;
	else if (err == LICENSE_FORMAL)
		return 0;
	else if (err == LICENSE_TEST_NOEXPIRED)
         {
		 time_t t = test_mktime(info);
		 t += TEST_PERIOD*24*60*60;                                         

		 /* printf("****************************************************************************\n"); */
		 /* printf("Test License for the Product will be expired at %s\n", ctime(&t)); */

		 /* printf("ZhongKe RedFlag Inc. (http://www.redflag-linux.com)\n"); */
		 /* printf("****************************************************************************\n"); */
		 return 0;
        }
        return 0; /* all licenses are fine, passed */
}

#if 0
int main(int argc, char* argv[])
{
        //int ret;
        if (argc == 2)
                opt_debug = 1;
        else
                opt_debug = 0;

        struct product_info info;
        if(license_check(0,&info))
        {
                return -1;
                DBG("License_check Fail\n");
        }
        else
        {
                DBG("License_check OK\n");
                return 0;
        }
}
#endif
