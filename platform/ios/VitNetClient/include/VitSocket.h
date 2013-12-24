//
//  VitSocket.h
//  VitNetClient
//
//  Created by 李光日 on 12-9-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef VitNetClient_VitSocket_h
#define VitNetClient_VitSocket_h

#if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
#include <WinSock2.h>
#pragma comment( lib, "ws2_32" )
#endif

#include "VitPlatformConfig.h"

#define     VIT_SOCK_INVALID   -1
#define		VIT_SOCK_ERROR	   -1

#if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
#define VIT_SOCKET               SOCKET  
#define VIT_SOCKADDR_IN          SOCKADDR_IN  
#define VIT_SOCKADDR             SOCKADDR  
#define VIT_SOCKLEN_T            unsigned int
#else  
#define VIT_SOCKET               int  
#define VIT_SOCKADDR_IN          struct sockaddr_in  
#define VIT_SOCKADDR             struct sockaddr  
#define VIT_SOCKLEN_T            unsigned int
#endif 

#include "VitConnectDefine.h"

/**
 *	@brief	创建一个套接字
 *
 *	@param 	family   	一个地址描述。支持AF_INET
 *	@param 	type 	    套接口的类型描述类型，如TCP（SOCK_STREAM）和UDP (SOCK_DGRAM)
 *	@param 	protocol 	套接口所用的协议。如调用者不想指定，可用0。一般0
 *
 *	@return	socket()返回引用新套接口的描述字
 */
extern int     Socket_Socket(int family,int type,int protocol);
/**
 *	@brief	将一本地地址与一套接口捆绑。
 *
 *	@param 	fd          标识一未捆绑套接口的描述字
 *	@param 	addr        赋予套接口的地址
 *	@param 	addrlen 	套接口地址长度
 *
 *	@return	返回 0 说明没有错误
 */
extern bool_t    Socket_Bind( VIT_SOCKET fd,const VIT_SOCKADDR* addr,VIT_SOCKLEN_T addrlen );
/**
 *	@brief	建立与一个指定端的连接。
 *
 *	@param 	fd          标识一未捆绑套接口的描述字
 *	@param 	addr        欲进行连接的端口名
 *	@param 	addrlen 	端口名长度
 *
 *	@return	返回 0 说明没有错误
 */
extern bool_t    Socket_Connect( VIT_SOCKET fd,const VIT_SOCKADDR* addr,VIT_SOCKLEN_T addrlen );
/**
 *	@brief	创建一个套接口并监听申请的连接.
 *
 *	@param 	fd 	用于标识一个已捆绑未连接套接口的描述字
 *	@param 	backlog 	等待连接队列的最大长度。
 *
 *	@return	返回 0 说明没有错误
 */
extern bool_t    Socket_Listen( VIT_SOCKET fd,int backlog );
/**
 *	@brief	在一个套接口接受一个连接
 *
 *	@param 	fd          套接口描述字，该套接口在listen()后监听连接。
 *	@param 	addr        (可选)指针，指向一缓冲区，其中接收为通讯层所知的连接实体的地址。Addr参数的实际格式由套接口创建时所产生的地址族确定。
 *	@param 	addrlen 	addr 地址长度
 *
 *	@return	返回 0 说明没有错误
 */
extern int     Socket_Accept( VIT_SOCKET fd,VIT_SOCKADDR* addr,VIT_SOCKLEN_T* addrlen );
/**
 *	@brief	设置套接口的选项
 *
 *	@param 	fd          标识一个套接口的描述字
 *	@param 	level       选项定义的层次；目前仅支持SOL_SOCKET和IPPROTO_TCP层次
 *	@param 	optname 	需设置的选项
 *	@param 	optval      指针，指向存放选项值的缓冲区
 *	@param 	optlen      optval缓冲区长度
 *
 *	@return	返回 0 说明没有错误
 */
extern bool_t     Socket_Setsockopt( VIT_SOCKET fd,int level,int optname,const void* optval, VIT_SOCKLEN_T optlen );
/**
 *	@brief	获取一个套接口选项
 *
 *	@param 	fd 	一个标识套接口的描述字。
 *	@param 	level 	选项定义的层次。支持的层次仅有SOL_SOCKET和IPPROTO_TCP。
 *	@param 	optname 	需获取的套接口选项。
 *	@param 	optval 	指针，指向存放所获得选项值的缓冲区。
 *	@param 	optlen 	指针，指向optval缓冲区的长度值
 *
 *	@return	返回 0 说明没有错误
 */
