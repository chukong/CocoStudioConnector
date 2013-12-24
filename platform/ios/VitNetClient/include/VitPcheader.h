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
*  FileName : VitPcheader.h
*  Use      : System header files
*  Version  : Base 1.0.0
*  Author   : Lee
*  Date     : 2012-01-06
*  Note     :
*/

#ifndef _VIT_PCHEADER_H_LEE_
#define _VIT_PCHEADER_H_LEE_

#include "VitPlatformConfig.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <stdarg.h>

#if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
//#include <WinSock2.h>
//#pragma comment( lib, "ws2_32" )
#else
//#include<netdb.h>
#include <stdarg.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//#include<sys/socket.h>
#endif



#endif	// _VIT_PCHEADER_H_LEE_









