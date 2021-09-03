#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include "prod_config.h"

#define XOR_NUM "Rn/1chD693T^!135565=*--.|??1Su34mG!"

//将input转换成base64编码格式并返回
static unsigned char* b64_dec(char *input, size_t* out_len)
{
    BIO *bmem, *b64;
    size_t in_len=strlen(input);
    size_t out_max_len=(in_len*6+7)/8;
    unsigned char *buf = (unsigned char*)malloc(out_max_len);
    if (buf) {
        memset(buf, 0, out_max_len);
        b64 = BIO_new(BIO_f_base64());
        if (b64) {
            BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
            bmem = BIO_new_mem_buf((char*)input, in_len);
            b64 = BIO_push(b64, bmem);
            *out_len=BIO_read(b64, buf, out_max_len);
            BIO_free_all(b64);
        }
    }
    return buf;
}

static int decsn(const unsigned char* src, unsigned char *dest)
{	
    RSA *key=NULL,*tmp=NULL;
    int i;
    //pem格式的串
    char pubkey[]={
	0x54,0x54,0x54,0x54,0x54,0x3b,0x3c,0x3e,0x30,0x37,0x59,0x29,0x2c,0x3b,0x35,0x30,0x3a,0x59,0x32,0x3c,0x20,0x54,0x54,0x54,0x54,0x54,0x73,0x34,0x3d,0x3c,0xe,0x3d,0x28,0x20,0x33,0x32,0x16,0x23,0x30,0x11,0xf,0x1a,0x37,0x38,0x28,0x3c,0x3b,0x3b,0x28,0x38,0x3d,0x30,0x38,0x38,0xe,0x31,0x28,0x30,0x2e,0x38,0x32,0x4e,0x1b,0x48,0x40,0x40,0x56,0x1b,0x2a,0x3a,0x3c,0x48,0x3a,0x2c,0x4a,0x30,0x1a,0x3e,0x12,0x1c,0x11,0x41,0x31,0x32,0x38,0x41,0x14,0x16,0xe,0x30,0x3d,0x73,0x38,0x28,0x38,0x3b,0x73,0x54,0x54,0x54,0x54,0x54,0x3c,0x37,0x3d,0x59,0x29,0x2c,0x3b,0x35,0x30,0x3a,0x59,0x32,0x3c,0x20,0x54,0x54,0x54,0x54,0x54,0x73,0x79
    };

    key = RSA_new();
    //char buf[1024];
    for(i=0;i<sizeof(pubkey);++i)
        pubkey[i]^=0x79;
    BIO *bio=BIO_new_mem_buf(pubkey,sizeof(pubkey));
    if(!bio) return -1;
    tmp=PEM_read_bio_RSA_PUBKEY(bio,&key,NULL,NULL); //解析出公钥放入key中
    //	int ret=RSA_public_decrypt(21, (unsigned char*)src, 
    //			(unsigned char*)dest, key, RSA_PKCS1_PADDING);
    int ret=RSA_public_decrypt(21, (unsigned char*)src,
                               (unsigned char*)dest, key, RSA_NO_PADDING);
    /*
      for(i=0;i<21;i++)
      printf("%02x:",dest[i]);
    */
    RSA_free(key);
    BIO_free_all(bio);
    return ret;
}

//将小写字母全部转成大写字母,并将-去除
static int checkinput(char *buf)
{
    int len;
    int i,j;
    char tmp[1024];

    strcpy(tmp,buf);
    len=strlen(tmp);
    if(len!=39) return -1;
    for(i=0,j=0;i<len;++i) {
        if(isdigit(tmp[i]) || (tmp[i]>='A' && tmp[i]<='Z'))
            buf[j++]=tmp[i];
        else if(tmp[i]>='a' && tmp[i]<='z')
            buf[j++]=tmp[i]-('a'-'A');
        else if(tmp[i]=='-')
            continue;
        else return -1;
    }
    if(j!=35) return -1;
    buf[j]='\0';
    return j;
}

static int getb64(const char* sn,char *b64)
{
    //unsigned char tmp[28]={0};
    unsigned char tmpchar;
    static char starr[256];
    static char codes[]="0123456789ABCDEFGHIJKLMNOPQRSTUV";
    int i,j;
    int cur=0;
    for(i='0';i<='9';++i)
        starr[i]=i-'0';
    for(j='A';j<='V';++j)
        starr[j]=j-'A'+10;
    for(i=0;i<28;++i) {
        tmpchar=((starr[sn[28+(i/5)]])>>(4-i%5))&1;
        if(tmpchar) {
            cur+=sn[i];
            if(sn[i]>='A' && sn[i]<='Z')
                b64[i]=sn[i]+('a'-'A');
            else if(sn[i]=='7')
                b64[i]='+';
            else if(sn[i]=='9')
                b64[i]='/';
            else if(sn[i]=='5')
                b64[i]='=';
            else return -1;
        }
        else
            b64[i]=sn[i];
    }
    b64[i]='\0';
    if(sn[34]!=codes[cur%32])
        return -1;
    return i;
}

static int checkver(const char* edt,const char* ver)
{
    // if verion check is needed, add strcmp here, like:
    //	if(strcmp(edt,"redhat") || strcmp(ver,"53"))  return -1;
    return 0;
}

int get_product_info(const char* sn, struct product_info* info)
{
    unsigned char *buf,b64[100];
    int len;
    int i;
    char input[100];
    unsigned char dest[100];
	
    if(sn == NULL) {
        printf("Invalid License\n");
        return -1;
    }
    else {
        if(strlen(sn) != S_SERIAL + 4) {
            printf("Invalid License\n");
	    return -1;
        }
    }

    strcpy(input,sn);
    if(checkinput(input)<0) {
        printf("Invalid License\n");
        return -1;
    }
    if(getb64(input,b64)<0) {
        printf("Invalid License\n");
        return -1;
    }

    //lhx
    buf=b64_dec((char *)b64,(size_t*)&len);
	
    for(i=0;i<21;++i) {
        buf[i]=(buf[i]^XOR_NUM[i+5])-0x18;
    }

    int ret=decsn(buf,dest);
    free(buf);
    if(ret!=-1) {
        //memcpy(info,dest,10);
        //skip padding
        int len = sizeof(struct product_info); //10 bety
        memcpy(info, dest+21-len, len);
    }
    else {
        printf("Invalid License\n");
        return -1;
    }
    return 0;
}

