# openssl

## 各种padding对输入数据长度的要求：

​    私钥加密：

​    RSA_PKCS1_PADDING    RSA_size-11

​    RSA_NO_PADDING      RSA_size-0

​    RSA_X931_PADDING     RSA_size-2

​    公钥加密

​    RSA_PKCS1_PADDING    RSA_size-11

​    RSA_SSLV23_PADDING   RSA_size-11

​    RSA_X931_PADDING     RSA_size-2

​    RSA_NO_PADDING      RSA_size-0

​    RSA_PKCS1_OAEP_PADDING      RSA_size-2 * SHA_DIGEST_LENGTH-2  

## RSA加密长度限制问题		

RSA加解密中必须考虑到的密钥长度、明文长度和密文长度问题。明文长度需要小于密钥长度，而密文长度则等于密钥长度。因此当加密内容长度大于密钥长度时，有效的RSA加解密就需要对内容进行分段。

这是因为，RSA算法本身要求加密内容也就是明文长度m必须0<m<密钥长度n。如果小于这个长度就需要进行padding，因为如果没有padding，就无法确定解密后内容的真实长度，字符串之类的内容问题还不大，以0作为结束符，但对二进制数据就很难，因为不确定后面的0是内容还是内容结束符。而只要用到padding，那么就要占用实际的明文长度，于是实际明文长度需要减去padding字节长度。我们一般使用的padding标准有NoPPadding、OAEPPadding、PKCS1Padding等，其中PKCS#1建议的padding就占用了11个字节。

这样，对于1024长度的密钥。128字节（1024bits）-减去11字节正好是117字节，但对于RSA加密来讲，padding也是参与加密的，所以，依然按照1024bits去理解，但实际的明文只有117字节了。

所以如果要对任意长度的数据进行加密，就需要将数据分段后进行逐一加密，并将结果进行拼接。同样，解码也需要分段解码，并将结果进行拼接。

