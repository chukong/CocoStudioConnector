
#ifndef _VITJVCRYPION_H_LEE_
#define _VITJVCRYPION_H_LEE_

/*
* function: process encrypt
* param   : pubkey : encrypt public key, prikey : encrypt private key , len : datain len, datain : need encrypt's data, dataout:encrypt after data
* return  : void
*/
extern void Cryption_JvDecryption(long pubkey,long prikey,int len, char *datain, char *dataout);
/*
* function: process deciphering
* param   : pubkey : encrypt public key, prikey : encrypt private key , len : datain len, datain : need deciphering's data, dataout: deciphering after data
* return  : has new's SMsgSn
*/
extern int Cryption_JvDecryptionWithCRC32( long pubkey,long prikey,int len, char *datain, char *dataout );
/*
* function: call encrypt need's file
* param   : pBuf : need shift's data end size
* return  : long
*/
extern unsigned long Cryption_crc32c(char *pBuf, int size,unsigned long dwCrc);

//===========================TEA========================
/*
parameters description:
	out:	8 bytes cipher
	data:	8 bytes plaintext
	key:	16 bytes key
*/
extern void TEA_Encrypt(unsigned char *out, unsigned char *data, unsigned char *key);

/*
parameters description:
	out:	8 bytes plaintext to be output
	data:	8 bytes cipher to be decrypted
	key:	16 bytes key
*/
extern void TEA_Decrypt(unsigned char *out, unsigned char *data, unsigned char *key);


#endif