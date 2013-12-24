#include "VitConnect.h"
#include "VitLzf.h"
#include "VitLzfP.h"
#include "VitJvCryption.h"
#include "VitEncryptionManager.h"
#include "VitProtocol.h"
#include "VitConnectDefine.h"
#include "VitPcheader.h"
#include "VitSocket.h"
#include <fcntl.h>


SConnectArg* g_NetConnect = NULL;

/*
* function: sconnectarg pointer
* param   : p:connectarg pointer,protocol:protocl id
* return  : void
*/
void Connect_Trigger_MsgCallback( SConnectArg* p,int protocol );

void Connect_ParseProtocol( protocol_t* protocolID,bool_t* zip,bool_t* encrypt );

void Connect_TcpRecvMessage( SConnectArg* p );
/*
    func : new SConnectArg对象和初始化对应的几个值
*/
SConnectArg* Connect_New( int parsebuffsize )
{
    SConnectArg* p = (SConnectArg*)malloc(sizeof(SConnectArg));
    if( p == NULL )
    {
        VITLOG(1,"(%s %d %s) New SConnectArg is Failed  ",__FILE__,__LINE__,__FUNCTION__); return NULL;
    }
    
    p->pRecvBuffer = (char*)malloc(READ_MAX);
    if( p->pRecvBuffer == NULL )
    {
        VITLOG(1,"(%s %d %s) New pRecvBuffer is Failed ",__FILE__,__LINE__,__FUNCTION__); return NULL;
    }
    
    p->pRecvMsg_cb_List = List_CreateHead();
    if( p->pRecvBuffer == NULL )
    {
        VITLOG(1,"(%s %d %s) New pRecvMsg_cb_List is Failed ",__FILE__,__LINE__,__FUNCTION__); return NULL;
    }
    
    p->pParseBuffer = Buffer_New(parsebuffsize);
    if( p->pParseBuffer == NULL )
    {
        VITLOG(1,"(%s %d %s) New pParseBuffer is Failed ",__FILE__,__LINE__,__FUNCTION__); return NULL;
    }
    p->parse_cb = NULL;
	p->nKey[0] = 0x12345678;	
	p->nKey[1] = 0x734a67fc;
	p->nKey[2] = 0xe367a642;
	p->nKey[3] = 0x78432562;

    p->bDisconnect = 0;
    return p;
}

/*
    顺序 ：2
    func : 创建客户端连接
*/
bool_t Connect_CreateClient( SConnectArg* p,const char* ip,int port,bool_t encrypt,bool_t zip,enConType contype )
{
#if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
	WSADATA       WD;    //声明结构体实力
	if( 0 != WSAStartup( MAKEWORD(2,2),&WD ) )
	{
		VITLOG(1,"(%s %d %s) WSAStartup is Failed ",__FILE__,__LINE__,__FUNCTION__);
		WSACleanup();
		return false;
	}
#endif
    if( p == NULL || ip == NULL )
    {
        VITLOG(1,"(%s %d %s) SConnectArg or Ip Is Null ",__FILE__,__LINE__,__FUNCTION__); return 0;
    }
    //--new SconnectArg obj and Initialization parameters--
	p->bEncrypt = encrypt;
	p->bZip     = zip;
	p->nConType = contype;
    
	//--tcp or udp check--
	if( contype == enTcp )
        p->fd = Socket_Socket( AF_INET,SOCK_STREAM,0 );
	if(p->fd <= 0)
	{
		VITLOG(1,"(%s %d %s) socket create falied",__FILE__,__LINE__,__FUNCTION__); return 0;
	}
    
	//--set addr--
	p->ServerAddress.sin_family = AF_INET;
	p->ServerAddress.sin_port = htons(port);
	p->ServerAddress.sin_addr.s_addr = inet_addr(ip);
	
    if( contype == enTcp )
    {
        //--connect--
        if( 0 == Socket_Connect(p->fd,(struct sockaddr*)&p->ServerAddress,sizeof( p->ServerAddress ) ))
        {
           VITLOG(1,"(%s %d %s) connect falied",__FILE__,__LINE__,__FUNCTION__); return 0;
        }
    }
    if( 0 == Socket_Setnonblocking(p->fd) )
        return 0;
    
    g_NetConnect = p;
	VITLOG(3,"***************************Connect Is OK*****************************");
    
	return 1;
}

