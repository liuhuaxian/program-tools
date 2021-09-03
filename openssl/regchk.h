#ifndef _INSTALL_H_
#define _INSTALL_H_

/*---------------------------  install_license  ----------------------------
			Used when install our tool CD

IN:
	char * serial		The serial number receive form input
	int    sign		If it is 0, check the license
				If it is 1, install license with input
OUT:
	0			Creat a formal license  
	1			Invalid serial
	2			It is not for this product
	3			It is not for this platform
	4			Creat a test license

	5			Check there is no license file (first install)
	6			Check it is a test license but already expired
	7			Check it is a formal license
	8			Check it is not for this platform
	9			Check it is not for this product
	10			Check it is a test license (not expired)
	11			Check there is a invalid license file

	12			Input sign error (not 0 ro 1)

Keeper: xiaoliu
Date:   2007.10.10
--------------------------------------------------------------------------*/
#include "prod_config.h"
//int install_license(char *lic_file, char* serial, int sign);
int reglic( char *lic_file, char* serial, struct product_info *info);
int license_check(char* lic_file,struct product_info *info);
#endif /* _INSTALL_H_ */
