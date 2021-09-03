#ifndef _CHECK_HA_H_
#define _CHECK_HA_H_
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

#define PRODUCTNAME 8
#define THISVERSION 2
#define THISRElEASE 1

typedef struct __cs_version__ { 
        int major; 
        int minor; 
} *PCS_VERSION, CS_VERSION;

typedef struct __cs_vm_info__ { 
	char name[128];
	CS_VERSION ver; 
} *PCS_VM_INFO, CS_VM_INFO; 
	
extern struct product_info prod;
/*
 * 0 :     normal. operating on RedFlag HA. 
 * NOT 0 : abnormal. operating on NOT RedFlag HA, Other errors
 *         occured.
 */
int checkvm(PCS_VM_INFO vm, struct product_info *prod);
	
#ifdef  __cplusplus
}
#endif

#endif
