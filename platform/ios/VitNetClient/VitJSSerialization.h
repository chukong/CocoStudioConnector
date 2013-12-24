
/*
 * Copyright (c) 2012 Beijing Chukong Aipu Technology Co.,Ltd.
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
 * module : VitBase
 * file   : VitJSSerialization.h
 * author : wy
 * date   : 2012-01-01
 * note   : js bind Serialization.
*/

#ifndef _VITJSSERIALIZATION_H
#define _VITJSSERIALIZATION_H
#include "SPJSBind.h"
using namespace SP_JSBind;
#include "VitMsgSn.h"

class CVitJSSerialization
{
public:
	CVitJSSerialization();
	~CVitJSSerialization();
	// 
	static void InitJSSerialization( CVitMsgSn* pSendMSG, CVitMsgUSn* pRecvMSG );

	//
	TYPE_FUNCTION_H( SetBuffChar );

	TYPE_FUNCTION_H( SetBuffShort );

	TYPE_FUNCTION_H( SetBuffInt );

	TYPE_FUNCTION_H( SetBuffUChar );

	TYPE_FUNCTION_H( SetBuffUShort );

	TYPE_FUNCTION_H( SetBuffUInt );

	TYPE_FUNCTION_H( SetBuffFloat );

	TYPE_FUNCTION_H( SetBuffDouble );

	TYPE_FUNCTION_H( SetBuffBool );

	TYPE_FUNCTION_H( SetBuffString );

	TYPE_FUNCTION_H( SetBuffObject );

	TYPE_FUNCTION_H( SetBuffBoolAry );

	TYPE_FUNCTION_H( SetBuffCharAry );

	TYPE_FUNCTION_H( SetBuffShortAry );

	TYPE_FUNCTION_H( SetBuffIntAry );

	TYPE_FUNCTION_H( SetBuffUCharAry );

	TYPE_FUNCTION_H( SetBuffUShortAry );

	TYPE_FUNCTION_H( SetBuffUIntAry );

	TYPE_FUNCTION_H( SetBuffFloatAry );

	TYPE_FUNCTION_H( SetBuffDoubleAry );

	TYPE_FUNCTION_H( SetBuffObjectAry );

	// 
	TYPE_FUNCTION_H( GetBuffChar );

	TYPE_FUNCTION_H( GetBuffShort );

	TYPE_FUNCTION_H( GetBuffInt );

	TYPE_FUNCTION_H( GetBuffUChar );

	TYPE_FUNCTION_H( GetBuffUShort );

	TYPE_FUNCTION_H( GetBuffUInt );

	TYPE_FUNCTION_H( GetBuffFloat );

	TYPE_FUNCTION_H( GetBuffDouble );

	TYPE_FUNCTION_H( GetBuffBool );

	TYPE_FUNCTION_H( GetBuffString );

	TYPE_FUNCTION_H( GetBuffObject );

	TYPE_FUNCTION_H( GetBuffBoolAry );

	TYPE_FUNCTION_H( GetBuffCharAry );

	TYPE_FUNCTION_H( GetBuffShortAry );

	TYPE_FUNCTION_H( GetBuffIntAry );

	TYPE_FUNCTION_H( GetBuffUCharAry );

	TYPE_FUNCTION_H( GetBuffUShortAry );

	TYPE_FUNCTION_H( GetBuffUIntAry );

	TYPE_FUNCTION_H( GetBuffFloatAry );

	TYPE_FUNCTION_H( GetBuffDoubleAry );

	TYPE_FUNCTION_H( GetBuffObjectAryLength );
public:
	static JSFunctionSpec		s_GlobleFunctions[];		// 
	
public:
	static CVitMsgSn*		    s_pSendMsgSn;				// 
	static CVitMsgUSn*		    s_pRecvMsgUSn;				// 
};







#endif // _VITJSSERIALIZATION_H


