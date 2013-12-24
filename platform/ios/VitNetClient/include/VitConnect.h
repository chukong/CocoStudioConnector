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
*  FileName : VitConnect.h
*  Use      : Connect
*  Version  : Base 1.0.0
*  Author   : Lee
*  Date     : 2012-01-06
*  Note     :
*/

#ifndef _VIT_CONNECT_H_LEE_
#define _VIT_CONNECT_H_LEE_

#include "VitPlatformConfig.h"
#include "VitMsgSn.h"
#include "VitList.h"
#include "VitBuffer.h"

#if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
#include <WinSock2.h>
#endif

typedef enum
{
    enNone = 0,
    enTcp,
} enConType;

struct SConnectArg
{
	int							fd;					//--connect fd--

#if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
	HANDLE	                    insThreadID;        //--thread handle--
#else
    pthread_t                   insThreadID;        //--thread handle--
#endif
    struct sockaddr_in			ServerAddress;      //--With the server save after server address  (only udp)--
	CVitMsgSn					insSendSn;			//--send need's sn
	CVitMsgUSn                  insRecvSn;
    SListNode*                  pRecvMsg_cb_List;   //--list mgr--
    char*                       pRecvBuffer;        //--RecvBuffer--
    int                         nRecvSize;          //--RecvSize--
	SVitBuffer*					pParseBuffer;	    //--Recv Pack Parse Bufer--
	bool_t						bEncrypt;			//--Whether encrypt--
	bool_t						bZip;				//--Whether zip--
    enConType                   nConType;			//--1 tcp --
	int							nKey[4];			//--encrypt key--
    bool                        bDisconnect;
    Connect_RecvParse_cb        parse_cb;           //--parse recv msg--       
};

/*
* function: new SConnectArg obj
* param   : parsebuffsize : buff size, use_cb:when Registered callback function is true(is not js)
* return  : new's SMsgSn
*/
extern SConnectArg* Connect_New( int parsebuffsize);
/*
* function: connect server
* param   : p:connectarg pointer, ip:server ip,port:server port,encrypt:can encrypt, zip:can zip,contype:tcp udp...
* return  : true ok
*/
extern bool_t Connect_CreateClient( SConnectArg* p,const char* ip,int port,bool_t encrypt,bool_t zip,enConType contype );
/*
* function: send message
* param   : p:connectarg pointer,protocol:protocl id,sn:need send's msg
* return  : not -1 si true
*/
extern int  Connect_SendMessage( SConnectArg* p,protocol_t protocol,CVitMsgSn* sn );
/*
* function: to list add msgcallback
* param   : arg:connectarg pointer,protocol:protocl id,cb : callback
* return  : true ok
*/
extern bool_t Connect_Add_MsgCallback( SConnectArg* arg,unsigned short protocol,Connect_RecvMsg_cb cb );
/*
 * function: Binding to js layer when the call the function need not registered function
 * param   : arg:connectarg pointer,protocol:protocl id,cb : callback
 * return  : true ok
 */
extern bool_t Connect_ParseCallback( SConnectArg* arg,Connect_RecvParse_cb cb );
/*
* function: and server disconnect
* param   : p:connectarg pointer
* return  : true ok
*/
extern void Connect_CloseConnect( SConnectArg* p );
/*
* function: free connectarg obj
* param   : p:connectarg pointer
* return  : void
*/
extern void Connect_Free( SConnectArg* p );
/*
* function: set send recv of key
* param   : p:connectarg pointer ,key val
* return  : void
*/
extern void Connect_SetKey( SConnectArg* p,int key[4] );
/*
 * function: recv message
 * param   : 
 * return  : void
 */
extern void Connect_RecvMessage(  ) ;

extern void Connect_Trigger_MsgCallback( SConnectArg* p,int protocol );

//extern void  Connect_RecvMessage2();


#endif	// _VIT_CONNECT_H_LEE_
