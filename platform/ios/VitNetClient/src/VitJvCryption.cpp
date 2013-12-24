#include "VitPcheader.h"
#include "VitConnectDefine.h"
#include "VitJvCryption.h"
#include "VitCrc_32_table.h"

void Cryption_JvDecryption( long pubkey,long prikey,int len, char *datain, char *dataout )
{
	int  i;
	int  rkey;
	long tkey;
	char *pkey, lkey, rsk;
    
	tkey = pubkey ^ prikey;
	pkey = (char *)&tkey;
	rkey = 2157;
	lkey = (len * 157) & 0xff;

	for( i = 0; i < len; i++ ) 
	{
		rsk = (rkey >> 8) & 0xff;
		dataout[i] = ((datain[i] ^ lkey) ^ pkey[(i % 8)]) ^ rsk;
		rkey *= 2171;
	}
	return;
}

int Cryption_JvDecryptionWithCRC32(long pubkey,long prikey,int len, char *datain, char *dataout)
{
	unsigned long dwCrc32 = 0, dwCalcCrc32 = ~((unsigned long)0);
	unsigned int nCrc32Len = len - sizeof(unsigned long);

	Cryption_JvDecryption( pubkey,prikey,len, datain, dataout );
	memcpy( &dwCrc32, dataout + nCrc32Len, sizeof( unsigned long ) );
	dwCalcCrc32 = Cryption_crc32c( dataout, nCrc32Len, dwCalcCrc32 );
	if(dwCalcCrc32 == dwCrc32) 
		return nCrc32Len;
	else
		return -1;
}


unsigned long Cryption_crc32c(char *pBuf, int size,unsigned long dwCrc)
{
	int it;
	for(it=0; it<size; it++)
	{
		dwCrc = CRC_32_TABLE[(dwCrc^(pBuf[it]|0x00000000))&0x000000FF]^((dwCrc>>8)&0x00FFFFFF);
	}

	return dwCrc;
}


//====================================TEA=================================
//Encrypt
void TEA_Encrypt(unsigned char *out, unsigned char *data, unsigned char *key)
{
	int i,j;
	unsigned int y=0,z=0,a,b,c,d;
	unsigned int sum = 0;
	unsigned delta = 0x9E3779B9;
	unsigned int k[4] = {0};	//4 part Keys

	//set 4 part Keys
	for (i=0; i<4; i++)
	{
		for(j=3; j>-1; j--)
		{
			k[i] |= key[i*4+j];
			if (j!=0)
			{
				k[i] <<= 8;
			}
		}
	}

	//set y
	for (i=3; i>-1; i--)
	{
		y |= data[i];
		if (i!=0)
		{
			y <<= 8;
		}
	}

	//set z
	for (i=3; i>-1; i--)
	{
		z |= data[i+4];
		if (i!=0)
		{
			z <<= 8;
		}
	}

	//initial a,b,c,d
	a = k[0]; b =k[1]; c = k[2]; d = k[3];

	//encrypt
	for(i=0; i<32; i++)
	{
		sum += delta;
		y +=(z<<4) + a^z + sum^(z>>5) + b;
		z +=(y<<4) + c^y + sum^(y>>5) + d;
	}

	//output y
	for (i=0; i<4; i++)
	{
		out[i] = (y >> (8*i)) & 0xFF;
	}

	//output z
	for (i=0; i<4; i++)
	{
		out[i+4] = (z >> (8*i)) & 0xFF;
	}
}

//Decrypt
void TEA_Decrypt(unsigned char *out, unsigned char *data, unsigned char *key)
{
	int i,j;
	unsigned int y=0,z=0,a,b,c,d;
	unsigned int sum = 0xC6EF3720;
	unsigned delta = 0x9E3779B9;
	unsigned int k[4] = {0};	//4 part Keys

	//set 4 part Keys
	for (i=0; i<4; i++)
	{
		for(j=3; j>-1; j--)
		{
			k[i] |= key[i*4+j];
			if (j!=0)
			{
				k[i] <<= 8;
			}
		}
	}

	//set y
	for (i=3; i>-1; i--)
	{
		y |= data[i];
		if (i!=0)
		{
			y <<= 8;
		}
	}

	//set z
	for (i=3; i>-1; i--)
	{
		z |= data[i+4];
		if (i!=0)
		{
			z <<= 8;
		}
	}

	//initial a,b,c,d
	a = k[0]; b =k[1]; c = k[2]; d = k[3];

	//Decrypt
	for(i=0; i<32; i++)
	{
		z -= (y<<4) + c^y + sum^(y>>5) + d;
		y -= (z<<4) + a^z + sum^(z>>5) + b;
		sum -= delta;	
	}

	//output y
	for (i=0; i<4; i++)
	{
		out[i] = (y >> (8*i)) & 0xFF;
	}

	//output z
	for (i=0; i<4; i++)
	{
		out[i+4] = (z >> (8*i)) & 0xFF;
	}
}