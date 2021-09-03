#ifndef _CRYPT_H_
#define _CRYPT_H_
#ifdef _cplusplus
extern "C"{
#endif
int encrypt_buffer(const char* passwd,
		   unsigned char **out, int *outl,
		   unsigned char *in, int inl);
int decrypt_buffer(const char* passwd,
		   unsigned char **out, int *outl,
		   unsigned char *in, int inl);

int encrypt_file(const char* filename, const char* passwd,
                 unsigned char* buf, int size);
int decrypt_file(const char* filename, const char* passwd,
                 unsigned char** buf, int* size);
#ifdef _cplusplus
}
#endif

#endif /* _CRYPT_H_ */