/*
    func : recv 处理
*/
void Connect_TcpRecvMessage( SConnectArg* p )  
{  
    if(p == NULL) 
    {
        VITLOG(1,"(%s %d %s) p Is Null ",__FILE__,__LINE__,__FUNCTION__); return;
    }

	int len = 0;  
	// receive data
    memset(p->pRecvBuffer, 0x00, strlen(p->pRecvBuffer));
    len = Socket_Recv(p->fd, p->pRecvBuffer, READ_MAX, 0);
	if(len > 0)  
	{
		
		VITLOG(3,"***************************Connect Is OK*****************************%s",p->pRecvBuffer);
		VITLOG(3,"recive: len %d, fd: %d this:%0x buffer: %s\n",len, p->fd, p, p->pRecvBuffer);

		Connect_Trigger_MsgCallback(p,1001);

//		p->nRecvSize = len;
//        if( !Buffer_WriteToBuff(p->pParseBuffer,p->pRecvBuffer,len))
//        {
//            VITLOG(1,"(%s %d %s) recv's msg write to buf error len=[%d] ",__FILE__,__LINE__,__FUNCTION__); return;
//        }
//        char destbuf[READ_MAX];  memset( destbuf, 0x00,sizeof( destbuf ) );
//		Connect_Trigger_MsgCallback(p,1001);
//        while( Buffer_GetMsg( p->pParseBuffer,p->pRecvBuffer,&p->nRecvSize ) )
//        {
//            p->insRecvSn.SetBuf(p->pRecvBuffer, p->nRecvSize);
//            protocol_t   nID;
//            p->insRecvSn >> nID;
//			bool_t zip,encrypt;
//			Connect_ParseProtocol( &nID,&zip,&encrypt);
//
//            if( zip )
//            {
//                packetlen_t bodylen = 0;
//                packetlen_t beforelen; //--packet zip before len--
//                p->insRecvSn >> beforelen;
//                p->insRecvSn.USnGetCurEffectiveBufer( p->pRecvBuffer,bodylen );
//                
//                lzf_decompress(p->pRecvBuffer,bodylen,destbuf,beforelen);
//				if( encrypt )
//				{
//					char outbuf[READ_MAX];  memset( outbuf, 0x00,sizeof( outbuf ) );
//					VitXXTEADecode( (const unsigned char*)destbuf,(unsigned char*)outbuf,beforelen,p->nKey );
//                    p->insRecvSn.SetBuf(outbuf, beforelen);
//				}
//				else
//				{
//                    p->insRecvSn.SetBuf(destbuf, beforelen);
//				}
//            }
//            //--usn 传的是除了头的身体--
//            Connect_Trigger_MsgCallback(p,nID);
//        }
	}
	else if(len == 0)  
	{  
        //---下线--
		Connect_CloseConnect( p );
		p->bDisconnect = 1;
		Connect_Trigger_MsgCallback(p,eGTC_TcpDisconnectAck);
        VITLOG(1,"(%s %d %s) And Server Disconnect ",__FILE__,__LINE__,__FUNCTION__); return;

    }  
	else  
	{  
        
	}  
}

