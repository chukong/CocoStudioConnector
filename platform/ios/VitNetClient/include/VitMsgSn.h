
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
*  FileName : VitMsgSn.h
*  ClassName: VitMsgSn
*  Use      : The encapsulation of the bag of analytical
*  Version  : Base 1.0.0
*  Author   : Lee
*  Date     : 2012-01-31
*  Note     :
*/

#ifndef _CVITMSGSN_H_LEE_
#define _CVITMSGSN_H_LEE_

#include "VitLog.h"


#define CHECKLEN if(m_nLen+sizeof(val)>READ_MAX) \
{ \
	VITLOG( 1, " (%s %d %s) Lee_Error : m_nLen=[%d],sizeof(val)=[%d] Failed",__FILE__,__LINE__,__FUNCTION__,m_nLen,sizeof(val) );\
	return *this;\
}

#define CHECKSHORT if(m_nLen+sizeof(val)>m_nBufflen) \
{\
	VITLOG( 1, " (%s %d %s) Lee_Error : m_nLen=[%d],sizeof(val)=[%d],m_nBufflen=[%d] Failed",__FILE__,__LINE__,__FUNCTION__,m_nLen,sizeof(val),m_nBufflen );\
	return *this;\
}

class CVitMsgSn
{
public:
	CVitMsgSn() {Clear();}
	~CVitMsgSn() {}
	void Clear() {m_nLen=0;}
	const char* GetBuf() const {return m_cBuff;}
	int GetLen() const {return m_nLen;}
public:
	CVitMsgSn& operator << (char val) {CHECKLEN memcpy(m_cBuff+m_nLen, &val, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgSn& operator << (short val) {CHECKLEN memcpy(m_cBuff+m_nLen, &val, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgSn& operator << (int val) {CHECKLEN memcpy(m_cBuff+m_nLen, &val, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgSn& operator << (long val) {CHECKLEN memcpy(m_cBuff+m_nLen, &val, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgSn& operator << (unsigned char val) {CHECKLEN memcpy(m_cBuff+m_nLen, &val, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgSn& operator << (unsigned short val) {CHECKLEN memcpy(m_cBuff+m_nLen, &val, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgSn& operator << (unsigned int val) {CHECKLEN memcpy(m_cBuff+m_nLen, &val, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgSn& operator << (unsigned long val) {CHECKLEN memcpy(m_cBuff+m_nLen, &val, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgSn& operator << (float val) {CHECKLEN memcpy(m_cBuff+m_nLen, &val, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgSn& operator << (double val) {CHECKLEN memcpy(m_cBuff+m_nLen, &val, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgSn& operator << (bool val) {CHECKLEN memcpy(m_cBuff+m_nLen, &val, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	//CVitMsgSn& operator << (__int64 val) {CHECKLEN memcpy(m_cBuff+m_nLen, &val, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	//CVitMsgSn& operator << (unsigned __int64 val) {CHECKLEN memcpy(m_cBuff+m_nLen, &val, sizeof(val)); m_nLen+=sizeof(val); return *this;}

	//--char array--
	CVitMsgSn& operator << (const char* val)
	{
		packetlen_t len = strlen(val);
		printf("%d %s\n", len, val);
		if(m_nLen + sizeof(packetlen_t) + len > READ_MAX)
		{
			VITLOG( 1, " (%s %d %s) Lee_Error : m_nLen=[%d],len=[%d] max =[%d] Failed",__FILE__,__LINE__,__FUNCTION__,m_nLen,len,READ_MAX );
			return *this;
		}
//		printf("%s\n", m_cBuff);
//		memcpy(m_cBuff+m_nLen, &len, sizeof(len));
//		printf("%s\n", m_cBuff);
//		m_nLen += sizeof(len);
		memcpy(m_cBuff+m_nLen, val, len);
//		printf("%s\n", m_cBuff);
		m_nLen += len;
		return *this;
	}
    //--改变pos这个位置的zhi长度为sizeof(packetlen_t)--
	bool  ChangePosValue( packetlen_t value,int pos )
	{
		if( m_nLen < (size_t)pos || (int)pos + (int)value > (int)READ_MAX )
		{
			VITLOG( 1, " (%s %d %s) Lee_Error : m_nLen=[%d], pos=[%d], value=[%d] max=[%d] Failed",__FILE__,__LINE__,__FUNCTION__,m_nLen,pos,value,READ_MAX );
			return false;
		}
		memcpy( m_cBuff+pos,&value,sizeof(value) );
		return true;
	}

	bool  SnVoidValue( const void* value,packetlen_t const& size )
	{
		if(m_nLen + sizeof(packetlen_t) + size > READ_MAX)
		{
			VITLOG( 1, " (%s %d %s) Lee_Error : m_nLen=[%d], size=[%d] max=[%d] Failed",__FILE__,__LINE__,__FUNCTION__,m_nLen,size,READ_MAX );
			return false;
		}
		memcpy(m_cBuff+m_nLen, &size, sizeof(packetlen_t));
		m_nLen += sizeof(packetlen_t);
		memcpy(m_cBuff+m_nLen, value, size);
		m_nLen += size;
		return true;
	}

	bool  SnAppendSn( CVitMsgSn* sn )
	{
		if(m_nLen +  sn->GetLen() > READ_MAX)
		{
			VITLOG( 1, " (%s %d %s) Lee_Error : m_nLen=[%d] snLen=[%d] max=[%d] Failed",__FILE__,__LINE__,__FUNCTION__,m_nLen,sn->GetLen(),READ_MAX );
			return false;
		}
		memcpy( m_cBuff+m_nLen,sn->GetBuf(),sn->GetLen() );
		m_nLen += sn->GetLen();
		return true;
	}

	bool  SnAppendSn(void* value,packetlen_t size )
	{
		if(m_nLen + sizeof(packetlen_t) + size > READ_MAX)
		{
			VITLOG( 1, " (%s %d %s) Lee_Error : m_nLen=[%d], size=[%d] max=[%d] Failed",__FILE__,__LINE__,__FUNCTION__,m_nLen,size,READ_MAX );
			return false;
		}
//		memcpy(m_cBuff+m_nLen, value, size);
		memcpy(m_cBuff, value, size);
		m_nLen += size;
		return true;
	}

private:
	char     m_cBuff[READ_MAX];
	size_t   m_nLen;
};


class CVitMsgUSn
{
public:
	CVitMsgUSn() {Clear();}
	~CVitMsgUSn() {}
	void Clear() {m_nLen=0; m_nBufflen=0;}
	int	 GetLen( ){ return m_nBufflen - m_nLen; }
	bool SetBuf(const char* buf, size_t len) {if(len>READ_MAX){ VITLOG( 1, " (%s %d %s) Lee_Error : len=[%d] Failed",__FILE__,__LINE__,__FUNCTION__,len ); return false; } memcpy(m_cBuff, buf, len); m_nLen=0; m_nBufflen=len; return true;}
	void ResetLen() {m_nLen=0;}
public:
	CVitMsgUSn& operator >> (char& val){CHECKSHORT memcpy(&val, m_cBuff+m_nLen, sizeof(val));m_nLen+=sizeof(val);return *this;}
	CVitMsgUSn& operator >> (short& val) {CHECKSHORT memcpy(&val, m_cBuff+m_nLen, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgUSn& operator >> (int& val) {CHECKSHORT memcpy(&val, m_cBuff+m_nLen, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgUSn& operator >> (long& val) {CHECKSHORT memcpy(&val, m_cBuff+m_nLen, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgUSn& operator >> (unsigned char& val) {CHECKSHORT memcpy(&val, m_cBuff+m_nLen, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgUSn& operator >> (unsigned short& val) {CHECKSHORT memcpy(&val, m_cBuff+m_nLen, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgUSn& operator >> (unsigned int& val) {CHECKSHORT memcpy(&val, m_cBuff+m_nLen, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgUSn& operator >> (unsigned long& val) {CHECKSHORT memcpy(&val, m_cBuff+m_nLen, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgUSn& operator >> (float& val) {CHECKSHORT memcpy(&val, m_cBuff+m_nLen, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgUSn& operator >> (double& val) {CHECKSHORT memcpy(&val, m_cBuff+m_nLen, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	CVitMsgUSn& operator >> (bool& val) {CHECKSHORT memcpy(&val, m_cBuff+m_nLen, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	//CVitMsgUSn& operator >> (__int64& val) {CHECKSHORT memcpy(&val, m_cBuff+m_nLen, sizeof(val)); m_nLen+=sizeof(val); return *this;}
	//CVitMsgUSn& operator >> (unsigned __int64& val) {CHECKSHORT memcpy(&val, m_cBuff+m_nLen, sizeof(val)); m_nLen+=sizeof(val); return *this;}

	//--char array--
	CVitMsgUSn& operator >> (char* outval)
	{
		packetlen_t len;
		*this>>len;

		if(m_nLen + len > m_nBufflen)
		{
			VITLOG( 1, " (%s %d %s) Lee_Error : m_nLen=[%d], len=[%d], m_nBufflen=[%d]  Failed",__FILE__,__LINE__,__FUNCTION__,m_nLen,len,m_nBufflen );
			return *this;
		}
		
		memcpy( outval,m_cBuff+m_nLen,len );
		m_nLen += len;
		return *this;
	}
	bool  USnVoidValue( void* outval,packetlen_t& outsize )
	{
		*this>>outsize;

		if(m_nLen + outsize > m_nBufflen)
		{
			VITLOG( 1, " (%s %d %s) Lee_Error : m_nLen=[%d], outsize=[%d], m_nBufflen=[%d]  Failed",__FILE__,__LINE__,__FUNCTION__,m_nLen,outsize,m_nBufflen );
			return false;
		}
		memcpy( outval,m_cBuff+m_nLen,outsize );
		m_nLen += outsize;
		return true;
	}
	bool  USnGetCurEffectiveBufer( char* destbuff,packetlen_t& out_len )
	{
		if( m_nLen < 0 || m_nLen > m_nBufflen )
		{
			VITLOG( 1, " (%s %d %s) Lee_Error : m_nLen=[%d], outsize=[%d], m_nBufflen=[%d]  Failed",__FILE__,__LINE__,__FUNCTION__,m_nLen,out_len,m_nBufflen );
			return false;
		}
		memcpy( destbuff,m_cBuff + m_nLen ,m_nBufflen - m_nLen );
		out_len = m_nBufflen-m_nLen;
		return true;
	}
	bool   USnAppendBuf( void* buf,int size )
	{
		if( size < 0 || size+m_nBufflen > READ_MAX )
		{
			VITLOG( 1, " (%s %d %s) Lee_Error : size=[%d], m_nBufflen=[%d], max=[%d]  Failed",__FILE__,__LINE__,__FUNCTION__,size,m_nBufflen,READ_MAX );
			return false;
		}
		memcpy( m_cBuff+m_nBufflen,buf,size );
		m_nBufflen+=size;
		return true;
	}
    

public:
	char m_cBuff[READ_MAX];
	size_t m_nLen;
	size_t m_nBufflen;		
};

typedef void (*Connect_RecvMsg_cb)(CVitMsgUSn* usn);
typedef void (*Connect_RecvParse_cb)(int protocol,const char* buf);

#endif
