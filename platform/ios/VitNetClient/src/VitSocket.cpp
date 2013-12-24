//
//  VitSocket.cpp
//  VitNetClient
//
//  Created by 李光日 on 12-9-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "VitPcheader.h"
#include "VitSocket.h"
#include <fcntl.h>




int     Socket_Socket(int family,int type,int protocol)
{
	VIT_SOCKET s = socket(family,type,protocol);
	return s;
}

bool_t    Socket_Bind( VIT_SOCKET fd,const VIT_SOCKADDR* addr,VIT_SOCKLEN_T addrlen )
{
    if( bind ( fd , addr , addrlen ) == VIT_SOCK_ERROR )
    {
        return 0;
    }
    return 1;
}

bool_t    Socket_Connect( VIT_SOCKET fd,const VIT_SOCKADDR* addr,VIT_SOCKLEN_T addrlen )
{
    if ( connect(fd,addr,addrlen) == VIT_SOCK_ERROR ) 
    {
        return 0;
    }
    return 1;
}

bool_t    Socket_Listen( VIT_SOCKET fd,int backlog )
{
    if ( listen( fd , backlog ) == VIT_SOCK_ERROR )
    {
        return 0;
    }
    return 1;
}

int     Socket_Accept( VIT_SOCKET fd, VIT_SOCKADDR* addr,VIT_SOCKLEN_T* addrlen )
{
   #if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32 || VIT_TARGET_PLATFORM == VIT_PLATFORM_ANDROID)
        VIT_SOCKET client = accept( fd , addr , (int*)addrlen );
    #else
        VIT_SOCKET client = accept(fd, addr, addrlen);
    #endif
        return client;
}

bool_t     Socket_Setsockopt( VIT_SOCKET fd,int level,int optname,const void* optval, VIT_SOCKLEN_T optlen )
{
    #if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
        if ( setsockopt( fd , level , optname , (CHAR*)optval , optlen ) == VIT_SOCK_ERROR ) 
            return 0;
        return 1;
    #else
        if ( setsockopt( fd , level , optname , optval , optlen ) == VIT_SOCK_ERROR ) 
            return 0;
        return 1;
    #endif
}

int     Socket_Getsockopt( VIT_SOCKET fd,int level,int optname,void* optval,VIT_SOCKLEN_T* optlen )
{
    #if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32 || VIT_TARGET_PLATFORM == VIT_PLATFORM_ANDROID)
        if ( getsockopt( fd , level , optname , (char*)optval , (int*)optlen ) == VIT_SOCK_ERROR )
            return 0;
        return 1;
    #else
        if ( getsockopt( fd , level , optname , optval , optlen ) == VIT_SOCK_ERROR )
            return 0;
        return 1;
    #endif
}

int     Socket_Send( VIT_SOCKET fd,const void* buff,int len,int flags )
{
    #ifdef WIN32
        int nSent = send(fd,(CHAR*)buff,len,flags);
    #else
        int nSent = send(fd,buff,len,flags);
    #endif
        return nSent;
}

int     Socket_Sendto( VIT_SOCKET fd,const void* buff,int len,int flags,const VIT_SOCKADDR* to,int tolen )
{
    #if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
        int nSent = sendto(fd,(CHAR*)buff,len,flags,to,tolen);
    #else
        int nSent = sendto(fd,buff,len,flags,to,tolen);
    #endif
        return nSent;
}

int     Socket_Recv( VIT_SOCKET fd,void* buff,int len,int flags )
{
    #if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
        int nrecv = recv(fd,(CHAR*)buff,len,flags);
    #else
        int nrecv = recv(fd,buff,len,flags);
    #endif
        return nrecv;
}

int     Socket_Recvfrom( VIT_SOCKET fd,void* buff,int len,int flags,VIT_SOCKADDR* from,VIT_SOCKLEN_T* fromlen )
{
    #if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32 || VIT_TARGET_PLATFORM == VIT_PLATFORM_ANDROID)
        int nReceived = recvfrom(fd,(char*)buff,len,flags,from,(int*)fromlen);
    #else
        int nReceived = recvfrom(fd,buff,(size_t)len,flags,from,fromlen);
    #endif
        return nReceived;
}

bool_t    Socket_Closesocket( VIT_SOCKET fd )
{
    #if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
        if ( closesocket(fd) == SOCKET_ERROR ) 
            return 0;
    #else
        close(fd);
    #endif    
        return 1;
}

bool_t    Socket_Setnonblocking( VIT_SOCKET fd )
{
   #if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
        {
            u_long nonblocking = 1;
            if (ioctlsocket(fd, FIONBIO, &nonblocking) == SOCKET_ERROR) {
                return 0;
            }
        }
    #else
        {
            int flags;
            if ((flags = fcntl(fd, F_GETFL, NULL)) < 0) {
                return 0;
            }
            if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
                return 0;
            }
        }
    #endif
    return 1;
}
bool_t    Socket_Getnonblocking( VIT_SOCKET fd )
{
    return 1;
}
bool_t    Socket_Shutdown( VIT_SOCKET fd,int how )
{
    if ( shutdown(fd,how) < 0 ) 
        return 0;
    return 1;
}
int     Socket_Select( int maxfdp,fd_set* readset,fd_set* writeset,fd_set* exceptset,struct timeval* timeout )
{
    int result = select( maxfdp , readset , writeset , exceptset , timeout );
    return result;
}
