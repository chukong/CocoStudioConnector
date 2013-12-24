
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
 * file   : VitJSSerialization.cpp
 * author : wy
 * date   : 2012-01-01
 * note   : js bind Serialization.
*/

#include "VitJSSerialization.h"

CVitMsgSn*		    CVitJSSerialization::s_pSendMsgSn = NULL;
CVitMsgUSn*		    CVitJSSerialization::s_pRecvMsgUSn = NULL;

CVitJSSerialization::CVitJSSerialization()
{
	s_pSendMsgSn = NULL;
}

CVitJSSerialization::~CVitJSSerialization()
{

}

void CVitJSSerialization::InitJSSerialization( CVitMsgSn* pSendMSG, CVitMsgUSn* pRecvMSG )
{
	s_pSendMsgSn = pSendMSG;
	s_pRecvMsgUSn = pRecvMSG;
}
JSBool CVitJSSerialization::SetBuffChar( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_INT( argv[ 0 ] ) )
	{
		char cValue = JSVAL_TO_INT( argv[ 0 ] );
		printf( "char Value == %d\n", cValue );
		*CVitJSSerialization::s_pSendMsgSn << cValue;
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffChar Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffShort( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_INT( argv[ 0 ] ) )
	{
		short sValue = JSVAL_TO_INT( argv[ 0 ] );
		printf( "Short Value == %d\n", sValue );
		*CVitJSSerialization::s_pSendMsgSn << sValue;
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );

		printf( "SetBuffShort Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffInt( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_INT( argv[ 0 ] ) )
	{
		int iValue = JSVAL_TO_INT( argv[ 0 ] );
		printf( "int Value == %d\n", iValue );
		*CVitJSSerialization::s_pSendMsgSn << iValue;
	}	
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );

		printf( "SetBuffInt Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffUChar( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_INT( argv[ 0 ] ) )
	{
		unsigned char cValue = JSVAL_TO_INT( argv[ 0 ] );
		printf( "unsigned char Value == %d\n", cValue );
		*CVitJSSerialization::s_pSendMsgSn << cValue;
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );

		printf( "SetBuffChar Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffUShort( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_INT( argv[ 0 ] ) )
	{
		unsigned short sValue = JSVAL_TO_INT( argv[ 0 ] );
		printf( "unsigned Short Value == %d\n", sValue );
		*CVitJSSerialization::s_pSendMsgSn << sValue;
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffShort Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffUInt( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_INT( argv[ 0 ] ) )
	{
		unsigned int iValue = JSVAL_TO_INT( argv[ 0 ] );
		printf( "unsigned int Value == %d\n", iValue );
		*CVitJSSerialization::s_pSendMsgSn << iValue;
	}	
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffInt Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffFloat( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_NUMBER( argv[ 0 ] ) )
	{
		float fValue = 0.f;
		if ( JSVAL_IS_DOUBLE( argv[ 0 ] ) )
		{
			fValue = JSVAL_TO_DOUBLE( argv[ 0 ] );
		}
		else
		{
			fValue = JSVAL_TO_INT( argv[ 0 ] );
		}	
		printf( "flaot Value == %f\n", fValue );
		*CVitJSSerialization::s_pSendMsgSn << fValue;
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffFloat Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffDouble( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_DOUBLE( argv[ 0 ] ) )
	{
		double dValue = JSVAL_TO_DOUBLE( argv[ 0 ] );
		printf( "double Value == %f\n", dValue );
		*CVitJSSerialization::s_pSendMsgSn << dValue;
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffDouble Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffBool( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );

	if ( JSVAL_IS_BOOLEAN( argv[ 0 ] ) )
	{
		bool bValue = JSVAL_TO_BOOLEAN( argv[ 0 ] );
		printf( "bool Value == %d\n", bValue );
		*CVitJSSerialization::s_pSendMsgSn << bValue;
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffBool Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffString( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );

	if ( JSVAL_IS_STRING( argv[ 0 ] ) )
	{
		JSString* str = JSVAL_TO_STRING( argv[ 0 ] );
		char* strValue = JS_EncodeString( cx, str );
		printf( "string Value == %s\n", strValue );
		*CVitJSSerialization::s_pSendMsgSn << strValue;
		JS_free(cx, strValue);
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffString Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffObject( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );

	if ( JSVAL_IS_OBJECT( argv[ 0 ] ) )
	{
		JSObject* pJSObject = JSVAL_TO_OBJECT( argv[ 0 ] );

		if( JS_IsArrayObject( cx, pJSObject ) )
		{
			printf( "JSObject is Array \n" );
			return JS_FALSE;
		}
		JSBool bhasMethod;
		JS_HasProperty(cx, pJSObject, "Serialize", &bhasMethod);
		if (bhasMethod) 
		{
			jsval pFunction, rval;
			JS_GetProperty(cx, pJSObject, "Serialize", &pFunction);
			printf( "Call object function [ Serialize ] ****** start \n" );
			JS_CallFunctionValue(cx, pJSObject, pFunction, 0, 0, &rval);
			printf( "Call object function [ Serialize ] ****** End \n" );
		}
		else
		{
			printf( "SetBuffObject JSObject not have this function [ Serialize ]\n" );
			return JS_FALSE;
		}
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffObject Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffBoolAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_OBJECT( argv[ 0 ] ) )
	{
		JSObject* pObj = JSVAL_TO_OBJECT( argv[ 0 ] );
		if( JS_IsArrayObject( cx, pObj ) )
		{
			uint32_t nLength = 0;
			JS_GetArrayLength( cx, pObj, &nLength );
			printf( "bool Array Length == [ %d ]\n", nLength );
			bool* bValueAry = new bool[ nLength ];
			for ( uint32_t iNum = 0; iNum < nLength; ++iNum )
			{
				jsval ary;
				if( JS_GetElement( cx, pObj, iNum, &ary ) && JSVAL_IS_BOOLEAN( ary ) )
				{
					bValueAry[ iNum ] = JSVAL_TO_BOOLEAN( ary );
					printf( "bool Array Value == %d\n", bValueAry[ iNum ] );
				}
				else
				{
					JSType nType = JS_TypeOfValue( cx, ary );
					printf( "bool Array Value not bool  [ %s ]\n", JS_GetTypeName( cx, nType ) );
					delete[] bValueAry;
					return JS_FALSE;
				}
			}

			*CVitJSSerialization::s_pSendMsgSn << (char)nLength;

			for ( uint32_t i = 0; i < nLength; ++i )
			{
				*CVitJSSerialization::s_pSendMsgSn << bValueAry[ i ];
			}
			delete[] bValueAry;
		}
		else
		{
			printf( "SetBuffBoolAry is not  Array \n");
			return JS_FALSE;
		}
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffBoolAry Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffCharAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_OBJECT( argv[ 0 ] ) )
	{
		JSObject* pObj = JSVAL_TO_OBJECT( argv[ 0 ] );
		if( JS_IsArrayObject( cx, pObj ) )
		{
			uint32_t nLength = 0;
			JS_GetArrayLength( cx, pObj, &nLength );
			char* cValueAry = new char[ nLength ];

			printf( "char Array Length == [ %d ] \n", nLength );
			for ( uint32_t iNum = 0; iNum < nLength; ++iNum )
			{
				jsval ary;
				if( JS_GetElement( cx, pObj, iNum, &ary ) && JSVAL_IS_INT( ary ) )
				{
					cValueAry[ iNum ] = JSVAL_TO_INT( ary );
					printf( "char Array Value == %d\n", cValueAry[ iNum ] );
				}
				else
				{
					JSType nType = JS_TypeOfValue( cx, ary );
					printf( "char Array Value not char  [ %s ]\n", JS_GetTypeName( cx, nType ) );
					delete[] cValueAry;
					return JS_FALSE;
				}
			}

			*CVitJSSerialization::s_pSendMsgSn << (char)nLength;

			for ( uint32_t i = 0; i < nLength; ++i )
			{
				*CVitJSSerialization::s_pSendMsgSn << cValueAry[ i ];
			}
			delete[] cValueAry;
		}
		else
		{
			printf( "SetBuffCharAry is not  Array \n");
			return JS_FALSE;
		}
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffCharAry Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffShortAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_OBJECT( argv[ 0 ] ) )
	{
		JSObject* pObj = JSVAL_TO_OBJECT( argv[ 0 ] );
		if( JS_IsArrayObject( cx, pObj ) )
		{
			uint32_t nLength = 0;
			JS_GetArrayLength( cx, pObj, &nLength );
			printf( "short Array Length == [ %d ]\n", nLength );
			short* sValueAry = new short[ nLength ];
			for ( uint32_t iNum = 0; iNum < nLength; ++iNum )
			{
				jsval ary;
				if( JS_GetElement( cx, pObj, iNum, &ary ) && JSVAL_IS_INT( ary ) )
				{
					sValueAry[ iNum ] = JSVAL_TO_INT( ary );
					printf( "short Array Value == %d\n", sValueAry[ iNum ] );
				}
				else
				{
					JSType nType = JS_TypeOfValue( cx, ary );
					printf( "short Array Value not short  [ %s ]\n", JS_GetTypeName( cx, nType ) );
					delete[] sValueAry;
					return JS_FALSE;
				}
			}

			*CVitJSSerialization::s_pSendMsgSn << (char)nLength;

			for ( uint32_t i = 0; i < nLength; ++i )
			{
				*CVitJSSerialization::s_pSendMsgSn << sValueAry[ i ];
			}
			delete[] sValueAry;
		}
		else
		{
			printf( "SetBuffShortAry is not  Array \n");
			return JS_FALSE;
		}
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffShortAry Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffIntAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_OBJECT( argv[ 0 ] ) )
	{
		JSObject* pObj = JSVAL_TO_OBJECT( argv[ 0 ] );
		if( JS_IsArrayObject( cx, pObj ) )
		{
			uint32_t nLength = 0;
			JS_GetArrayLength( cx, pObj, &nLength );
			printf( "int Array Length == [ %d ]\n", nLength );
			int* iValueAry = new int[ nLength ];
			for ( uint32_t iNum = 0; iNum < nLength; ++iNum )
			{
				jsval ary;
				if( JS_GetElement( cx, pObj, iNum, &ary ) && JSVAL_IS_INT( ary ) )
				{
					iValueAry[ iNum ] = JSVAL_TO_INT( ary );
					printf( "int Array Value == %d\n", iValueAry[ iNum ] );
				}
				else
				{
					JSType nType = JS_TypeOfValue( cx, ary );
					printf( "int Array Value not bool  [ %s ]\n", JS_GetTypeName( cx, nType ) );
					delete[] iValueAry;
					return JS_FALSE;
				}
			}

			*CVitJSSerialization::s_pSendMsgSn << (char)nLength;

			for ( uint32_t i = 0; i < nLength; ++i )
			{
				*CVitJSSerialization::s_pSendMsgSn << iValueAry[ i ];
			}
			delete[] iValueAry;
		}
		else
		{
			printf( "SetBuffIntAry is not  Array \n");
			return JS_FALSE;
		}
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffIntAry Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffUCharAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_OBJECT( argv[ 0 ] ) )
	{
		JSObject* pObj = JSVAL_TO_OBJECT( argv[ 0 ] );
		if( JS_IsArrayObject( cx, pObj ) )
		{
			uint32_t nLength = 0;
			JS_GetArrayLength( cx, pObj, &nLength );
			printf( "unsigned char Array Length == [ %d ]\n", nLength );
			unsigned char* cValueAry = new unsigned char[ nLength ];
			for ( uint32_t iNum = 0; iNum < nLength; ++iNum )
			{
				jsval ary;
				if( JS_GetElement( cx, pObj, iNum, &ary ) && JSVAL_IS_INT( ary ) )
				{
					cValueAry[ iNum ] = JSVAL_TO_INT( ary );
					printf( "unsigned char Array Value == %d\n", cValueAry[ iNum ] );
				}
				else
				{
					JSType nType = JS_TypeOfValue( cx, ary );
					printf( "unsigned char Array Value not unsigned char   [ %s ]\n", JS_GetTypeName( cx, nType ) );
					delete[] cValueAry;
					return JS_FALSE;
				}
			}

			*CVitJSSerialization::s_pSendMsgSn << (char)nLength;

			for ( uint32_t i = 0; i < nLength; ++i )
			{
				*CVitJSSerialization::s_pSendMsgSn << cValueAry[ i ];
			}
			delete[] cValueAry;
		}
		else
		{
			printf( "SetBuffUCharAry is not  Array \n");
			return JS_FALSE;
		}
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffUCharAry Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffUShortAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_OBJECT( argv[ 0 ] ) )
	{
		JSObject* pObj = JSVAL_TO_OBJECT( argv[ 0 ] );
		if( JS_IsArrayObject( cx, pObj ) )
		{
			uint32_t nLength = 0;
			JS_GetArrayLength( cx, pObj, &nLength );
			printf( "unsigned short Array Length == [ %d ]\n", nLength );
			unsigned short* sValueAry = new unsigned short[ nLength ];
			for ( uint32_t iNum = 0; iNum < nLength; ++iNum )
			{
				jsval ary;
				if( JS_GetElement( cx, pObj, iNum, &ary ) && JSVAL_IS_INT( ary ) )
				{
					sValueAry[ iNum ] = JSVAL_TO_INT( ary );
					printf( "unsigned short Array Value == %d\n", sValueAry[ iNum ] );
				}
				else
				{
					JSType nType = JS_TypeOfValue( cx, ary );
					printf( "unsigned short Array Value not unsigned short   [ %s ]\n", JS_GetTypeName( cx, nType ) );
					delete[] sValueAry;
					return JS_FALSE;
				}
			}

			*CVitJSSerialization::s_pSendMsgSn << (char)nLength;

			for ( uint32_t i = 0; i < nLength; ++i )
			{
				*CVitJSSerialization::s_pSendMsgSn << sValueAry[ i ];
			}
			delete[] sValueAry;
		}
		else
		{
			printf( "SetBuffUShortAry is not  Array \n");
			return JS_FALSE;
		}
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffUShortAry Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffUIntAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_OBJECT( argv[ 0 ] ) )
	{
		JSObject* pObj = JSVAL_TO_OBJECT( argv[ 0 ] );
		if( JS_IsArrayObject( cx, pObj ) )
		{
			uint32_t nLength = 0;
			JS_GetArrayLength( cx, pObj, &nLength );
			printf( "unsigned int Array Length == [ %d ]\n", nLength );
			unsigned int* iValueAry = new unsigned int[ nLength ];
			for ( uint32_t iNum = 0; iNum < nLength; ++iNum )
			{
				jsval ary;
				if( JS_GetElement( cx, pObj, iNum, &ary ) && JSVAL_IS_INT( ary ) )
				{
					iValueAry[ iNum ] = JSVAL_TO_INT( ary );
					printf( "unsigned int Array Value == %d\n", iValueAry[ iNum ] );
				}
				else
				{
					JSType nType = JS_TypeOfValue( cx, ary );
					printf( "unsigned int Array Value not unsigned int   [ %s ]\n", JS_GetTypeName( cx, nType ) );
					delete[] iValueAry;
					return JS_FALSE;
				}
			}

			*CVitJSSerialization::s_pSendMsgSn << (char)nLength;

			for ( uint32_t i = 0; i < nLength; ++i )
			{
				*CVitJSSerialization::s_pSendMsgSn << iValueAry[ i ];
			}
			delete[] iValueAry;
		}
		else
		{
			printf( "SetBuffUIntAry is not  Array \n");
			return JS_FALSE;
		}
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffIntAry Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}
JSBool CVitJSSerialization::SetBuffFloatAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_OBJECT( argv[ 0 ] ) )
	{
		JSObject* pObj = JSVAL_TO_OBJECT( argv[ 0 ] );
		if( JS_IsArrayObject( cx, pObj ) )
		{
			uint32_t nLength = 0;
			JS_GetArrayLength( cx, pObj, &nLength );
			printf( "float Array Length == [ %d ]\n", nLength );
			float* fValueAry = new float[ nLength ];
			for ( uint32_t iNum = 0; iNum < nLength; ++iNum )
			{
				jsval ary;
				if( JS_GetElement( cx, pObj, iNum, &ary ) && JSVAL_IS_DOUBLE( ary ) )
				{
					fValueAry[ iNum ] = JSVAL_TO_DOUBLE( ary );
					printf( "flaot Array Value == %f\n", fValueAry[ iNum ] );
				}
				else
				{
					JSType nType = JS_TypeOfValue( cx, ary );
					printf( "float Array Value not float  [ %s ]\n", JS_GetTypeName( cx, nType ) );
					delete[] fValueAry;
					return JS_FALSE;
				}
			}

			*CVitJSSerialization::s_pSendMsgSn << (char)nLength;

			for ( uint32_t i = 0; i < nLength; ++i )
			{
				*CVitJSSerialization::s_pSendMsgSn << fValueAry[ i ];
			}
			delete[] fValueAry;
		}
		else
		{
			printf( "SetBuffFloatAry is not  Array \n");
			return JS_FALSE;
		}
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffFloatAry Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffDoubleAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_OBJECT( argv[ 0 ] ) )
	{
		JSObject* pObj = JSVAL_TO_OBJECT( argv[ 0 ] );
		if( JS_IsArrayObject( cx, pObj ) )
		{
			uint32_t nLength = 0;
			JS_GetArrayLength( cx, pObj, &nLength );
			printf( "double Array Length == [ %d ]\n", nLength );
			double* dValueAry = new double[ nLength ];
			for ( uint32_t iNum = 0; iNum < nLength; ++iNum )
			{
				jsval ary;
				if( JS_GetElement( cx, pObj, iNum, &ary ) && JSVAL_IS_DOUBLE( ary ) )
				{
					dValueAry[ iNum ] = JSVAL_TO_DOUBLE( ary );
					printf( "double Array Value == %f\n", dValueAry[ iNum ] );
				}
				else
				{
					JSType nType = JS_TypeOfValue( cx, ary );
					printf( "double Array Value not double  [ %s ]\n", JS_GetTypeName( cx, nType ) );
					delete[] dValueAry;
					return JS_FALSE;
				}
			}

			*CVitJSSerialization::s_pSendMsgSn << (char)nLength;

			for ( uint32_t i = 0; i < nLength; ++i )
			{
				*CVitJSSerialization::s_pSendMsgSn << dValueAry[ i ];
			}
			delete[] dValueAry;
		}
		else
		{
			printf( "SetBuffDoubleAry is not  Array \n");
			return JS_FALSE;
		}
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffDoubleAry Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}

JSBool CVitJSSerialization::SetBuffObjectAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );
	if ( JSVAL_IS_OBJECT( argv[ 0 ] ) )
	{
		JSObject* pObj = JSVAL_TO_OBJECT( argv[ 0 ] );
		if( JS_IsArrayObject( cx, pObj ) )
		{
			uint32_t nLength = 0;
			JS_GetArrayLength( cx, pObj, &nLength );
			printf( "Object Array Length == [ %d ]\n", nLength );
			*CVitJSSerialization::s_pSendMsgSn << (char)nLength;
			jsval ary;
			for ( uint32_t iNum = 0; iNum < nLength; ++iNum )
			{
				if( JS_GetElement( cx, pObj, iNum, &ary ) && JSVAL_IS_OBJECT( ary ) )
				{
					JSObject* pJSObject = JSVAL_TO_OBJECT( ary );

					if( JS_IsArrayObject( cx, pJSObject ) )
					{
						printf( "SetBuffObjectAry  JSObject is Array \n" );
						return JS_FALSE;
					}
					JSBool bhasMethod;

					JS_HasProperty(cx, pJSObject, "Serialize", &bhasMethod);
					if (bhasMethod)
					{
						printf( "run Serialize function: \n " );
						jsval pFunction, rval;
						JS_GetProperty(cx, pJSObject, "Serialize", &pFunction);
						JS_CallFunctionValue(cx, pJSObject, pFunction, 0, 0, &rval);
						printf( "end Serialize function: \n " );
					}
					else
					{
						printf( "SetBuffObjectAry JSObject not have this function [ Serialize ]\n" );
						return JS_FALSE;
					}
				}
				else
				{
					JSType nType = JS_TypeOfValue( cx, ary );
					printf( "Object Array Value not Object  [ %s ]\n", JS_GetTypeName( cx, nType ) );
					return JS_FALSE;
				}
			}
		}
		else
		{
			printf( "SetBuffObjectAry is not  Array \n");
			return JS_FALSE;
		}
	}
	else
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "SetBuffObjectAry Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}
	return JS_TRUE;
}


JSBool CVitJSSerialization::GetBuffChar( JSContext* cx, uint32_t argc, jsval* vp )
{

	char cValue = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> cValue;
	printf( "Get char Value == %d\n", cValue );

	jsval rval = INT_TO_JSVAL( cValue );
	JS_SET_RVAL(cx, vp, rval);

	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffShort( JSContext* cx, uint32_t argc, jsval* vp )
{
	short sValue = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> sValue;
	printf( "Get short Value == %d\n", sValue );

	jsval rval = INT_TO_JSVAL( sValue );
	JS_SET_RVAL(cx, vp, rval);
	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffInt( JSContext* cx, uint32_t argc, jsval* vp )
{
	int iValue = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> iValue;
	printf( "Get int Value == %d\n", iValue );

	jsval rval = INT_TO_JSVAL( iValue );
	JS_SET_RVAL(cx, vp, rval);
	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffUChar( JSContext* cx, uint32_t argc, jsval* vp )
{

	unsigned char cValue = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> cValue;
	printf( "Get char Value == %d\n", cValue );

	jsval rval = UINT_TO_JSVAL( cValue );
	JS_SET_RVAL(cx, vp, rval);

	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffUShort( JSContext* cx, uint32_t argc, jsval* vp )
{
	unsigned short sValue = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> sValue;
	printf( "Get short Value == %d\n", sValue );

	jsval rval = UINT_TO_JSVAL( sValue );
	JS_SET_RVAL(cx, vp, rval);
	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffUInt( JSContext* cx, uint32_t argc, jsval* vp )
{
	unsigned int iValue = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> iValue;
	printf( "Get int Value == %d\n", iValue );

	jsval rval = UINT_TO_JSVAL( iValue );
	JS_SET_RVAL(cx, vp, rval);
	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffFloat( JSContext* cx, uint32_t argc, jsval* vp )
{
	float fValue = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> fValue;
	printf( "Get float Value == %f\n", fValue );

	jsval rval = DOUBLE_TO_JSVAL( fValue );
	JS_SET_RVAL(cx, vp, rval);
	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffDouble( JSContext* cx, uint32_t argc, jsval* vp )
{
	double dValue = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> dValue;
	printf( "Get double Value == %f\n", dValue );

	jsval rval = DOUBLE_TO_JSVAL( dValue );
	JS_SET_RVAL(cx, vp, rval);
	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffBool( JSContext* cx, uint32_t argc, jsval* vp )
{

	bool bValue = false;
	*CVitJSSerialization::s_pRecvMsgUSn >> bValue;
	printf( "Get bool Value == %d\n", bValue );

	jsval rval = INT_TO_JSVAL( bValue );
	JS_SET_RVAL(cx, vp, rval);

	return JS_TRUE;
}


JSBool CVitJSSerialization::GetBuffString( JSContext* cx, uint32_t argc, jsval* vp )
{

	char strValue[ 1024 ];
	memset( strValue, 0x00, sizeof( strValue ) );
	*CVitJSSerialization::s_pRecvMsgUSn >> strValue;
	printf( "Get string Value == %s\n", strValue );
	jsval rval = STRING_TO_JSVAL(JS_NewStringCopyZ( cx, strValue ));
	JS_SET_RVAL(cx, vp, rval);

	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffObject( JSContext* cx, uint32_t argc, jsval* vp )
{
	jsval* argv = JS_ARGV( cx, vp );

	if ( argc < 1 )
	{
		printf( "GetBuffObject Wrong number of arguments [ %d ]\n", argc );
		return JS_FALSE;
	}

	if ( !JSVAL_IS_OBJECT( argv[ 0 ] ) )
	{
		JSType nType = JS_TypeOfValue( cx, argv[ 0 ] );
		printf( "GetBuffObject Type Error [ %s ]\n", JS_GetTypeName( cx, nType ));
		return JS_FALSE;
	}

	JSObject* pJSObject = JSVAL_TO_OBJECT( argv[ 0 ] );

	if( JS_IsArrayObject( cx, pJSObject ) )
	{
		printf( "GetBuffObject JSObject is Array \n" );
		return JS_FALSE;
	}
	JSBool bhasMethod;
	JS_HasProperty(cx, pJSObject, "Deserialize", &bhasMethod);
	if (bhasMethod) 
	{
		jsval pFunction, rval;
		printf( "Call object function [ Deserialize ] ****** start \n" );
		JS_GetProperty(cx, pJSObject, "Deserialize", &pFunction);
		JS_CallFunctionValue(cx, pJSObject, pFunction, 0, 0, &rval);
		printf( "Call object function [ Deserialize ] ****** End \n" );
	}
	else
	{
		printf( "GetBuffObject JSObject not have this function [ Deserialize ]\n" );
		return JS_FALSE;
	}

	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffBoolAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	char nLength = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> nLength;
	bool* bValueAry = new bool[ nLength ];
	printf( "Get Short Array: Length = %d", nLength );
	jsval* argv = new jsval[ nLength ];
	for ( int i = 0; i < nLength; ++i )
	{
		*CVitJSSerialization::s_pRecvMsgUSn >> bValueAry[ i ];
		argv[ i ] = BOOLEAN_TO_JSVAL( bValueAry[ i ] );
		printf( ", %d", bValueAry[ i ] );
	}
	printf( "\n" );
	JSObject* pObject = JS_NewArrayObject( cx, nLength, argv );
	jsval rval = OBJECT_TO_JSVAL( pObject );
	JS_SET_RVAL(cx, vp, rval);
	delete[] bValueAry;
	delete[] argv;
	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffCharAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	char nLength = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> nLength;
	char* cValueAry = new char[ nLength ];
	printf( "Get Short Array: Length = %d", nLength );
	jsval* argv = new jsval[ nLength ];
	for ( int i = 0; i < nLength; ++i )
	{
		*CVitJSSerialization::s_pRecvMsgUSn >> cValueAry[ i ];
		argv[ i ] = INT_TO_JSVAL( cValueAry[ i ] );
		printf( ", %d", cValueAry[ i ] );
	}
	printf( "\n" );
	JSObject* pObject = JS_NewArrayObject( cx, nLength, argv );
	jsval rval = OBJECT_TO_JSVAL( pObject );
	JS_SET_RVAL(cx, vp, rval);
	delete[] cValueAry;
	delete[] argv;
	return JS_TRUE;
}


JSBool CVitJSSerialization::GetBuffShortAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	char nLength = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> nLength;
	short* sValueAry = new short[ nLength ];
	printf( "Get Short Array: Length = %d", nLength );
	jsval* argv = new jsval[ nLength ];
	for ( int i = 0; i < nLength; ++i )
	{
		*CVitJSSerialization::s_pRecvMsgUSn >> sValueAry[ i ];
		argv[ i ] = INT_TO_JSVAL( sValueAry[ i ] );
		printf( ", %d", sValueAry[ i ] );
	}
	printf( "\n" );
	JSObject* pObject = JS_NewArrayObject( cx, nLength, argv );
	jsval rval = OBJECT_TO_JSVAL( pObject );
	JS_SET_RVAL(cx, vp, rval);
	delete[] sValueAry;
	delete[] argv;
	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffIntAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	char nLength = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> nLength;
	int* iValueAry = new int[ nLength ];
	printf( "Get int Array: Length = %d", nLength );
	jsval* argv = new jsval[ nLength ];
	for ( int i = 0; i < nLength; ++i )
	{
		*CVitJSSerialization::s_pRecvMsgUSn >> iValueAry[ i ];
		argv[ i ] = INT_TO_JSVAL( iValueAry[ i ] );
		printf( ", %d", iValueAry[ i ] );
	}
	printf( "\n" );
	JSObject* pObject = JS_NewArrayObject( cx, nLength, argv );
	jsval rval = OBJECT_TO_JSVAL( pObject );
	JS_SET_RVAL(cx, vp, rval);
	delete[] iValueAry;
	delete[] argv;
	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffUCharAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	char nLength = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> nLength;
	unsigned char* cValueAry = new unsigned char[ nLength ];
	printf( "Get unsigned char Array: Length = %d", nLength );
	jsval* argv = new jsval[ nLength ];
	for ( int i = 0; i < nLength; ++i )
	{
		*CVitJSSerialization::s_pRecvMsgUSn >> cValueAry[ i ];
		argv[ i ] = UINT_TO_JSVAL( cValueAry[ i ] );
		printf( ", %d", cValueAry[ i ] );
	}
	printf( "\n" );
	JSObject* pObject = JS_NewArrayObject( cx, nLength, argv );
	jsval rval = OBJECT_TO_JSVAL( pObject );
	JS_SET_RVAL(cx, vp, rval);
	delete[] cValueAry;
	delete[] argv;
	return JS_TRUE;
}


JSBool CVitJSSerialization::GetBuffUShortAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	char nLength = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> nLength;
	unsigned short* sValueAry = new unsigned short[ nLength ];
	printf( "Get unsigned short Array: Length = %d", nLength );
	jsval* argv = new jsval[ nLength ];
	for ( int i = 0; i < nLength; ++i )
	{
		*CVitJSSerialization::s_pRecvMsgUSn >> sValueAry[ i ];
		argv[ i ] = UINT_TO_JSVAL( sValueAry[ i ] );
		printf( ", %d", sValueAry[ i ] );
	}
	printf( "\n" );
	JSObject* pObject = JS_NewArrayObject( cx, nLength, argv );
	jsval rval = OBJECT_TO_JSVAL( pObject );
	JS_SET_RVAL(cx, vp, rval);
	delete[] sValueAry;
	delete[] argv;
	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffUIntAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	char nLength = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> nLength;
	unsigned int* iValueAry = new unsigned int[ nLength ];
	printf( "Get unsigned int Array: Length = %d", nLength );
	jsval* argv = new jsval[ nLength ];
	for ( int i = 0; i < nLength; ++i )
	{
		*CVitJSSerialization::s_pRecvMsgUSn >> iValueAry[ i ];
		argv[ i ] = UINT_TO_JSVAL( iValueAry[ i ] );
		printf( ", %d", iValueAry[ i ] );
	}
	printf( "\n" );
	JSObject* pObject = JS_NewArrayObject( cx, nLength, argv );
	jsval rval = OBJECT_TO_JSVAL( pObject );
	JS_SET_RVAL(cx, vp, rval);
	delete[] iValueAry;
	delete[] argv;
	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffFloatAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	char nLength = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> nLength;
	float* fValueAry = new float[ nLength ];
	printf( "Get float Array: Length = %d", nLength );
	jsval* argv = new jsval[ nLength ];
	for ( int i = 0; i < nLength; ++i )
	{
		*CVitJSSerialization::s_pRecvMsgUSn >> fValueAry[ i ];
		argv[ i ] = DOUBLE_TO_JSVAL( fValueAry[ i ] );
		printf( ", %f", fValueAry[ i ] );
	}
	printf( "\n" );
	JSObject* pObject = JS_NewArrayObject( cx, nLength, argv );
	jsval rval = OBJECT_TO_JSVAL( pObject );
	JS_SET_RVAL(cx, vp, rval);
	delete[] fValueAry;
	delete[] argv;
	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffDoubleAry( JSContext* cx, uint32_t argc, jsval* vp )
{
	char nLength = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> nLength;
	double* dValueAry = new double[ nLength ];
	printf( "Get Double Array: Length = %d", nLength );
	jsval* argv = new jsval[ nLength ];
	for ( int i = 0; i < nLength; ++i )
	{
		*CVitJSSerialization::s_pRecvMsgUSn >> dValueAry[ i ];
		argv[ i ] = DOUBLE_TO_JSVAL( dValueAry[ i ] );
		printf( ", %f", dValueAry[ i ] );
	}
	printf( "\n" );
	JSObject* pObject = JS_NewArrayObject( cx, nLength, argv );
	jsval rval = OBJECT_TO_JSVAL( pObject );
	JS_SET_RVAL(cx, vp, rval);
	delete[] dValueAry;
	delete[] argv;
	return JS_TRUE;
}

JSBool CVitJSSerialization::GetBuffObjectAryLength( JSContext* cx, uint32_t argc, jsval* vp )
{
	char nLength = 0;
	*CVitJSSerialization::s_pRecvMsgUSn >> nLength;
	printf( "Get Object Array: Length = %d\n", nLength );

	jsval rval = INT_TO_JSVAL( nLength );
	JS_SET_RVAL(cx, vp, rval);
	return JS_TRUE;
}



JSFunctionSpec CVitJSSerialization::s_GlobleFunctions[] = {
	JS_FN( "SetBuffChar", CVitJSSerialization::SetBuffChar, 1, 0),
	JS_FN( "SetBuffShort", CVitJSSerialization::SetBuffShort, 1, 0),
	JS_FN( "SetBuffInt", CVitJSSerialization::SetBuffInt, 1, 0),
	JS_FN( "SetBuffUChar", CVitJSSerialization::SetBuffUChar, 1, 0),
	JS_FN( "SetBuffUShort", CVitJSSerialization::SetBuffUShort, 1, 0),
	JS_FN( "SetBuffUInt", CVitJSSerialization::SetBuffUInt, 1, 0),
	JS_FN( "SetBuffFloat", CVitJSSerialization::SetBuffFloat, 1, 0),
	JS_FN( "SetBuffDouble", CVitJSSerialization::SetBuffDouble, 1, 0),
	JS_FN( "SetBuffBool", CVitJSSerialization::SetBuffBool, 1, 0),
	JS_FN( "SetBuffString", CVitJSSerialization::SetBuffString, 1, 0),
	JS_FN( "SetBuffObject", CVitJSSerialization::SetBuffObject, 1, 0),
	JS_FN( "SetBuffBoolAry", CVitJSSerialization::SetBuffBoolAry, 1, 0),
	JS_FN( "SetBuffCharAry", CVitJSSerialization::SetBuffCharAry, 1, 0),
	JS_FN( "SetBuffShortAry", CVitJSSerialization::SetBuffShortAry, 1, 0),
	JS_FN( "SetBuffIntAry", CVitJSSerialization::SetBuffIntAry, 1, 0),
	JS_FN( "SetBuffUCharAry", CVitJSSerialization::SetBuffUCharAry, 1, 0),
	JS_FN( "SetBuffUShortAry", CVitJSSerialization::SetBuffUShortAry, 1, 0),
	JS_FN( "SetBuffUIntAry", CVitJSSerialization::SetBuffUIntAry, 1, 0),
	JS_FN( "SetBuffFloatAry", CVitJSSerialization::SetBuffFloatAry, 1, 0),
	JS_FN( "SetBuffDoubleAry", CVitJSSerialization::SetBuffDoubleAry, 1, 0),
	JS_FN( "SetBuffObjectAry", CVitJSSerialization::SetBuffObjectAry, 1, 0),


	JS_FN( "GetBuffChar", CVitJSSerialization::GetBuffChar, 1, 0),
	JS_FN( "GetBuffShort", CVitJSSerialization::GetBuffShort, 1, 0),
	JS_FN( "GetBuffInt", CVitJSSerialization::GetBuffInt, 1, 0),
	JS_FN( "GetBuffUChar", CVitJSSerialization::GetBuffUChar, 1, 0),
	JS_FN( "GetBuffUShort", CVitJSSerialization::GetBuffUShort, 1, 0),
	JS_FN( "GetBuffUInt", CVitJSSerialization::GetBuffUInt, 1, 0),
	JS_FN( "GetBuffFloat", CVitJSSerialization::GetBuffFloat, 1, 0),
	JS_FN( "GetBuffDouble", CVitJSSerialization::GetBuffDouble, 1, 0),
	JS_FN( "GetBuffBool", CVitJSSerialization::GetBuffBool, 1, 0),
	JS_FN( "GetBuffString", CVitJSSerialization::GetBuffString, 1, 0),
	JS_FN( "GetBuffObject", CVitJSSerialization::GetBuffObject, 1, 0),
	JS_FN( "GetBuffBoolAry", CVitJSSerialization::GetBuffBoolAry, 1, 0),
	JS_FN( "GetBuffCharAry", CVitJSSerialization::GetBuffCharAry, 1, 0),
	JS_FN( "GetBuffShortAry", CVitJSSerialization::GetBuffShortAry, 1, 0),
	JS_FN( "GetBuffIntAry", CVitJSSerialization::GetBuffIntAry, 1, 0),
	JS_FN( "GetBuffUCharAry", CVitJSSerialization::GetBuffUCharAry, 1, 0),
	JS_FN( "GetBuffUShortAry", CVitJSSerialization::GetBuffUShortAry, 1, 0),
	JS_FN( "GetBuffUIntAry", CVitJSSerialization::GetBuffUIntAry, 1, 0),
	JS_FN( "GetBuffFloatAry", CVitJSSerialization::GetBuffFloatAry, 1, 0),
	JS_FN( "GetBuffDoubleAry", CVitJSSerialization::GetBuffDoubleAry, 1, 0),
	JS_FN( "GetBuffObjectAryLength", CVitJSSerialization::GetBuffObjectAryLength, 1, 0),
	JS_FS_END
};