/*
func : recv 处理
*/
void Connect_UdpRecvMessage( SConnectArg* p )  
{  
//    if(p == NULL) 
//    {
//        VITLOG(1,"(%s %d %s) p Is Null ",__FILE__,__LINE__,__FUNCTION__); return;
//    }
//    
//	int len = 0;  
//	// receive data  
//    sockaddr_in addr;
//    int addr_len = sizeof( struct sockaddr_in );
//    packetlen_t nRecvSize = READ_MAX;
//    
//
//    len = recvfrom(p->fd, p->pRecvBuffer, nRecvSize, 0,(sockaddr*)&addr,&addr_len );
//        
//    if(len > 0)  
//    {  
//        p->nRecvSize = len;  
//        if( !Buffer_WriteToBuff(p->pParseBuffer,p->pRecvBuffer,len))
//        {
//            VITLOG(1,"(%s %d %s) recv's msg write to buf error len=[%d] ",__FILE__,__LINE__,__FUNCTION__); return;
//        }
//        char destbuf[READ_MAX];  memset( destbuf, 0x00,sizeof( destbuf ) );
//        while( Buffer_GetMsg( p->pParseBuffer,p->pRecvBuffer,&p->nRecvSize ) )
//        {
//            SMsgUSn* usn = MsgUSn_New( p->pRecvBuffer,p->nRecvSize );
//            protocol_t   nID;
//            MsgUSn_BaseType(usn,nID);
//            //			cout<<nID<<endl;
//            bool_t zip,encrypt;
//            Connect_ParseProtocol( &nID,&zip,&encrypt);
//            
//            if( zip )
//            {
//                packetlen_t bodylen = 0;
//                packetlen_t beforelen; //--packet zip before len--
//                MsgUSn_BaseType(usn,beforelen);
//                MsgUSn_GetEffectiveBuf( usn,p->pRecvBuffer,&bodylen );
//                int  out_size = lzf_decompress(p->pRecvBuffer,bodylen,destbuf,beforelen);
//                if( encrypt )
//                {
//                    char outbuf[READ_MAX];  memset( outbuf, 0x00,sizeof( outbuf ) );
//                    VitXXTEADecode( (const unsigned char*)destbuf,(unsigned char*)outbuf,beforelen,p->nKey );
//                    MsgUSn_ResetBuf( usn,outbuf,beforelen );
//                }
//                else
//                {
//                    MsgUSn_ResetBuf( usn,destbuf,beforelen );
//                }
//            }
//            //--usn 传的是除了头的身体--
//            Connect_Trigger_MsgCallback(p,nID,usn);
//            MsgUSn_Free(usn);
//        }
//    }   
}

void Connect_RecvMessage(  ) 
{
//    if( p->nConType == enTcp )
    if( !g_NetConnect || g_NetConnect->bDisconnect )
	{
        return ;
	}
		
	Connect_TcpRecvMessage( g_NetConnect );
 //   else
  //      Connect_UdpRecvMessage(p);
}

void Connect_RecvMessage2()
{
    
}
/*
    func ：根据协议ID 触发对应的函数
*/
void Connect_Trigger_MsgCallback( SConnectArg* p,const char*buf )
{
	if(p == NULL)
	{
		VITLOG(1,"(%s %d %s) arg == NULL falied",__FILE__,__LINE__,__FUNCTION__); return;
	}
    
    if( p->parse_cb != NULL )
    {
        p->parse_cb( 1, buf);
        return;
    }
//    
//    SListNode* node = List_Find( p->pRecvMsg_cb_List,protocol );
//    if( node )
//    {
//        (node->cb)(&p->insRecvSn);
//    }
//    else
//    {
//        VITLOG(1,"(%s %d %s) protocol=%d is not found",__FILE__,__LINE__,__FUNCTION__,protocol); return;   
//    }
}
/*
    func ：根据协议ID 触发对应的函数
*/
void Connect_Trigger_MsgCallback( SConnectArg* p,int protocol )
{
	if(p == NULL)
	{
		VITLOG(1,"(%s %d %s) arg == NULL falied",__FILE__,__LINE__,__FUNCTION__); return;
	}
    
    if( p->parse_cb != NULL )
    {
        p->parse_cb( protocol,p->pParseBuffer->pBuffer);
        return;
    }
	if(protocol == eGTC_TcpDisconnectAck)
	{
		
	}
//
//    SListNode* node = List_Find( p->pRecvMsg_cb_List,protocol );
//    if( node )
//    {
//        (node->cb)(&p->insRecvSn);
//    }
//    else
//    {
//        VITLOG(1,"(%s %d %s) protocol=%d is not found",__FILE__,__LINE__,__FUNCTION__,protocol); return;   
//    }
}
/*
    func : send 处理
*/
int Connect_TcpSendMessage( SConnectArg* p,protocol_t protocol,CVitMsgSn* sn )
{
    char strSendBuf[SEND_MAX]; memset( strSendBuf,0x00,sizeof(strSendBuf) );
    p->insSendSn.Clear();

//	char mark1 = '*',mark2 = '^',mark3 = '#',mark4 = '@';
//    p->insSendSn<<mark1;
//    p->insSendSn<<mark2;
    
	//--SHeader_TCP_ZIP--	
	if( p->bZip )
	{
		packetlen_t beforelen;
		int len = 0;
		if( sn )
		{	
			//--加密--
			if( p->bEncrypt )
			{
				unsigned char outEncodeBuf[SEND_MAX]; memset( outEncodeBuf,0x00,sizeof(outEncodeBuf) );
                
				int tealen = VitXXTEAEncode((const unsigned char*)sn->GetBuf(),outEncodeBuf,sn->GetLen(),p->nKey);
				beforelen = tealen;
				len = lzf_compress( outEncodeBuf,tealen,strSendBuf,sizeof(strSendBuf) );	
//				protocol = protocol | PROTOCOL_ENTRYPT;
			}
			else //--不加密--
			{
				beforelen = sn->GetLen();
				len = lzf_compress( sn->GetBuf(),sn->GetLen(),strSendBuf,sizeof(strSendBuf) );	
			}
			
//			protocol = protocol | PROTOCOL_ZIP;
		}
//        p->insSendSn << protocol;
		if( sn )
		{
			p->insSendSn << beforelen;      //--zip before size--
            p->insSendSn.SnAppendSn(strSendBuf, len);
		}
	}
	else
	{
//		if( p->bEncrypt )
//			protocol = protocol | PROTOCOL_ENTRYPT;
		//--SHeader_TCP_ID--
//        p->insSendSn<<protocol;
		if(sn)
		{
			if(  p->bEncrypt )
			{
				unsigned char outEncodeBuf[SEND_MAX]; memset( outEncodeBuf,0x00,sizeof(outEncodeBuf) );
				int tealen = VitXXTEAEncode((const unsigned char*)sn->GetBuf(),outEncodeBuf,sn->GetLen(),p->nKey);
                p->insSendSn.SnAppendSn(outEncodeBuf,tealen );  //--packet body--
			}
			else
			{
                p->insSendSn.SnAppendSn((void*)sn->GetBuf(), sn->GetLen() );  //--packet body--
			}
		}
		else
		{
            
		}
	}
//    
//     p->insSendSn<<mark3;
//     p->insSendSn<<mark4;
	printf("send %d  buffer: %s\n", p->fd, p->insSendSn.GetBuf());
    int ret = send( p->fd,p->insSendSn.GetBuf(),p->insSendSn.GetLen(),0 );
    return ret;

}

