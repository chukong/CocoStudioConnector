
//
//  CHttp.h  Windows
//  CCHttpRequestExample
//
//  Created by 李光日 on 12-8-3.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef CCHttpRequestExample_CHttp_h
#define CCHttpRequestExample_CHttp_h

#include <stdio.h>
#include <iostream>
#include "curl/curl.h"
#include <map>
#include <vector>
#include <semaphore.h>

//#define USE_JS
#define PLATFORM_IOS
//#define PLATFORM_WIN32
//#define PLATFORM_ANDROID

#ifdef USE_JS
#include "SPJSbind.h"
using namespace SP_JSBind;
#endif

#ifdef PLATFORM_IOS
#define VIT_ASYNC_HTTPREQUEST_USE_NAMED_SEMAPHORE 1
#else
#define VIT_ASYNC_HTTPREQUEST_USE_NAMED_SEMAPHORE 0
#endif

#if VIT_ASYNC_HTTPREQUEST_USE_NAMED_SEMAPHORE
#define VIT_ASYNC_HTTPREQUEST_SEMAPHORE "vitHttpAsync"
#else
static sem_t s_sem;
#endif

#ifdef PLATFORM_WIN32
typedef int int32_t;
#endif


typedef void	(*http_error_cb)(const char* action,const char* info,int errortype,int retcode );
typedef void    (*http_callback_cb)( const char* action,const char* info,const char* data,bool isfile );
typedef void    (*http_getCurUpdateProgress_cb)( const char* action,const char* filename,float amount );

//****************************http_log****************************
extern void httplog( int line,const char* func,const char* fmt,... );
#define HTTP_ERROR( format,... )( httplog( 0,__LINE__,__FUNCTION__,format,__VA_ARGS__ ) )
#define HTTP_DEBUG( format,... )( httplog( 1,__LINE__,__FUNCTION__,format,__VA_ARGS__ ) )


#ifdef PLATFORM_WIN32
#pragma comment(lib, "ws2_32.lib")
//****************************http_thread****************************
typedef HANDLE    http_Thread;
#define Http_CreateThread( thread,cb,arg ){ thread = CreateThread( NULL,0,(LPTHREAD_START_ROUTINE)cb,(void*)arg,0,NULL ); }
#define Http_CloseThread( thread ){ CloseHandle( thread ); }
//****************************http_lock****************************
typedef CRITICAL_SECTION  http_Lock;
#define Http_CreateLock(lock){ InitializeCriticalSection (&lock); }         //--create lock--
#define Http_DestroyLock(lock){ DeleteCriticalSection (&lock); }            //--destroylock--
#define Http_Lock(lock){ EnterCriticalSection (&lock); }                    //--locking--
#define Http_UnLock(lock){ LeaveCriticalSection (&lock); }                  //--unlock--
#else
//****************************http_thread****************************
typedef pthread_t http_Thread;
#define Http_CreateThread(thread,cb,arg) { pthread_create( &thread,NULL,cb, arg); }
#define Http_CloseThread(thread) { pthread_exit( (void*)thread ); }
//****************************http_lock****************************
typedef pthread_mutex_t   http_Lock;
#define Http_CreateLock(lock){ pthread_mutex_init (&lock,NULL); }           //--create lock--
#define Http_DestroyLock(lock){ pthread_mutex_destroy (&lock); }            //--destroylock--
#define Http_Lock(lock){ pthread_mutex_lock (&lock); }                      //--locking--
#define Http_UnLock(lock){ pthread_mutex_unlock (&lock); }                  //--unlock--
#endif

enum enRequestType {
    eRT_None = 0,
    eRT_GetRequest,         //--GET 简单的请求服务器--
    eRT_PostRequest,        //--POST 向服务器发送参数的时候用--
    eRT_DownLoad            //--从服务器下载资源的时候用 模式还是 GET--
};

typedef struct{
    std::string action;
    std::string info;
    std::string data;
    bool        isfile;
} HttpCallbackMsg;

typedef struct{
    std::string action;
    std::string info;
    int         errorType;
    int         retcode;
} HttpCallbackError;

