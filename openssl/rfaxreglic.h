#ifndef _CHECK_AX_H_
#define _CHECK_AX_H_
#ifdef  __cplusplus
extern "C" {
#endif

#ifdef __i386__
#define buildarch 1
#endif

#ifdef __x86_64__
#define buildarch 2
#endif

#ifdef __ia64__
#define buildarch 3
#endif

#ifdef __PPC__
#define buildarch 4
#endif

#ifdef __mips__
#define buildarch 5
#endif

#ifdef __aarch64__
#define buildarch 6
#endif

#ifdef __alpha__
#define buildarch 7
#endif


#define PRODUCTNAME 4
#define THISVERSION 8 
#define THISRELEASE 2

typedef struct __cs_version__ { 
        int major; 
        int minor; 
} *PCS_VERSION, CS_VERSION;

typedef struct __cs_ax_info__ { 
	char name[128];
	CS_VERSION ver; 
} *PCS_AX_INFO, CS_AX_INFO; 
	
extern struct product_info prod;
/*
 * 0 :     normal. operating on RedFlag AX. 
 * NOT 0 : abnormal. operating on NOT RedFlag AX, Other errors
 *         occured.
 */
int checkax(PCS_AX_INFO ax, struct product_info *prod);
	
#ifdef  __cplusplus
}
#endif

#endif
