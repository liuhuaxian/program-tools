/* See COPYRIGHT for copyright information. */

#ifndef ERROR_H
#define ERROR_H

#define   LICENSE_ERR_NOERR     0      	//Creat a formal license
#define   LICENSE_ERR_EVAL      1       //Invalid serial
#define   LICENSE_ERR_PRODUCT   2       //It is not for this product
#define   LICENSE_ERR_ARCH      3       //It is not for this platform
#define   LICENSE_ERR_NULL      4      //Creat a test license
#define   LICENSE_ERR_NOFILE    5      //Check there is no license file (first install)

#define   LICENSE_TEST_EXPIRED  6       //Check it is a test license but already expired
#define   LICENSE_FORMAL        7       //Check it is a formal license

#define   LICENSE_FORMAL_8        8       //Check it is not for this platform
#define   LICENSE_FORMAL_9        9       //Check it is not for this product

#define   LICENSE_TEST_NOEXPIRED 10     //Check it is a test license (not expired)
#define   LICENSE_ERR_PARAM 	12      //Input sign error (not 0 ro 1)

#endif	// !ERROR_H */
