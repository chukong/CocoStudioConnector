//
//  EncryptionManager.h
//  ApparkTest
//
//  Created by XiaoFeng on 12-2-1.
//  Copyright (c) 2012年 PunchBox. All rights reserved.
//

#ifndef VitApparkTest_EncryptionManager_h
#define VitApparkTest_EncryptionManager_h

//#include <iostream>
//#include "VitCommonDef.h"
#include "VitConnectDefine.h"

//namespace ApparkSDK
//{
    /*!	\class CEncryptionManager
     *	\brief Implementation base64 and XXTEA(bTEA) encoding and decoding methords.
     *
     * The #CEncryptionManager implementation encrypt & decrypt and binary data 
     * encoding to string & string decoding to binary data functions. All function 
     * in this class is static, can be call directly.
     */

    /*!	\fn Base64Encode(const unsigned char * pInputBuffer, 
        *                      unsigned char * pOutputBuffer, 
        *                      unsigned int nLength)
        *	\brief Encoding binary data to ASCII string.
        *
        *	\param pInputBuffer Buffer of input binary data.
        *  \param pOutputBuffer Buffer to output encode data. Function won't check
        *          buffer overflow. User should provide buffer with enough size.
        *  \param nLength Byte size of data in input buffer.
        *	\return If pOutputBuffer pointer is NULL, function will return buffer 
        *              size needed to contain output data. Caution : This buffer 
        *              size won't contian terminate null char at end, user must 
        *              provide a buffer bigger than return size.
        *          Otherwise return value is the encoded data size .
        */
    unsigned int VitBase64Encode(const unsigned char * pInputBuffer, 
                                unsigned char * pOutputBuffer, 
                                unsigned int nLength);
        
        
    /*!	\fn Base64Decode(const unsigned char * pInputBuffer, 
        *                      unsigned char * pOutputBuffer)
        *	\brief Decoding ASCII string to binary data.
        *
        *	\param pInputBuffer Buffer of input ASCII string.
        *  \param pOutputBuffer Buffer to output decode data. Function won't check
        *          buffer overflow. User should provide buffer with enough size.
        *	\return If pOutputBuffer pointer is NULL, function will return buffer 
        *              size needed to contain output data. This value is a rough
        *              value, maybe bigger than decode data size, but won't less 
        *              than decode size.
        *          Otherwise return value is the decoded data size .
        */
    unsigned int VitBase64Decode(const unsigned char * pInputBuffer, 
                                unsigned char * pOutputBuffer);
        
        
    /*!	\fn XXTEAEncode(const unsigned char * pInputBuffer, 
        *                      unsigned char * pOutputBuffer)
        *	\brief Encrypt source data with XXTEA algorithm.
        *
        *	\param pInputBuffer Buffer of input data.
        *  \param pOutputBuffer Buffer to output encrypt data. Function won't check
        *          buffer overflow. User should provide buffer with enough size.
        *  \param nLength Byte size of data in input buffer. Caution : It's must be
        *          eight bytes aligned. Otherwise may cause unknown error.
        *  \param pUserDefineKey User defined encrypt key. It should be contain 
        *          more than four 32bit interger, otherwise may cause unknown error.
        *          This param set to NULL to use default encrypt key.
        *	\return If pOutputBuffer pointer is NULL, function will return buffer 
        *              size needed to contain output data.
        *          Otherwise return value is the encrypt data size.
        */
    unsigned int VitXXTEAEncode(const unsigned char * pInputBuffer, 
                                unsigned char * pOutputBuffer, 
                                unsigned int nLength, 
                                vitconst_uint_ptr pUserDefineKey);
        
        
    /*!	\fn XXTEADecode(const unsigned char * pInputBuffer, 
        *                      unsigned char * pOutputBuffer)
        *	\brief Decrypt source data with XXTEA algorithm.
        *
        *	\param pInputBuffer Buffer of input data.
        *  \param pOutputBuffer Buffer to output encrypt data. Function won't check
        *          buffer overflow. Buffer should be same or bigger than input 
        *          buffer. If decrypt data is string, user must alloc the terminate
        *          charator memory and set it to 0.
        *  \param nLength Byte size of data in input buffer. Caution : It's must be
        *          eight bytes aligned. Otherwise may cause unknown error.
        *  \param pUserDefineKey User defined encrypt key. It should be contain 
        *          more than four 32bit interger, otherwise may cause unknown error.
        *          This param set to NULL to use default encrypt key.
        *	\return If nLength is not eight bytes aligned, the input data is bad,
        *          return false, otherwise return true.
        */
    bool_t VitXXTEADecode(const unsigned char * pInputBuffer, 
                            unsigned char * pOutputBuffer, 
                            unsigned int nLength, 
                            vitconst_uint_ptr pUserDefineKey);
        
        
    char * VitURLEncode(const char * pInputBuffer, unsigned int nLenth);
        
        
    char * VitURLDecode(const char * pInputBuffer, unsigned int * pnOutLength);


    void VitConfuseString(char * pszText);

#endif