typedef struct {
    enRequestType               requestType;
    std::string                 url;                //--url--
    std::string                 action;             //--作用协议--
    std::string                 info;               //--客户端自定义参数 下载资源的时候这个参数传文件名字--
    CURL*                       easyHandle;         //--curl句柄对象--
    FILE*                       tempfile;           //--下载资源的时候临时文件--
    std::string                 tempfilename;       //--临时文件名字--
    std::string                 recvData;           //--从服务器回复过来的值--
    std::string                 postData;           //--post数据--
    int                         nMaxSize;           //--包的max大小--
    int                         nOldSize;           //--断点续传的时候原先文件大小--
    bool                        bActive;            //--是否已添加 curl_multi_add_handle--
    std::string                 strPostDatas;       //--post数据--
    struct timeval              start_time;         //--时间计算 开始--
} HttpRequestPacket;


typedef std::map<CURL*,HttpRequestPacket*>             URL_PACKET_MAP;
typedef std::map<CURL*,HttpRequestPacket*>::iterator   URL_PACKET_MAP_Iter;
typedef std::map<CURL*,HttpRequestPacket*>::value_type URL_PACKET_MAP_ValueType;

extern double  g_nSpeed;           // 下载速度

class CHttp
{
public:
#ifdef USE_JS
	CHttp( JSObject* pObj );
#endif
    CHttp();
    virtual ~CHttp();
    
    static CHttp *getSingletonPtr();
    static void   clearSharedHttp();
    /**
     *	@brief	初始化CHTTP
     */
    void   init();
    void   registerCallback(http_callback_cb callback, http_error_cb error,http_getCurUpdateProgress_cb progress);
    /**
     *  @brief  暂停下载
     */
    void    stopRequest();
    void    restart();
    /**
     *	@brief	添加http的头信息 这个函数 在调用请求前或者游戏的初始化里就设置一次就行 可以添加很多头可以无限addheader
     *
     *	@param 	info 	头的信息 比如：addHeader(“Agent_User : Lee”) addHeader(“Len : 1012”)
     */
    void   addHeader( std::string info );
    /**
     *	@brief	设置下载资源后保存文件的路径
     *
     *	@param 	path 	指定绝对路径
     */
    void   setFileSavePath( char* path );
    /**
     *	@brief	设置断点传序功能 默认是不是断点传序
     *
     *	@param 	b 	true 断点传序 false 普通
     */
    void   setAllowResumeForFileDownloads( bool b ){ m_bResumeForFileDownload = b; }
    
    void   setHttps( char* key, char* crt,char* ca_crt );
    /**
     *	@brief	向服务器发送请求的时候连接 超时 时间设置
     *
     *	@param 	time 	超时时间秒为单位
     */
    void   setConnectTimeOut( int time ){ m_nConnectTimeOut = time; }
    /**
     *	@brief	超时时间下载到下载完之间的时间 需要自己估算
     *
     *	@param 	time 	超时时间秒为单位
     */
    void   setTimeOut(int time){ m_nTimeOut = time; }
    /**
     *	@brief	使用addPostRequest向服务器post数据的时候使用
     */
    void   initPostData();
    /**
     *	@brief	使用addPostRequest向服务器post数据的时候使用此函数添加内容 可以调用N次添加body信息
     *
     *	@param 	key 	标签 例：name
     *	@param 	data 	内容 例：lee
     */
    void   addPostData( char* key,char* data );
    void   addPostStream( char* stream,int size );
    /**
     *	@brief	向服务器使用Get模式请求 此函数主要用于简单的Get模式请求
     *
     *	@param 	addr 	url “?”符号前面的地址部分
     *	@param 	query 	url “？”后面的参数部分
     *	@param 	action 	客户端自己用的作用协议 例：login 这个可以自己定义的
     *	@param 	info 	自定义附加参数
     *
     *	@return	请求成功返回true
     */
    bool   addGetRequest( std::string addr,std::string query,std::string action,const char* info );
    /**
     *	@brief	向服务器发送数据的时候使用post请求
     *
     *	@param 	addr 	url “?”符号前面的地址部分
     *	@param 	action 	客户端自己用的作用协议 例：login 这个可以自己定义的
     *	@param 	info 	自定义附加参数
     *
     *	@return	请求成功返回true
     */
    bool   addPostRequest(std::string addr,std::string action,const char* info);
    /**
     *	@brief	向服务器请求下载资源（文件）的时候用，也是GET请求
     *
     *	@param 	url 	服务器url
     *	@param 	action 	客户端自己用的作用协议 例：login 这个可以自己定义的
     *
     *	@return	请求成功返回 true
     */
    bool   addDownloadRequest( std::string url,std::string action,char *pszLocalFile );
    double getDownSpeed(){ return g_nSpeed; }
    
