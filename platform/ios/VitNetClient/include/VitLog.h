#ifndef _VITLOG_H_LEE_
#define _VITLOG_H_LEE_

#include "VitPcheader.h"
#include "VitConnectDefine.h"
#if (VIT_TARGET_PLATFORM == VIT_PLATFORM_IOS || VIT_TARGET_PLATFORM == VIT_PLATFORM_ANDROID)
#define VIT_DLL 
#endif

#if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)

#if defined(_USRDLL)
#define VIT_DLL     __declspec(dllexport)
#else         /* use a DLL library */
#define VIT_DLL     __declspec(dllimport)
#endif
#endif

//extern SP_DLL void (*VITLOG)( int loglevel,const char* strformat,...);
void VIT_DLL VITLOG( int loglevel,const char* strformat,...);
//inline void VITLOG( int loglevel,const char* strformat,...)
//{
//	//char buff[1024]; memset(buff,0x00,sizeof(buff));
//	char destbuff[1024]; memset(destbuff,0x00,sizeof(destbuff));
//	va_list insvalist;
//
//	va_start(insvalist, strformat) ;
//	vsprintf(destbuff, strformat, insvalist) ;
//	va_end(insvalist) ;
//
//	//	if( loglevel == 1 )
//	//		sprintf( destbuff,"Error : ( file:%s, line:%d, func:%s ) %s",file,line,function,buff );
//	//	else if( loglevel == 2 )
//	//		sprintf( destbuff,"Warning : ( file:%s, line:%d, func:%s ) %s",file,line,function,buff );
//	//	else if( loglevel == 3 )
//	//		sprintf( destbuff," ( file:%s, line:%d, func:%s ) %s",file,line,function,buff );
//	printf("%s\n",destbuff);
//	//	cout<<destbuff<<endl;
//}

//#define VITLOG( loglevel,pStr,... )(Log_Log( __FILE__,__LINE__,__FUNCTION__,loglevel,pStr,__VA_ARGS__ ));


#endif