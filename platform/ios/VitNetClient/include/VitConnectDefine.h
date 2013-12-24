/*
* Copyright (c) 2012 Chukong Technologies, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the
* following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
* NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
* USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*  FileName : VitConnectDefine.h
*  Use      : Connect Define
*  Version  : Base 1.0.0
*  Author   : Lee
*  Date     : 2012-01-06
*  Note     :
*/

#ifndef _VIT_CONNECT_DEFINE_H_LEE_
#define _VIT_CONNECT_DEFINE_H_LEE_

//---------所有对象的线程锁 需要枷锁的结构体必须带有此变量-----------
#define ALLOBJ_LOCK(x) { x->insLock.Lock(); }
#define ALLOBJ_UNLOCK(x) { x->insLock.UnLock(); }

#define READ_MAX 2048
#define CONNECT_READ_MAX 65535
#define SEND_MAX 1024*10
#define SEND_MSG_SIZE 1024
#define PROTOCOL_PROCESS 0xc000
#define PROTOCOL_ZIP     0x4000 //1000000000
#define PROTOCOL_ENTRYPT 0x8000 //0100000000
//#define ZIP_PROTOCOL 10000
//typedef int           bool_t;

//typedef _Bool bool ;

typedef unsigned short packetlen_t ;
typedef unsigned short protocol_t ;
typedef char           bool_t;

#define vitconst_uint_ptr      const int *

/************************************************************************/
/*                           SHeader_TCP                                */
/************************************************************************/
typedef struct 
{
	protocol_t   nID;      //--protocol--
}SHeader_TCP_ID;
#define HEAD_TCP_ID_SIZE sizeof( SHeader_TCP_ID )

//--LEN ID--
typedef struct 
{
	protocol_t   nID;      //--protocol--
	packetlen_t  nLength;  //--The Before compression bag body size--
}SHeader_TCP_ZIP;
#define HEAD_TCP_ZIP sizeof( SHeader_TCP_ZIP )

/************************************************************************/
/*                           SHeader_UDP                                */
/************************************************************************/
//--LEN ID--
typedef struct 
{
	packetlen_t  nLength;  //--The packet size--
	protocol_t   nID;      //--protocol--
}SHeader_UDP_ID_LEN;
#define HEAD_UDP_ID_LEN sizeof( SHeader_UDP_ID_LEN )

//--AFTERZIPLEN ID BEFOREZIPLEN--
typedef struct 
{
	packetlen_t  nAfterZipLen; //--The packet size after zip
	protocol_t   nID;          //--protocol--
	packetlen_t  nLength;      //--The Before compression bag body size--
}SHeader_UDP_ZIP;
#define HEAD_UDP_ZIP sizeof( SHeader_UDP_ZIP )


#endif
