

#include "VitLog.h"
//SP_DLL void (*VITLOG)( int loglevel,const char* strformat,...);
void VITLOG( int loglevel,const char* strformat,...)
{
	//char buff[1024]; memset(buff,0x00,sizeof(buff));
	char destbuff[1024]; memset(destbuff,0x00,sizeof(destbuff));
	va_list insvalist;
	
	va_start(insvalist, strformat) ;
    vsprintf(destbuff, strformat, insvalist) ;
    va_end(insvalist) ;
	
//	if( loglevel == 1 )
//		sprintf( destbuff,"Error : ( file:%s, line:%d, func:%s ) %s",file,line,function,buff );
//	else if( loglevel == 2 )
//		sprintf( destbuff,"Warning : ( file:%s, line:%d, func:%s ) %s",file,line,function,buff );
//	else if( loglevel == 3 )
//		sprintf( destbuff," ( file:%s, line:%d, func:%s ) %s",file,line,function,buff );
	printf("%s\n",destbuff);
//	cout<<destbuff<<endl;
}
