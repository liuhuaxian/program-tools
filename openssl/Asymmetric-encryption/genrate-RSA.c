#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
int main(){
	RSA *rsa = RSA_generate_key(1024, 65537, NULL, NULL);
	/* 提取私钥 */
	printf("----------提取私钥---------\n-");
	int dlen = RSA_size(rsa);
    	printf("RSA private key len: %d\n",dlen);
	printf("PRIKEY:\n");
	FILE* myprfp = fopen("myprivate.key","w");
	PEM_write_RSAPrivateKey(stdout, rsa, NULL, NULL, 0, NULL, NULL);
	PEM_write_RSAPrivateKey(myprfp, rsa, NULL, NULL, 0, NULL, NULL);
	fflush(myprfp);
	
	/* 提取公钥 */
	printf("------------提取公钥-----------\n");

	printf("PUBKEY: \n");
	FILE* mypufp = fopen("mypublic.key","w");
	PEM_write_RSAPublicKey(stdout, rsa);
	PEM_write_RSAPublicKey(mypufp, rsa);
	fflush(mypufp);
	/*获取私钥二进制表示*/
	printf("--------获取私钥二进制表示------------\n");
	BIO *b =BIO_new_file("myprivate.key","r");
	struct stat statbuf;
	char buf[1024];
	if (stat("myprivate.key", &statbuf) < 0)
	{
		perror("fstat error");
		
	}
	printf("BIO has %d byte\n",statbuf.st_size);
    	BIO_read(b,buf,statbuf.st_size);

	int i;
    	for(i=0;i<statbuf.st_size;++i)
    	{
       		//printf("%02X ",buf[i]);
    	}
	buf[i]='\0';
	printf("\n");

	/*通过二进制文件获取私钥*/
	printf("--------通过二进制文件获取私钥-------------\n");
	RSA *key = RSA_new();
	BIO *bio = BIO_new_mem_buf(buf,strlen(buf));
	if(!bio) return -1;
	if(!PEM_read_bio_RSAPrivateKey(bio, &key, NULL, NULL))
	{
		BIO_free_all(bio);
		return -1;
	}
	printf("private key\n");
	PEM_write_RSAPrivateKey(stdout, key, NULL, NULL, 0, NULL, NULL);


	/*通过私钥对数据进行加密*/
	printf("-----通过私钥对数据进行加密---------\n");
	char *txt = "lhx yll";
	unsigned char des[1024]; 
	unsigned char from[1024];
	int flen=RSA_size(key);
	printf("flen = %d\n",flen);
	for (i=0;i<flen;++i)
	{
		from[i]=i+1;
		printf("%02X",from[i]);
	}
	int len;
	//if ((len = RSA_private_encrypt(flen,txt,des,rsa,RSA_NO_PADDING)) <0)
	if ((len = RSA_private_encrypt(strlen(txt),txt,des,rsa,RSA_PKCS1_PADDING)) <0)
	{
		printf("RSA_private_encrypt error\n");	
		return -1;
	}
	for(i=0;i<len;++i)
		printf("%02X ",des[i]);
	printf("\n");

	/*通过公钥对数据进行解密*/
	printf("---通过公钥对数据进行解密---\n");
	RSA *pukey=RSA_new();
	FILE *pubkeyfd=fopen("mypublic.key","r");
	if (PEM_read_RSAPublicKey(pubkeyfd,&pukey,NULL,NULL)==0)
	{
		printf("get pukey error\n");
		return 0;
	}
	unsigned char test[1024]={'l','h'};
	//int l = RSA_public_decrypt(len,des,test,pukey,RSA_NO_PADDING);
	int l = RSA_public_decrypt(len,des,test,pukey,RSA_PKCS1_PADDING);
	printf("l = %d\n", l);
	if (l < 0)
	{
		printf("RSA_public_decrypt error\n");
		return 0;
	}
	for(i=0;i<l;i++)
	{
		printf("%c",test[i]);
	}
	printf("\n");

	RSA_free(key);
	BIO_free_all(bio);
	RSA_free(rsa);

	return 	0;

}

