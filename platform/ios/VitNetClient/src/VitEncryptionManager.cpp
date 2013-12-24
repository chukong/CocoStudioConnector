//
//  EncryptionManager.cpp
//  ApparkTest
//
//  Created by XiaoFeng on 12-2-1.
//  Copyright (c) 2012年 PunchBox. All rights reserved.
//

#include "VitEncryptionManager.h"
#include "VitBaseAlgorithm.h"
#include "VitPcheader.h"

//using namespace ApparkSDK;

#define XXTEA_ALIGNMENT_BYTES   8

const unsigned int DefaultEncryptKey[4] = { 0x474DC838, 0xBB095D81, 0xE38E718F, 0x3FA91527 };

//const char szKey[]= "474DC838BB095D81";

unsigned int VitBase64Encode(const unsigned char * pInputBuffer, 
                                        unsigned char * pOutputBuffer, 
                                        unsigned int nLength)
{
    unsigned int nResult = 0;

    if(pInputBuffer && pOutputBuffer && nLength > 0)
        return VitEnBase64(pOutputBuffer, pInputBuffer, nLength);
    else if(nLength > 0)
        nResult = ((nLength / 3) + (nLength % 3 ? 1 : 0)) * 4;

    return nResult;
}

unsigned int VitBase64Decode(const unsigned char * pInputBuffer, 
                                        unsigned char * pOutputBuffer)
{
    unsigned int nResult = 0;

    if(pInputBuffer && pOutputBuffer)
    {
        return VitUnBase64(pOutputBuffer, pInputBuffer, strlen((char *)pInputBuffer));
    }
    else if(pInputBuffer)
    {
        nResult = strlen((char *)pInputBuffer);
        if(nResult)
            nResult = ((nResult / 4) + (nResult % 4 ? 1 : 0)) * 3;
    }

    return nResult;
}


unsigned int VitXXTEAEncode(const unsigned char * pInputBuffer, 
                                       unsigned char * pOutputBuffer, 
                                       unsigned int nLength, 
                                       vitconst_uint_ptr pUserDefineKey)
{
    unsigned int nResult = 0;
    if (pInputBuffer && pOutputBuffer && nLength > 0)
    {
        nResult = nLength / XXTEA_ALIGNMENT_BYTES + 
                    (nLength % XXTEA_ALIGNMENT_BYTES ? 1 : 0);
        memset(pOutputBuffer, 0, nResult * 8);
        memcpy(pOutputBuffer, pInputBuffer, nLength);
        unsigned int const * pnKey = pUserDefineKey ? (unsigned int const *)pUserDefineKey : DefaultEncryptKey;
        Vitbtea((unsigned int *)pOutputBuffer, nResult * 2, pnKey);

        nResult *= 8;
    }
    else if(nLength > 0)
        nResult = ((nLength / XXTEA_ALIGNMENT_BYTES) + 
                   (nLength % XXTEA_ALIGNMENT_BYTES ? 1 : 0)) * XXTEA_ALIGNMENT_BYTES;

    return nResult;
}


bool_t VitXXTEADecode(const unsigned char * pInputBuffer, 
                                     unsigned char * pOutputBuffer, 
                                     unsigned int nLength, 
                                     vitconst_uint_ptr pUserDefineKey)
{
    if(nLength % 4)
        return 0;

    bool_t result = 0;

    if(pInputBuffer && pOutputBuffer && nLength > 0)
    {
        int nSize = (nLength / XXTEA_ALIGNMENT_BYTES) * 2;
        memset(pOutputBuffer, 0, nLength);
        memcpy(pOutputBuffer, pInputBuffer, nLength);
        unsigned int const * pnKey = pUserDefineKey ? (unsigned int const *)pUserDefineKey : DefaultEncryptKey;
        Vitbtea((unsigned int *)pOutputBuffer, -nSize, pnKey);
        result = 1;
    }

    return result;
}


char * VitURLEncode(const char * pInputBuffer, unsigned int nLenth)
{
    return Vittcurlencode(pInputBuffer, nLenth);
}


char * VitURLDecode(const char * pInputBuffer, unsigned int * pnOutLength)
{
    return Vittcurldecode(pInputBuffer, (int *)pnOutLength);
}


void VitConfuseString(char * pszText)
{
    int nLength = strlen(pszText);
    char *pCurPos = pszText, *pEndPos = pszText + nLength;
    while (pEndPos - pCurPos > 4)
    {
        char cTemp = *pCurPos;
        *pCurPos = *(pCurPos + 1);
        *(pCurPos + 1) = cTemp;
        pCurPos += 2;
    }
}


