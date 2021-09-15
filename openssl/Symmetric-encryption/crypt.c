#include <stdio.h>
#include <openssl/evp.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
extern int errno;

#include "crypt.h"

/*
 * This function takes 'inl' bytes from 'in' and outputs bytes
 * encrypted/decrypted by DES into 'out'.
 * 
 * operation: 1: encrypt, 0: decrypt
 *
 * it's up to caller to free 'out'.
 */
static int ende_crypt_buffer(const char* passwd, int operation,
			     unsigned char **out, int *outl,
			     unsigned char *in, int inl)
{
    EVP_CIPHER_CTX *ectx = EVP_CIPHER_CTX_new();

    unsigned char iv[EVP_MAX_IV_LENGTH], key[EVP_MAX_KEY_LENGTH];
    unsigned char* dbuf;
    int dsize;
    const char* salt = NULL;
	
    EVP_BytesToKey(EVP_des_ecb(), EVP_md5(),
                   salt, passwd, strlen(passwd),
                   1, key, iv);

    int n = EVP_CipherInit_ex(ectx, EVP_des_ecb() ,NULL,key, iv, operation);
    
    *outl = 0;
    /* inl+8 should be enough, but i am paranoid */
    //*out = (unsigned char*)malloc(inl+16);
    //dbuf = (unsigned char*)malloc(inl+16);
    *out = (unsigned char*)calloc(sizeof(char),inl+16);
    dbuf = (unsigned char*)calloc(sizeof(char),inl+16);
    	
    if (*out == NULL || dbuf == NULL) {
        fprintf(stderr, "out/dbuf is NULL!\n");
        if (*out)
            free(*out);
        if (dbuf)
            free(dbuf);
        return -1;
    }
    memset(*out, 0, inl + 16);
    memset(dbuf, 0, inl + 16);

    /* encrypt */
    EVP_CipherUpdate(ectx, dbuf, &dsize, in, inl); //开始对in 加解密，结果保存到dbuf中
    memcpy(*out, dbuf, dsize);
    *outl = dsize;

    /* handle leftover */
    EVP_CipherFinal_ex(ectx, dbuf, &dsize);
    if (*outl+dsize > inl+16) {
        fprintf(stderr, "encrypt_buffer() bug: buffer too small. %d>%d\n", *outl+dsize, inl+7);
        return -1;
    }

    memcpy(*out+*outl, dbuf, dsize);
    *outl += dsize;

    EVP_CIPHER_CTX_free(ectx);

    free(dbuf);
    return 0;
}

int encrypt_buffer(const char* passwd,
		   unsigned char **out, int *outl,
		   unsigned char *in, int inl)
{
    return ende_crypt_buffer(passwd, 1, out, outl, in, inl);
}

static int get_file_size(const char* filename)
{
    struct stat statbuf;
    int err;
    err = stat(filename, &statbuf);
    if (err)
        return err;
    return statbuf.st_size;
}


int decrypt_buffer(const char* passwd,
		   unsigned char **out, int *outl,
		   unsigned char *in, int inl)
{
    return ende_crypt_buffer(passwd, 0, out, outl, in, inl);
}

/* encrypt buffer, then write to file */
int encrypt_file(const char* filename, const char* passwd,
                 unsigned char* buf, int size)
{
    int err;
    unsigned char* out;
    int outl;
    int fd;

    err = encrypt_buffer(passwd, &out, &outl, buf, size);
    if (err) {
        fprintf(stderr, "encrypt failed.\n");
        return -1;
    }

    fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC, 00644);
    if (fd < 0) {
        //		fprintf(stderr, "open %s failed: %s\n", filename, strerror(errno));
        free(out);
        return -1;
    }

    err = write(fd, out, outl);
    if (err != outl) {
        perror("write");
        close(fd);
        free(out);
        return -1;
    }
    close(fd);
    free(out);
    return 0;
}

/* dencrypt file, then write to buffer */
int decrypt_file(const char* filename, const char* passwd,
                 unsigned char** buf, int* size)
{
    int err;
    int inl;
    unsigned char* in;
    int fd;

    /* load fromfile */
    inl = get_file_size(filename);
    if (inl < 0) {
        //		fprintf(stderr, "stat file %s failed: %s\n", filename, strerror(errno));
        return -1;
    }
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        //		fprintf(stderr, "open %s failed: %s\n", filename, strerror(errno));
        return -1;
    }
    in = (unsigned char*)malloc(inl);
    if (read(fd, in, inl) != inl) {
        //		fprintf(stderr, "read %s failed: %s\n", filename, strerror(errno));
        free(in);
        close(fd);
        return -1;
    }
    close(fd);
	
    err = decrypt_buffer(passwd, buf, size, in, inl);
    if (err) {
        fprintf(stderr, "encrypt failed.\n");
        return -1;
    }
    free(in);
    return 0;
}

#if 1 /* testing */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int err;
    char* fromfile, *tofile, *pass;
    unsigned char* in, *out;
    int inl, outl;
    int fd;

    if (argc != 5 || argv[1][0] != '-') {
        fprintf(stderr, "Usage %s -[d|e] fromfile tofile password\n", argv[0]);
        return -1;
    }
    fromfile = argv[2];
    tofile = argv[3];
    pass = argv[4];

    /* load fromfile */
    inl = get_file_size(fromfile);
    if (inl < 0) {
        fprintf(stderr, "stat file %s failed: %s\n", fromfile, strerror(errno));
        return -1;
    }
    fd = open(fromfile, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "open %s failed: %s\n", fromfile, strerror(errno));
        return -1;
    }
    in = (unsigned char*)malloc(inl);
    if (read(fd, in, inl) != inl) {
        perror("read");
        free(in);
        close(fd);
        return -1;
    }
    close(fd);
		
    if (argv[1][1] == 'e') {
        err = encrypt_buffer(pass, &out, &outl, in, inl);
        if (err) {
            fprintf(stderr, "encrypt failed.\n");
            free(in);
            return -1;
        }
    } else {
        err = decrypt_buffer(pass, &out, &outl, in, inl);
        if (err) {
            fprintf(stderr, "encrypt failed.\n");
            free(in);
            return -1;
        }
    }
	
    free(in);
	
    /* write result to file */
    fd = open(tofile, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
    if (fd < 0) {
        fprintf(stderr, "open %s failed: %s\n", tofile, strerror(errno));
        free(out);
        return -1;
    }
    err = write(fd, out, outl);
    if (err != outl) {
        perror("write");
        close(fd);
        free(out);
        return -1;
    }
    close(fd);
    free(out);
    printf("success.\n");
    return 0;
}

#endif
