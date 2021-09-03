#ifndef _OPLICENSE_H_
#define _OPLICENSE_H_

#define TEST_PERIOD 90         /* 90 days of testing/tryout period */
#include <time.h>
#include "prod_config.h"
#define MAXINTERFACES 256

#ifdef __cplusplus
extern "C" {
#endif
int no_license_file(char* lic_file);
int lic_is_test(struct product_info *info);
int load_license(char* lic_file, struct product_info *info);
int dump_license(char* lic_file,  const char *buf);
int lic_is_expired(struct product_info *info);
time_t test_mktime(struct product_info *info);
void print_lic(struct product_info *info);
int lic_is_state(struct product_info *info);
int check_mac(struct product_info *info);
int check_platform(struct product_info *info);
#ifdef __cplusplus
}
#endif

#endif