    void    clear();
    /**
     *	@brief	执行请求curl的api
     */
    void   _performRequest( );
    /**
     *	@brief	接收到从服务器的回复的时候调用
     */
    void   _recvRespon();
    
    /* pipu */
    void setLocalFileLen(int len);
    const double getDownLoadTotal() const { return m_DownLoadTotal; };
    void setDownLoadTotal(double total);
    void setDownLoadRequestCount(int count) { m_nDownLoadRequestCount = count; };
    const int getDownLoadRequestCount() const { return m_nDownLoadRequestCount; };
    /**/
public:
    std::string                         m_strShareSavePath;                         //--公共文件保存路径--
    curl_slist*                         m_pCurlheaders;                             //--头部信息--
    http_Thread                         m_requestThread;                            //--创建一个请求下载线程--
    std::map<CURL*,HttpRequestPacket*>  m_mapRequestMgr;                            //--请求的包的管理--
	
	std::vector< HttpCallbackError >   m_vecError;
	std::vector< HttpCallbackError >   m_mainVecError;
	
    std::string                         m_strPostDatas;                             //--post数据--
    bool                                m_bResumeForFileDownload;                   //--true说明断点传序--
    int                                 m_nConnectTimeOut;                          //--刚申请连接超时时间--
    int                                 m_nTimeOut;                                 //--正在下载等时候超时时间--
    
    bool                                m_isHttps;                                  //--是否HTTPS--
    std::string                         m_keyFileName;                              //--https专用 客户端key 文件路径--
    std::string                         m_crtFileName;                              //--https专用 客户端crt 文件路径--
    std::string                         m_caCrtFileName;                            //--https专用 ca crt 文件路径--
    
public:
    CURLM*                              m_pMulti_handle;
    int                                 m_nRunningHandles;
    http_Lock                           m_httpLock;                                 //--线程锁--
	http_Lock                           m_errorHttpLock;                                 //--线程锁--
    http_error_cb						m_pError_cb;								//--错误回调函数--
	http_callback_cb					m_pCallback_cb;								//--recv回调函数--
	http_getCurUpdateProgress_cb		m_pUpdateProgress_cb;						//--及时更新回调函数--
    bool                                m_bStop;
    
    /* pipu */
    curl_off_t                          local_file_len;
    double                              m_DownLoadTotal;
    int                                 m_nDownLoadRequestCount;
    /**/
public:
#ifdef USE_JS
	CREATE_JS_CLASS_ROOT(CHttp)
    
    JS_FUNCTION_0(CHttp,getSingletonPtr)
    JS_NO_RETURN_FUNCTION_1(CHttp,addHeader, char*)
	JS_NO_RETURN_FUNCTION_1(CHttp,setFileSavePath, char* )
	JS_NO_RETURN_FUNCTION_1(CHttp,setConnectTimeOut, int)
	JS_NO_RETURN_FUNCTION_1(CHttp,setTimeOut, int)
	JS_NO_RETURN_FUNCTION_1(CHttp,setAllowResumeForFileDownloads, bool)
    JS_NO_RETURN_FUNCTION_0(CHttp,initPostData)
	JS_NO_RETURN_FUNCTION_2(CHttp,addPostData, char*, char*)
    JS_NO_RETURN_FUNCTION_2(CHttp,addPostStream, char*, int)
	JS_NO_RETURN_FUNCTION_4(CHttp,addGetRequest, char*, char*, char*,char*)
	JS_NO_RETURN_FUNCTION_3(CHttp,addPostRequest, char*, char*, char*)
	JS_FUNCTION_3(CHttp,addDownloadRequest, char*, char*,char*)
	JS_NO_RETURN_FUNCTION_0(CHttp,stopRequest)
#endif
};



#endif

