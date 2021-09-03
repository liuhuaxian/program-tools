#ifndef __CONFIG_H__
#define __CONFIG_H__

struct product_info
{
    unsigned int name:6; 
    unsigned int version:6;
    unsigned int release:5;
    unsigned int platform:5;
    unsigned int type:5;
    unsigned int relyear:4;
    unsigned int life:4;
    unsigned int region:6;
    unsigned int yearversion:1;
    /* total 42 bits above */
    /* unsigned int reserved:14; replaced by test sn usage */
    unsigned int istest:1;
    unsigned int testyear:4;
    unsigned int testmonth:4;
    unsigned int testday:5;
    unsigned int number:24;
    unsigned char mac[6];
    //unsigned char padding[5];
} __attribute__ ((packed));/* ten bytes total */

#define S_SERIAL  35
#define PRODUCT_INI "/etc/product.ini"

#ifdef __cplusplus
extern "C"{
#endif
    int reglic( char *lic_file, char* serial, struct product_info *info);
    //int install_license(char *lic_file, char* serial, int sign);
    int license_check(char* lic_file,struct product_info *info);
    int get_product_info(const char* sn, struct product_info* info);
    int generate(const struct product_info *info, char* ret);
#ifdef __cplusplus
}
#endif

#endif