extern int     Socket_Getsockopt( VIT_SOCKET fd,int level,int optname,void* optval,VIT_SOCKLEN_T* optlen );
/**
 *	@brief	向一个已连接的套接口发送数据 TCP
 *
 *	@param 	fd 	一个用于标识已连接套接口的描述字
 *	@param 	buff 	包含待发送数据的缓冲区。
 *	@param 	len 	缓冲区长度
 *	@param 	flags 	调用执行方式。
 *
 *	@return	返回所发送的长度
 */
extern int     Socket_Send( VIT_SOCKET fd,const void* buff,int len,int flags );
/**
 *	@brief	指向一指定目的地发送数据，sendto()适用于已连接的数据报或流式套接口发送数据。 UDP
 *
 *	@param 	fd      一个标识套接口的描述字。
 *	@param 	buff 	包含待发送数据的缓冲区
 *	@param 	len 	包含待发送数据的缓冲区长度
 *	@param 	flags 	调用方式标志位
 *	@param 	to      指针，指向目的套接口的地址
 *	@param 	tolen 	to所指地址的长度
 *
 *	@return	返回所发送的长度
 */
extern int     Socket_Sendto( VIT_SOCKET fd,const void* buff,int len,int flags,const VIT_SOCKADDR* to,int tolen );
/**
 *	@brief	从一个套接口接收数据 TCP
 *
 *	@param 	fd 	一个标识已连接套接口的描述字
 *	@param 	buff 	用于接收数据的缓冲区
 *	@param 	len 	缓冲区长度
 *	@param 	flags 	指定调用方式
 *
 *	@return	返回 读到的数据长度
 */
extern int     Socket_Recv( VIT_SOCKET fd,void* buff,int len,int flags );
/**
 *	@brief	接收一个数据报并保存源地址
 *
 *	@param 	fd      标识一个已连接套接口的描述字
 *	@param 	buff 	接收数据缓冲区
 *	@param 	len 	缓冲区长度
 *	@param 	flags 	调用操作方式
 *	@param 	from 	指针，指向装有源地址的缓冲区。
 *	@param 	fromlen 指针，指向装有源地址的缓冲区 长度
 *
 *	@return	返回独到的数据长度
 */
extern int     Socket_Recvfrom( VIT_SOCKET fd,void* buff,int len,int flags,VIT_SOCKADDR* from,int* fromlen );
/**
 *	@brief	关闭套接口
 *
 *	@param 	fd 	一个套接口的描述字
 *
 *	@return	返回 0 说明没有错误
 */
extern bool_t    Socket_Closesocket( VIT_SOCKET fd );
/**
 *	@brief	设置非堵塞模式
 *
 *	@param 	fd 	一个套接口的描述子
 *
 *	@return	设置成功返回 true
 */
extern bool_t    Socket_Setnonblocking( VIT_SOCKET fd );
/**
 *	@brief	获取当前堵塞模式状态
 *
 *	@param 	fd 	一个套接口的描述字
 *
 *	@return	获取成功 true
 */
extern bool_t    Socket_Getnonblocking( VIT_SOCKET fd );
/**
 *	@brief	禁止在一个套接口上进行数据的接收与发送。
 *
 *	@param 	fd 	用于标识一个套接口的描述字。
 *	@param 	how 	标志，用于描述禁止哪些操作
 *
 *	@return	返回0 说明没有错误
 */
extern bool_t    Socket_Shutdown( VIT_SOCKET fd,int how );
/**
 *	@brief	确定一个或多个套接口的状态，如需要则等待。
 *
 *	@param 	maxfdp 	是一个整数值，是指集合中所有文件描述符的范围，即所有文件描述符的最大值加1，不能错！在Windows中这个参数的值无所谓，可以设置不正确。
 *	@param 	readset 	指向一组等待可读性检查的套接口
 *	@param 	writeset 	指向一组等待可写性检查的套接口
 *	@param 	exceptset 	指向一组等待错误检查的套接口
 *	@param 	timeout 	最多等待时间，对阻塞操作则为NULL
 *
 *	@return	如果超时则返回0；否则的话，返回SOCKET_ERROR错误
 */
extern int     Socket_Select( int maxfdp,fd_set* readset,fd_set* writeset,fd_set* exceptset,struct timeval* timeout );

#endif