int Connect_UdpSendMessage( SConnectArg* p,protocol_t protocol,CVitMsgSn* sn )
{
//    char strSendBuf[SEND_MAX]; memset( strSendBuf,0x00,sizeof(strSendBuf) );
//	MsgSn_Clear( p->pSendSn );
//    
//    char mark1 = '*',mark2 = '^',mark3 = '#',mark4 = '@';
//	MsgSn_BaseType(p->pSendSn,mark1); //-mark--
//	MsgSn_BaseType(p->pSendSn,mark2); //--mark--
//    
//    if( p->bZip )
//    {
//        packetlen_t beforelen;
//		int len = 0;
//		if( sn )
//		{	
//			//--加密--
//			if( p->bEncrypt )
//			{
//				unsigned char outEncodeBuf[SEND_MAX]; memset( outEncodeBuf,0x00,sizeof(outEncodeBuf) );
//				int tealen = VitXXTEAEncode((const unsigned char*)MsgSn_GetBuf(sn),outEncodeBuf,MsgSn_GetLen(sn),p->nKey);
//				beforelen = tealen;
//				len = lzf_compress( outEncodeBuf,tealen,strSendBuf,sizeof(strSendBuf) );	
//				protocol = protocol | PROTOCOL_ENTRYPT;
//			}
//			else //--不加密--
//			{
//				beforelen = MsgSn_GetLen(sn);
//				len = lzf_compress( MsgSn_GetBuf(sn),MsgSn_GetLen(sn),strSendBuf,sizeof(strSendBuf) );	
//			}
//			
//			protocol = protocol | PROTOCOL_ZIP;
//		}
//		MsgSn_BaseType(p->pSendSn,protocol);  //--protocolid--
//		if( sn )
//		{
//			MsgSn_BaseType(p->pSendSn,beforelen);      //--zip before size--
//			MsgSn_AppendVal(p->pSendSn,strSendBuf,len); //--packet body--
//		}
//    }
//    else
//    {	
//        if( p->bEncrypt )
//        protocol = protocol | PROTOCOL_ENTRYPT;
//		//--SHeader_TCP_ID--
//		MsgSn_BaseType(p->pSendSn,protocol);  //--protocolid--
//		if(sn)
//		{
//			if(  p->bEncrypt )
//			{
//				unsigned char outEncodeBuf[SEND_MAX]; memset( outEncodeBuf,0x00,sizeof(outEncodeBuf) );
//				int tealen = VitXXTEAEncode((const unsigned char*)MsgSn_GetBuf(sn),outEncodeBuf,MsgSn_GetLen(sn),p->nKey);
//				MsgSn_AppendVal( p->pSendSn,outEncodeBuf,tealen );  //--packet body--
//			}
//			else
//			{
//				MsgSn_AppendVal(p->pSendSn,MsgSn_GetBuf(sn),MsgSn_GetLen(sn));  //--packet body--
//			}
//		}
//		else
//		{
//            
//		}
//    } 
//    MsgSn_BaseType(p->pSendSn,mark3); //--mark--
//	MsgSn_BaseType(p->pSendSn,mark4); //--mark--
//    int len = sendto( p->fd,MsgSn_GetBuf(p->pSendSn),MsgSn_GetLen(p->pSendSn),0,(sockaddr*)&p->ServerAddress,sizeof(p->ServerAddress) );
//    return len;
    return 1;
}
int Connect_SendMessage( SConnectArg* p,protocol_t protocol,CVitMsgSn* sn )
{
	if(p == NULL)
	{
		return 0 ;
	}
	if( p->nConType == enTcp )
	{
        return Connect_TcpSendMessage(p,protocol,sn);
	}
    
	return Connect_UdpSendMessage(p,protocol,sn);
}
/*
 func ：根据协议ID 消息回调管理添加对应的函数
*/
bool_t Connect_Add_MsgCallback( SConnectArg* arg,unsigned short protocol,Connect_RecvMsg_cb cb )
{
    if( protocol <= 0 )
    {        
        VITLOG(1,"(%s %d %s) protocol=[%d] is falied",__FILE__,__LINE__,__FUNCTION__,protocol); return 0;
    }
	if(arg == NULL)
	{
		VITLOG(1,"(%s %d %s) arg == NULL falied",__FILE__,__LINE__,__FUNCTION__); return 0;
	}
    
	List_Add( arg->pRecvMsg_cb_List,(protocol_t)protocol,cb );
	return 1;
}
bool_t Connect_ParseCallback( SConnectArg* arg,Connect_RecvParse_cb cb )
{
    if(arg == NULL)
	{
		VITLOG(1,"(%s %d %s) arg == NULL falied",__FILE__,__LINE__,__FUNCTION__); return 0;
	}

    arg->parse_cb = cb;
    return 1;
}
/*
    func : 与服务器断开连接
*/
void Connect_CloseConnect( SConnectArg* p )
{
	if(p)
    {
        Socket_Closesocket(p->fd);
    }
}
/*
    func : free掉p对象和对应的所有内存块
*/
void Connect_Free( SConnectArg* p )
{
    if(p)
    {
        free( p->pRecvBuffer );
		List_Free(p->pRecvMsg_cb_List);
        Buffer_Free(p->pParseBuffer);
    }
}

void Connect_ParseProtocol( protocol_t* protocolID,bool_t* zip,bool_t* encrypt )
{
	int type = 0;
	type = ( *protocolID & PROTOCOL_PROCESS );
	type = type >> 14;
    
    int protocol = *protocolID;
    protocol = protocol<<2;
	protocol = protocol>>2;
    *protocolID = protocol;
	//1100000000000   0 01=1  10=2  11=3
	if( type == 1 )
	{
		*encrypt = 0;
		*zip = 1;
	}
	else if( type == 2 )
	{
		*encrypt = 1;
		*zip = 0;
	}
	else if( type == 3 )
	{
		*zip = 1;
		*encrypt = 1;
	}
	else
	{
		*zip = 0;
		*encrypt = 0;	
	}

}
void Connect_SetKey( SConnectArg* p,int key[4] )
{
	for( int i = 0; i < 4; i++ )
	p->nKey[i] = key[i];
}