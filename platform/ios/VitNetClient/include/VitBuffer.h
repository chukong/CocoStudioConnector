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
*  FileName : VitBuffer.h
*  Use      : Buffer Manager
*  Version  : Base 1.0.0
*  Author   : Lee
*  Date     : 2012-01-06
*  Note     :
*/

#ifndef _VIT_BUFFER_H_LEE_
#define _VIT_BUFFER_H_LEE_

#include "VitLock.h"

typedef struct
{ 
	char*	 pBuffer;   //--buffer--
	int		 nMaxSize;	//--cur buffer max size--
	int		 nHead;		//--head--
	int		 nTail;		//--tail--
	SVitLock insLock;   //--lock--
}SVitBuffer;
/*
* function: new SvitBuffer and Init 
* param   : maxsize:buffer size
* return  : Has New's SvitBuffer
*/
extern SVitBuffer*	Buffer_New(int maxsize);
/*
* function: free SvitBuffer
* param   : p: SvitBuffer pointer
* return  : void
*/
extern void	Buffer_Free( SVitBuffer* p );
/*
* function: clear SvitBuffer
* param   : p: SvitBuffer pointer
* return  : void
*/
extern void	Buffer_Clear( SVitBuffer* p );
/*
* function: set head
* param   : p: SvitBuffer pointer,head: head
* return  : void
*/
extern void Buffer_SetHead( SVitBuffer* p,int head );
/*
* function: stream  add to cur buffer
* param   : p: SvitBuffer pointer, buf:Want to add the stream, nLen:stream len
* return  : true succed
*/
extern bool_t	Buffer_WriteToBuff( SVitBuffer* p,const void* buf,int len );
/*
* function: From the current in the buffer get stream
* param   : p: SvitBuffer pointer, out_buf:get need's stream, nLen:For the length of the stream
* return  : true succed
*/
extern bool_t	Buffer_ReadFromBuff( SVitBuffer* p,void* out_buf, int len );
/*
* function: check cur buffer bis empty
* param   : p: SvitBuffer pointer, nLen:want check's len
* return  : true is not empty
*/
extern bool_t	Buffer_CheckEmpty( SVitBuffer* p,int len );
/*
* function: check cur buffer bis full
* param   : p: SvitBuffer pointer, nLen:want check's len
* return  : true is not full
*/
extern bool_t Buffer_CheckFull( SVitBuffer* p,int len );
/*
* function: check cur buffer bis empty
* param   : p: SvitBuffer pointer
* return  : true is empty
*/
extern bool_t	Buffer_IsEmpty( SVitBuffer* p );
/*
* function: get in cur buffer Area effectively len
* param   : p: SvitBuffer pointer
* return  : Area effectively len
*/
extern int  Buffer_GetCurLen( SVitBuffer* p );
/*
* function: According to (* ^ # @) mark MSG screenshots
* param   : p: SvitBuffer pointer,out_buf:After the interception of complete information, out_buf:stream size
* return  : true have a msg
*/
extern bool_t Buffer_GetMsg( SVitBuffer* p,void* out_buf,int* out_size );

#endif	// _VIT_BUFFER_H_LEE_
