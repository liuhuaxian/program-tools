#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>

#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/utsname.h>

#include "crypt.h"
#include "oplicense.h"
#include "debug.h"
#include "prod_config.h"
#include "lic_error.h"

/* FIXME: need to hide this passwd deeper, because hackers know "strings" */
const char* passwd = "Redflag Linux"; /* password to encrypt license file */

int no_license_file(char* lic_file)
{
    struct stat statbuf;
    if(lic_file == NULL)
        return -1;

    if (stat(lic_file, &statbuf) && errno == ENOENT)
        return -1;
    return 0;
}

int lic_is_test(struct product_info *info)
{
    if (info->istest )
        return -1;
    else
        return 0;
}

time_t test_mktime(struct product_info *info)
{
    struct tm test_tm;
    time_t t;

    memset(&test_tm, 0, sizeof(test_tm));	
    test_tm.tm_year = info->testyear + 2009 - 1900;
    test_tm.tm_mon = info->testmonth - 1;
    test_tm.tm_mday = info->testday;

    t = mktime(&test_tm);
    return t;
}

int lic_is_expired(struct product_info *info)
{
    time_t now;
    time_t t;
	
    t = test_mktime(info);
    time(&now);
    //todo
    if (now > t + TEST_PERIOD*24*60*60)
        return -1; /* license expired */
    else 
        return 0;
}

int lic_is_state(struct product_info *info)
{
    if (lic_is_test(info)) {
        if (lic_is_expired(info)) {
            return LICENSE_TEST_EXPIRED;
        }
        else {
            return LICENSE_TEST_NOEXPIRED;
        }
    }
    return LICENSE_FORMAL;	
}

void print_lic(struct product_info *info)
{
    time_t t;
    int i;

    if(info->istest) {
        printf("Product  Edition:[Test edition]\n");
        t = test_mktime(info);
        if(t == -1) return;

        printf("Product     Date:%s",ctime(&t));
    }
    else {
        printf("Product     type:[Formal edition]\n");
    }
    printf("Product     Name:%d\n",info->name);
    printf("Product  Version:%d\n",info->version);
    printf("Product  Release:%d\n",info->release);
    printf("Product Platform:%d\n",info->platform);
    printf("Product     Type:%d\n",info->type);
    printf("Product      Mac");	
    for (i = 0; i < 6; i++)
        printf(":%02x",info->mac[i]);
    printf("\n");
}


/*
 * dump licenses into license file.
 */
int dump_license(char* lic_file, const  char *buf)
{
    int size;

    if (!lic_file)
        return -1;

    size = strlen(buf);
    if (encrypt_file(lic_file, passwd, (unsigned char *)buf, size)) {
        DBG("dump_license(): encrypt file failed.\n");
        return -1;
    }

    return 0;
}

int load_license(char* lic_file, struct product_info *info)
{
    unsigned char* buf;
    int size;
	
    if (!lic_file)
        return -1;

    if (decrypt_file(lic_file, passwd,&buf, &size)) {
        DBG("load_license(): decrypt license file failed.\n");
        return LICENSE_ERR_NOFILE;
    }

    if(get_product_info((char *)buf, info)) {
        DBG("load_license(): invalid serial#: %s.\n", buf);
        free(buf);
        return LICENSE_ERR_EVAL;
    }
	
    free(buf);

    return LICENSE_ERR_NOERR;	
}

int check_mac(struct product_info *info)
{
    int fd;
    int if_len;
    struct ifreq buf[MAXINTERFACES];
    struct ifconf ifc;
    char licmac[18], thismac[18];

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        return -1;
    }

    sprintf(licmac,"%02x:%02x:%02x:%02x:%02x:%02x",
            (unsigned char) info->mac[0],
            (unsigned char) info->mac[1],
            (unsigned char) info->mac[2],
            (unsigned char) info->mac[3],
            (unsigned char) info->mac[4],
            (unsigned char) info->mac[5]);
    licmac[17]='\0';

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t) buf;
    if (ioctl(fd, SIOCGIFCONF, (char *) &ifc) == -1) {
        return -1;
    }

    if_len = ifc.ifc_len / sizeof(struct ifreq);
    while (if_len-- > 0) {
        if (!(ioctl(fd, SIOCGIFHWADDR, (char *) &buf[if_len]))) {
            sprintf(thismac,"%02x:%02x:%02x:%02x:%02x:%02x",
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[0],
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[1],
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[2],
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[3],
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[4],
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[5]);
            thismac[17]='\0';
            if(strncmp(thismac,licmac,18) == 0) {
                // printf("thismac:%s\nlicmac:%s\n",thismac,licmac);
		close(fd);
	   	return 0;
            }
		
        }
        else {
            perror("ioctl mac address error\n");
        }
    } //–while end
    close(fd);
    return -1;
}

int check_platform(struct product_info *info)
{
    struct utsname buf;
    uname(&buf);
    int platform = -1;
    if (!strcmp(buf.machine, "i386") ||
        !strcmp(buf.machine, "i486") ||
        !strcmp(buf.machine, "i586") ||
        !strcmp(buf.machine, "i686"))
        platform = 0;
    else if (!strcmp(buf.machine, "x86_64"))
        platform = 1;

    else if (!strcmp(buf.machine, "IA64"))
        platform = 2;

    else if (!strcmp(buf.machine, "ppc64"))
        platform = 3;

    else if (!strcmp(buf.machine, "mips64"))
        platform = 4;

    else if (!strcmp(buf.machine, "Zhonzhi"))
        platform = 5;

    else if (!strcmp(buf.machine, "aarch64"))
        platform = 6;

    else if (!strcmp(buf.machine, "sw_64"))
        platform = 7;


    else
	platform = 31;
    
    /* wrong platform */
    if (platform != info->platform)
        return -1;

    return 0;
}
