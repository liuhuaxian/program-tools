#include "add.h"
#include "sub.h"
#include <stdio.h>

int main()
{
#if USING_ARCHIVE_LIB
	printf("define USING_ARCHIVE_LIB\n");
#endif

#if HAVE_A_ADD_TEN
	printf("define HAVE_A_ADD_TEN\n");
#endif

#if HAVE_ADD_MODIFY
	printf("define HAVE_ADD_MODIFY\n");
#endif

#if LIB_VERSION
	printf("define LIB_VERSION = %d\n",LIB_VERSION);
#endif 
#if CONTAINER_SIZE
	printf("define CONTAINER_SIZE = %d\n", CONTAINER_SIZE);
#endif

#if LIB1_WITH_EXE
	printf("define LIB1_WITH_EXE\n");
#endif 

#if CONSUMER_CMP0041_NEW
	printf("define CONSUMER_CMP0041_NEW\n");
#endif 
	printf("5+5=%d\n",add(5,5));
	return 0;
}
