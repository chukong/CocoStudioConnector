


#include "CHttp.h"
#include <queue>
#include <stdlib.h>

#include <signal.h>
#include <unistd.h>
//#include <cocos2d.h>






//-------------------------------------- callback --------------------------------------
struct timeval	g_start;		// 时间计算 开始
struct timeval	g_end;			// 时间计算 结束
int	   g_nRunTime = 0;			// 运行时间
static double  g_nDownNum = 0;  // 下载量
double g_nSpeed = 0;            // 下载速度
static sem_t* s_pSem = NULL;    // 信号量


void* requestThread(void *data);
size_t writeFile(void *ptr, size_t size, size_t nmemb, void *stream);
size_t writeHeader( void* ptr,size_t size,size_t nitems,void* strea );
size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream);
size_t progress_callback( void*clientp,double dltotal,double dlnow,double ultotal,double ulnow );

static CHttp *g_singleton_http = NULL;


//--http_log--
void httplog( int flag,int line,const char* func,const char* fmt,... )
{
    va_list argptr;
    char msg[512];memset(msg, 0x00, sizeof(msg));
    va_start(argptr, fmt);
    vsprintf( msg,fmt,argptr );
    va_end(argptr);
    char buff[512]; memset( buff,0x00,sizeof(buff) );
    if(flag)
    {
        printf(" [Http_debug : line=%d func=%s] => %s\n",line,func,msg);
        //cocos2d::CCLog(" [Http_debug : line=%d func=%s] => %s\n",line,func,msg);
    }
    else
    {
        printf(" [Http_error : line=%d func=%s] => %s\n",line,func,msg);
        //cocos2d::CCLog(" [Http_error : line=%d func=%s] => %s\n",line,func,msg);
    }
};

void* requestThread(void *data)
{
    CHttp* http = (CHttp*)data;
    if( !http  ){
        HTTP_ERROR( "%s","chttp 线程的参数为空\0"); return NULL;
    }
    
    while(true)
    {
        int semWaitRet = sem_wait( s_pSem );
        if( semWaitRet < 0 ){
            HTTP_ERROR( "%s","HttpRequest async thread semaphore error\0");
            break;
        }

        if( http->m_bStop ){
            http->clear();
            break;
        }
        curl_multi_perform(http->m_pMulti_handle, &http->m_nRunningHandles);
        http->_performRequest();
        http->_recvRespon();
    }
    
    if( s_pSem != NULL )
    {
        #if VIT_ASYNC_HTTPREQUEST_USE_NAMED_SEMAPHORE
                sem_unlink(VIT_ASYNC_HTTPREQUEST_SEMAPHORE);
                sem_close( s_pSem );
        #else
                sem_destroy( s_pSem );
        #endif
    }
    
    pthread_exit(NULL);
    
    return NULL;
}

size_t writeFile(void *ptr, size_t size, size_t nmemb, void *stream)
{
    HttpRequestPacket* req = (HttpRequestPacket*)stream;
    if( NULL == req )
    {
        HTTP_ERROR( "%s","req 是空的\0"); return -1;
    }
    size_t written = fwrite(ptr, size, nmemb, req->tempfile );
    return written;
}

size_t writeHeader( void* ptr,size_t size,size_t nitems,void* strea )
{
    HttpRequestPacket* req = (HttpRequestPacket*)strea;
    if( req == NULL )
    {
        HTTP_ERROR( "%s","设置WriteHead参数出错" );
        return nitems * size;
    }
    //--Content-Range: == 15 所以大于15--
    if( nitems > 15 )
    {
        const char* strRange = "Content-Range:";
        char buff[32]; memset(buff,0x00,sizeof(buff));
        char* temp = ((char*)ptr);
        int nStartCopyPos = 0;
        bool bConTentRange = true;
        for( int i = 0; i < 12; ++i )
        {
            if( temp[i] != strRange[i] )
            {
                bConTentRange = false;
                break;
            }
        }
        if( bConTentRange )
        {
            for( int i = 20; i < nitems; i++ )
            {
                if( temp[i] == '/' )
                {
                    nStartCopyPos = i; break;
                }
            }
            
            if( nStartCopyPos != 0 )
            {
                int max = (int)nitems;
                if( max - nStartCopyPos > 32 )
                {
                    HTTP_ERROR( "%s","显示的进度信息错误 %d",max );
                    return nitems * size;
                }
                strcpy(buff, temp + nStartCopyPos + 1);
                int maxsize = atoi( buff );
                req->nMaxSize = maxsize;
            }
        }
    }
    // printf( "=======%d  %s\n",nitems,(char*)ptr );
    return nitems * size ;
}

size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    HttpRequestPacket* req = (HttpRequestPacket*)stream;
    if( req == NULL ) return -1;
    size_t sizes = size * nmemb;
    req->recvData.append((char *)ptr, sizes);
    return sizes;
}

size_t progress_callback( void*clientp,double dltotal,double dlnow,double ultotal,double ulnow )
{
    gettimeofday(&g_end, NULL) ;
    g_nRunTime = (g_end.tv_sec - g_start.tv_sec) * 1000000 + (g_end.tv_usec - g_start.tv_usec);
    if( g_nRunTime >= 100000 )
    {
        g_nSpeed = dlnow - g_nDownNum;
        gettimeofday(&g_start, NULL) ;
        g_nDownNum = dlnow;
    }
    
    HttpRequestPacket* req = (HttpRequestPacket*)clientp;
    if( NULL == req )
    {
        HTTP_ERROR( "%s","req 是空的\0"); return -1;
    }
    curl_off_t amount;
    //--这里-1说明 没有可续传的 而是重新开始下载--
    if( req->nMaxSize == -1 )
    {
        amount = float( dlnow / dltotal ) * 100;
        
        /* pipu */
        CHttp* http = CHttp::getSingletonPtr();
        http->setDownLoadTotal(dltotal);
        /**/
    }
    else
    {
        amount = float( (req->nOldSize + dlnow) / req->nMaxSize ) * 100;
        
        /* pipu */
        CHttp* http = CHttp::getSingletonPtr();
        http->setDownLoadTotal(req->nMaxSize);
        /**/
    }
//    printf("amount %d %d %f \n", amount, req->nOldSize, dlnow);
    if(amount <= 0)
    {
        amount = 0;
        return 0;
    }
    CHttp* http = CHttp::getSingletonPtr();
    http->m_pUpdateProgress_cb( req->action.c_str(),req->info.c_str(), req->nOldSize + dlnow );
    
    return  0;;
}



#ifdef USE_JS
INIT_JS_CLASS_ROOT(CHttp)

JS_PROPERTY_BEGIN(CHttp)
JS_PROPERTY_END

JS_PROPERTY_GET_BEGIN(CHttp)
JS_PROPERTY_GET_END

JS_PROPERTY_SET_BEGIN(CHttp)
JS_PROPERTY_SET_END


JS_FUNCTION_BEGIN(CHttp)
JS_FUNCTION_BIND( setFileSavePath, 0 )
JS_FUNCTION_BIND(setConnectTimeOut, 0)
JS_FUNCTION_BIND(setTimeOut, 0)
JS_FUNCTION_BIND(addHeader, 0)
JS_FUNCTION_BIND(setAllowResumeForFileDownloads, 0)
JS_FUNCTION_BIND(addPostData, 0)
JS_FUNCTION_BIND(addPostStream, 0)
JS_FUNCTION_BIND(addGetRequest, 0)
JS_FUNCTION_BIND(initPostData, 0)
JS_FUNCTION_BIND(addPostRequest, 0)
JS_FUNCTION_BIND(addDownloadRequest, 0)
JS_FUNCTION_BIND(stopRequest, 0)
JS_FUNCTION_BIND(getSingletonPtr, 0)
JS_FUNCTION_END
#endif

//------------------------------------------------Http c++-------------------------------------------------
#ifdef USE_JS
CHttp::CHttp( JSObject* pObj )
{
	m_bResumeForFileDownload = false;
	m_pMulti_handle = NULL;
	m_nRunningHandles = 0;
	m_pCurlheaders = NULL;
    
    
	m_pError_cb = NULL;
	m_pCallback_cb = NULL;
	m_pUpdateProgress_cb = NULL;
    
	m_nConnectTimeOut = 10;
	m_nTimeOut = 3600;
    
	Http_CreateLock( m_httpLock);
		Http_CreateLock( m_errorHttpLock);
}
#endif

CHttp::CHttp()
{
    m_bResumeForFileDownload = false;
    m_pMulti_handle = NULL;
    m_nRunningHandles = 0;
    m_pCurlheaders = NULL;
    
    
   	m_pError_cb = NULL;
    m_pCallback_cb = NULL;
    m_pUpdateProgress_cb = NULL;
    
    m_nConnectTimeOut = 10;
    m_nTimeOut = 36000;
    
    m_bStop = false;
    
    /* pipu */
    m_nDownLoadRequestCount = 0;
    /**/
    
    Http_CreateLock( m_httpLock);
			Http_CreateLock( m_errorHttpLock);
#ifdef USE_JS
	CREATE_JS_OBJECT( CHttp )
#endif
}
CHttp::~CHttp()
{
    curl_global_cleanup();
    curl_multi_cleanup(m_pMulti_handle);
    curl_slist_free_all(m_pCurlheaders);  //--释放头的信息--
    Http_DestroyLock( m_httpLock);
	    Http_DestroyLock( m_errorHttpLock);
    
    URL_PACKET_MAP_Iter itr = m_mapRequestMgr.begin();
    for( ; itr != m_mapRequestMgr.end();itr++ )
    {
        HttpRequestPacket* p = itr->second;
        if( p )
        {
            curl_multi_remove_handle(m_pMulti_handle,p->easyHandle);
            curl_easy_cleanup( p->easyHandle );
            delete p;
            p = NULL;
        }
    }
    Http_CloseThread( (void*)(m_requestThread) );
    
    if( s_pSem != NULL )
        sem_post( s_pSem );
}

void CHttp::registerCallback(http_callback_cb callback, http_error_cb error,http_getCurUpdateProgress_cb progress)
{
    if( !error || !callback || !progress )
    {
        HTTP_ERROR( "%s","注册 所有的回调函数 == NULL \0"); return;
    }
	m_pError_cb = error;
	m_pCallback_cb = callback;
	m_pUpdateProgress_cb = progress;
}
void CHttp::clear()
{
    Http_Lock(m_httpLock);
    URL_PACKET_MAP_Iter itr = m_mapRequestMgr.begin();
    for( ; itr != m_mapRequestMgr.end();itr++ )
    {
        HttpRequestPacket* p = itr->second;
        if( p )
        {
            curl_multi_remove_handle(m_pMulti_handle,p->easyHandle);
            curl_easy_cleanup( p->easyHandle );
            delete p;
            p = NULL;
        }
    }
    curl_multi_cleanup(m_pMulti_handle);
    m_mapRequestMgr.clear();
    Http_UnLock(m_httpLock);
}

CHttp* CHttp::getSingletonPtr()
{
    if( g_singleton_http == NULL )
    {
        g_singleton_http = new CHttp();
        g_singleton_http->init();
    }
    return g_singleton_http;
}
void CHttp::clearSharedHttp()
{
    if( g_singleton_http )
    {
        delete g_singleton_http;
        g_singleton_http = NULL;
    }
}

void CHttp::init( )
{
#if VIT_ASYNC_HTTPREQUEST_USE_NAMED_SEMAPHORE
    s_pSem = sem_open( VIT_ASYNC_HTTPREQUEST_SEMAPHORE,O_CREAT,00644,0 );
    if( s_pSem == SEM_FAILED ){
        HTTP_ERROR( "%s","Open HttpRequest Semaphore failed");
        s_pSem = NULL;
        return;
    }
#else
    int semRet = sem_init( &s_sem,0,0 );
    if( semRet < 0 ){
        HTTP_ERROR( "%s","Init HttpRequest Semaphore failed");
        return;
    }
    s_pSem = &s_sem;
    HTTP_DEBUG( "%s%d","seminit is",semRet);
#endif
    
    Http_CreateThread(m_requestThread, requestThread, this);
    curl_global_init(CURL_GLOBAL_ALL);
    m_pMulti_handle = curl_multi_init();
}

void CHttp::_performRequest(  )
{
    while (m_nRunningHandles) {


		fd_set fd_read;
		fd_set fd_write;
		fd_set fd_execpt;
		FD_ZERO(&fd_read);
		FD_ZERO(&fd_write);
		FD_ZERO(&fd_execpt);
        
        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000;
        
//        curl_multi_timeout( m_pMulti_handle,&curl_timeo );
//        if( curl_timeo >= 0 )
//        {
//        	tv.tv_sec = curl_timeo / 1000;
//        	if( tv.tv_sec > 1 )
//        	{
//        		tv.tv_sec = 1;
//        	}else
//        	{
//                tv.tv_usec = (curl_timeo % 1000) * 1000;
//        	}
//        }
        
        int max_fd;
        curl_multi_fdset(m_pMulti_handle, &fd_read, &fd_write, &fd_execpt, &max_fd);
        
		int req = select(max_fd + 1, &fd_read, &fd_write, &fd_execpt, &tv);
        //--0 select error  -1 timeout
        if( req == 0)
        {
			HTTP_ERROR( "%s","http select requst timeout\0");            
        }
        else if( req == -1 )
        {
            HTTP_ERROR( "%s","http request error\0");
            break;
        }
        /* pipu */
        else
        {
            m_nDownLoadRequestCount = 0;
        }
        /**/
        
        if( m_bStop )
        {
            clear();
            break;
        }
        curl_multi_perform(m_pMulti_handle, &m_nRunningHandles);
        _recvRespon();
    }
}
void CHttp::_recvRespon()
{
    CURLMsg *msg;
    int msgs_left ;
    while ((msg = curl_multi_info_read(m_pMulti_handle, &msgs_left)))
    {
        if (msg->msg == CURLMSG_DONE)
        {
            if( m_mapRequestMgr.count(msg->easy_handle) > 0 )
            {
                HttpRequestPacket* p = m_mapRequestMgr[ msg->easy_handle ];
                if( p == NULL )
                {
                    Http_Lock(m_httpLock);
                    m_mapRequestMgr.erase(msg->easy_handle);
                    Http_UnLock(m_httpLock);
                    //curl_multi_remove_handle(m_pMulti_handle,msg->easy_handle);
                    //curl_easy_cleanup(msg->easy_handle);
                    continue;
                }
                
                long retcode = 0;
                curl_easy_getinfo(msg->easy_handle, CURLINFO_RESPONSE_CODE , &retcode);
                bool tmp_res = true;
                if( msg->data.result != 0 || retcode != 200 ){
                    if( retcode != 206 ){
					
//													HttpCallbackError err;
//								err.action = p->action;
//								err.info =p->info;
//									    Http_Lock(m_errorHttpLock);
//								m_vecError.push_back( err );
//									    Http_UnLock(m_errorHttpLock);
										
                        HTTP_ERROR("requst is error recode=[%d] result=[%d]", retcode,msg->data.result);
                        m_pError_cb( p->action.c_str(),p->info.c_str(),msg->data.result,retcode );
                        if (0 == msg->data.result && retcode >= 400)
                            remove(p->tempfilename.c_str());
                        tmp_res = false;
                    }
                }
                
                if( tmp_res ){
                    switch (p->requestType)
                    {
                        case eRT_GetRequest:
                        {
                            struct timeval	end;
                            ::gettimeofday(&end, NULL) ;
                            int rt = g_nRunTime = (end.tv_sec - p->start_time.tv_sec) * 1000000 + (end.tv_usec - p->start_time.tv_usec);
                            HTTP_DEBUG( "vit=== request action=[%s] info=[%s] time=[%d]\n",p->action.c_str(),p->info.c_str(),rt);
                            m_pCallback_cb(p->action.c_str(),p->info.c_str(),p->recvData.c_str(),false);
                            break;
                        }
                        case eRT_PostRequest:
                        {
                            struct timeval	end;
                            ::gettimeofday(&end, NULL) ;
                            int rt = g_nRunTime = (end.tv_sec - p->start_time.tv_sec) * 1000000 + (end.tv_usec - p->start_time.tv_usec);
                            HTTP_DEBUG( " data=[%s] time=[%d]\n",p->recvData.c_str(),rt);
                            m_pCallback_cb(p->action.c_str(),p->info.c_str(),p->recvData.c_str(),false);
                            break;
                        }
                        case eRT_DownLoad:
                        {
                            int local_file_len = ftell(p->tempfile);
                            //--刚下载完的文件句柄关闭--
                            fclose(p->tempfile);
//                            if( local_file_len < p->nMaxSize )
//                            {
//								//m_pError_cb( p->action.c_str(),p->info.c_str(),msg->data.result,retcode );
//								HttpCallbackError err;
//								err.action = p->action;
//								err.info =p->info;
//									    Http_Lock(m_errorHttpLock);
//								m_vecError.push_back( err );
//									    Http_UnLock(m_errorHttpLock);
//								
////								HttpCallbackError err;
////								err.action = p->action;
////								err.info =p->info;
////									    Http_Lock(m_errorHttpLock);
////								m_vecError.push_back( err );
////									    Http_UnLock(m_errorHttpLock);
////                                m_pError_cb( p->action.c_str(), p->info.c_str(), 2000, retcode );
//                                HTTP_ERROR( "%s","没有下载完毕 断点续传");
//                                break;
//                            }
                            /* pipu */
                            if( local_file_len < p->nMaxSize )
                            {
                                printf("没有下载完毕 断点续传\n");
                                m_pError_cb( p->action.c_str(), p->info.c_str(), 2000, retcode );
                                HTTP_ERROR( "%s","没有下载完毕 断点续传");
                                break;
                            }
                            /**/
                            //--改变文件名字 xx.temp 改成 xx--
                            std::string filename;
                            filename = p->tempfilename;
                            int pos = filename.size() - 5;
                            filename = filename.substr(0,pos);
                            rename(p->tempfilename.c_str(),filename.c_str() );
                            m_pCallback_cb(p->action.c_str(),p->info.c_str(),filename.c_str(),true);
                            
                            
                            
                            g_nSpeed = 0;
                            break;
                        }
                        default:
                            break;
                    }
                }//tmp_res
                
                delete p;          p = NULL;
                Http_Lock(m_httpLock);
                m_mapRequestMgr.erase(msg->easy_handle);
                Http_UnLock(m_httpLock);
                curl_multi_remove_handle(m_pMulti_handle,msg->easy_handle);
                curl_easy_cleanup(msg->easy_handle);

                
            }//m_mapRequestMgr
        }//if msg->msg
    }//while
}

void CHttp::addHeader( std::string info )
{
    //设置header信息
    m_pCurlheaders = curl_slist_append(m_pCurlheaders, info.c_str());             //--设置头的信息--
}

void CHttp::setFileSavePath( char* path )
{
    if( NULL != path )
        m_strShareSavePath = path;
}

void CHttp::setHttps( char* key, char* crt,char* ca_crt )
{
    if( !key || !crt || !ca_crt ){
        m_isHttps = false;
        HTTP_ERROR( "%s","https 对应参数 空");
        return;
    }
    
    m_isHttps = true;
    m_keyFileName = key;
    m_crtFileName = crt;
    m_caCrtFileName = ca_crt;
}

void CHttp::initPostData()
{
    m_strPostDatas.clear();
}

void CHttp::addPostData( char* key,char* data )
{
    if( m_strPostDatas.size() != 0 )
        m_strPostDatas += "&";
    m_strPostDatas += key;
    m_strPostDatas += "=";
    m_strPostDatas += data;
}
void CHttp::addPostStream( char* stream,int size )
{
    std::string strData(stream, size);
    m_strPostDatas = strData;
}
void CHttp::stopRequest()
{
    m_bStop = true;
}

bool CHttp::addGetRequest( std::string addr,std::string query,std::string action,const char* info )
{
    restart();
    HttpRequestPacket* req = new HttpRequestPacket();
    if( NULL == req ){
        HTTP_ERROR( "%s","HttpRequestPacket new 内存空间不够"); return false;
    }
    m_nTimeOut = 600;
    m_nConnectTimeOut = 600;
    req->requestType = eRT_GetRequest;
    req->url += addr;req->url += "?";req->url += query;
    req->action = action;
    req->info = info;
    req->bActive = false;
    req->easyHandle = curl_easy_init();
    
    CURLcode code;
    code = curl_easy_setopt(req->easyHandle, CURLOPT_HTTPHEADER,m_pCurlheaders);            //--设置头的信息--
    if (code != CURLE_OK){
		return false;
    }
//    code = curl_easy_setopt(req->easyHandle, CURLOPT_VERBOSE,1 );                           //--debug output--
    // code = curl_easy_setopt(req->easyHandle, CURLOPT_HEADER,1 );                          //--headers in the output--
    code = curl_easy_setopt(req->easyHandle, CURLOPT_NOSIGNAL, 1L);                         //--多线程超时崩溃 需要写这个--
    if (code != CURLE_OK){
		return false;
    }
    //设置url
    code = curl_easy_setopt(req->easyHandle, CURLOPT_URL, req->url.c_str() );               //--设置url--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d\n","curl_easy_setopt 设置 CURLOPT_URL 属性错误\0",code);
		return false;
    }
    curl_easy_setopt(req->easyHandle, CURLOPT_CONNECTTIMEOUT, m_nConnectTimeOut );          //--设置连接超时--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_CONNECTTIMEOUT 属性错误\0",code);
		return false;
    }
    code=curl_easy_setopt(req->easyHandle, CURLOPT_TIMEOUT, m_nTimeOut );                   //--正在下载的时候超时--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_TIMEOUT 属性错误\0",code);
		return false;
    }
    //设置回调函数
    code = curl_easy_setopt(req->easyHandle, CURLOPT_HEADERFUNCTION,writeHeader  );          //--设置头回调函数--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_HEADERFUNCTION 属性错误\0",code);
		return false;
    }
    code = curl_easy_setopt(req->easyHandle, CURLOPT_HEADERDATA,req );                      //--设置头回调函数参数--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_HEADERDATA 属性错误\0",code);
		return false;
    }
    
    code = curl_easy_setopt(req->easyHandle, CURLOPT_WRITEFUNCTION, writeData );            //--设置下载回调函数--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_WRITEFUNCTION 属性错误\0",code);
		return false;
    }
    code = curl_easy_setopt(req->easyHandle, CURLOPT_WRITEDATA,req  );                      //--设置给下载回调函数的最后一个参数--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_WRITEDATA 属性错误\0",code);
		return false;
    }
    if( m_isHttps ){
        code = curl_easy_setopt(req->easyHandle, CURLOPT_SSL_VERIFYPEER,0L);                    //--https--
        if (code != CURLE_OK){
            HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_SSL_VERIFYPEER 属性错误\0",code);
            return false;
        }
        code = curl_easy_setopt(req->easyHandle, CURLOPT_SSLKEY,m_keyFileName.c_str());                 //--https key--
        if (code != CURLE_OK){
            HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_SSLKEY 属性错误\0",code);
            return false;
        }
        code = curl_easy_setopt(req->easyHandle, CURLOPT_SSLCERT,m_crtFileName.c_str());                //--https crt--
        if (code != CURLE_OK){
            HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_SSLCERT 属性错误\0",code);
            return false;
        }
        code = curl_easy_setopt(req->easyHandle, CURLOPT_CAINFO,m_caCrtFileName.c_str());               //--https ca crt--
        if (code != CURLE_OK){
            HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_CAINFO 属性错误\0",code);
            return false;
        }
    }
    ::gettimeofday(&req->start_time, NULL) ;
    curl_multi_add_handle(m_pMulti_handle, req->easyHandle);
    Http_Lock(m_httpLock);
    m_mapRequestMgr.insert(URL_PACKET_MAP_ValueType( req->easyHandle,req ));
    Http_UnLock(m_httpLock);

    sem_post( s_pSem );
    
    return true;
}

void CHttp::restart()
{
    if( m_bStop )
    {
        m_pMulti_handle = curl_multi_init();
        m_bStop = false;
    }
}

bool CHttp::addPostRequest( std::string addr,std::string action,const char* info )
{
    restart();
    HttpRequestPacket* req = new HttpRequestPacket();
    if( NULL == req ){
        HTTP_ERROR( "%s","HttpRequestPacket new 内存空间不够\0"); return false;
    }
    m_nTimeOut = 3;
    m_nConnectTimeOut = 5;
    req->requestType = eRT_PostRequest;
    req->url = addr;
    req->action = action;
    req->info = info;
    req->bActive = false;
    req->easyHandle = curl_easy_init();
    req->strPostDatas = m_strPostDatas;
    
    CURLcode code;
    code = curl_easy_setopt(req->easyHandle, CURLOPT_HTTPHEADER,m_pCurlheaders);            //--设置头的信息--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_HTTPHEADER 属性错误\0",code);
		return false;
    }
    // code = curl_easy_setopt(req->easyHandle, CURLOPT_VERBOSE,1 );                        //--debug output--
    // code = curl_easy_setopt(req->easyHandle, CURLOPT_HEADER,1 );                         //--headers in the output--
    code = curl_easy_setopt(req->easyHandle, CURLOPT_NOSIGNAL, 1L);                         //--多线程超时崩溃 需要写这个--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_NOSIGNAL 属性错误\0",code);
		return false;
    }
    
    code = curl_easy_setopt(req->easyHandle, CURLOPT_URL, req->url.c_str() );               //--设置url--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_URL 属性错误\0",code);
		return false;
    }
    
    curl_easy_setopt(req->easyHandle, CURLOPT_CONNECTTIMEOUT, 10 );                          //--设置连接超时--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_CONNECTTIMEOUT 属性错误\0",code);
		return false;
    }
    curl_easy_setopt(req->easyHandle, CURLOPT_TIMEOUT, 10);                            //--正在下载的时候超时--
    // curl_easy_setopt(req->easyHandle, CURLOPT_DNS_CACHE_TIMEOUT, 10);                            //--正在下载的时候超时--
    // curl_easy_setopt(req->easyHandle, CURLOPT_TIMEOUT_MS, 10);                            //--正在下载的时候超时--
    // curl_easy_setopt(req->easyHandle, CURLOPT_CONNECTTIMEOUT_MS, 10);                            //--正在下载的时候超时--
    
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_TIMEOUT 属性错误\0",code);
		return false;
    }

  //  curl_easy_setopt(req->easyHandle, CURLOPT_DNS_CACHE_TIMEOUT, 5);
    
    //--body1-- 设置body方法一--
    
    curl_easy_setopt(req->easyHandle, CURLOPT_POSTFIELDS,req->strPostDatas.c_str() );          //--添加body方法--
    curl_easy_setopt(req->easyHandle, CURLOPT_POSTFIELDSIZE,req->strPostDatas.length() );      //--添加body方法--
    
    initPostData();  //注意看看这个需不需要加
    
    //--body2-- 设置body方法二--
    //  curl_httppost* post = NULL;
    //  curl_httppost* last = NULL;
    //  curl_formadd(&post, &last,CURLFORM_COPYNAME,"name",CURLFORM_COPYCONTENTS,"liguangri",CURLFORM_END); //--添加字符串——-
    //  curl_formadd(&post, &last,CURLFORM_COPYNAME,"address",CURLFORM_COPYCONTENTS,"beijing",CURLFORM_END);//--添加字符串——-
    //  curl_formadd(&post, &last,CURLFORM_COPYNAME,"address",CURLFORM_FILECONTENT,"me.txt",CURLFORM_END);  //--从txt文件里面获取数据--
    //    curl_easy_setopt(req->easyHandle, CURLOPT_HTTPPOST,post);
    //recv 和 header 回调函数
    code = curl_easy_setopt(req->easyHandle, CURLOPT_HEADERFUNCTION,writeHeader  );          //--设置头回调函数--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_HEADERFUNCTION 属性错误\0",code);
		return false;
    }
    code = curl_easy_setopt(req->easyHandle, CURLOPT_HEADERDATA,req );                      //--设置头回调函数参数--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_HEADERDATA 属性错误\0",code);
		return false;
    }
    code = curl_easy_setopt(req->easyHandle, CURLOPT_WRITEFUNCTION, writeData );            //--设置下载回调函数--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_WRITEFUNCTION 属性错误\0",code);
		return false;
    }
    code = curl_easy_setopt(req->easyHandle, CURLOPT_WRITEDATA,req  );                      //--设置给下载回调函数的最后一个参数--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_WRITEDATA 属性错误\0",code);
		return false;
    }
    
    if( m_isHttps ){
        code = curl_easy_setopt(req->easyHandle, CURLOPT_SSL_VERIFYPEER,0L);                    //--https--
        if (code != CURLE_OK){
            HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_SSL_VERIFYPEER 属性错误\0",code);
            return false;
        }
        code = curl_easy_setopt(req->easyHandle, CURLOPT_SSLKEY,m_keyFileName.c_str());                 //--https key--
        if (code != CURLE_OK){
            HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_SSLKEY 属性错误\0",code);
            return false;
        }
        code = curl_easy_setopt(req->easyHandle, CURLOPT_SSLCERT,m_crtFileName.c_str());                //--https crt--
        if (code != CURLE_OK){
            HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_SSLCERT 属性错误\0",code);
            return false;
        }
        code = curl_easy_setopt(req->easyHandle, CURLOPT_CAINFO,m_caCrtFileName.c_str());               //--https ca crt--
        if (code != CURLE_OK){
            HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_CAINFO 属性错误\0",code);
            return false;
        }
    }
    

    // int retcode;
    // code = curl_easy_getinfo(req->easyHandle, CURLINFO_RESPONSE_CODE , &retcode);
    // HTTP_DEBUG( "%s%d%d","===============",code,retcode);
    
    ::gettimeofday(&req->start_time, NULL) ;
    curl_multi_add_handle(m_pMulti_handle, req->easyHandle);
    Http_Lock(m_httpLock);
    m_mapRequestMgr.insert(URL_PACKET_MAP_ValueType( req->easyHandle,req ));
    Http_UnLock(m_httpLock);
    
    sem_post( s_pSem );
    HTTP_DEBUG( "%s","post message is ok");
    return true;
}

bool CHttp::addDownloadRequest( std::string url,std::string action,char *pszLocalFile )
{
    m_nTimeOut = 10;
    m_nConnectTimeOut = 5;
    restart();
    HttpRequestPacket* req = new HttpRequestPacket();
    if( NULL == req ){
        HTTP_ERROR( "%s","HttpRequestPacket new 内存空间不够\0"); return false;
    }
    std::string filename;
    if (pszLocalFile && *pszLocalFile) //自定义文件名字的时候
    {
        filename.assign(pszLocalFile);
    }
    else                               //从url里获取文件名字
    {
        for( int i = url.length()-1; i >= 0; --i )
        {
            if( url[i] == '/' )
            {
                filename = url.substr(i+1);
                break;
            }
        }
    }
    std::string  strFileSavePath;
    strFileSavePath += m_strShareSavePath;  strFileSavePath += filename; strFileSavePath += ".temp";  //--下载文件保存路径设置 下载的时候 保存成.temp文件 等下载完 取消.temp
    
    //检测同名文件
//    Http_Lock(m_httpLock);
//    std::map< CURL*,HttpRequestPacket* >::iterator itr = m_mapRequestMgr.begin();
//    for( ; itr!=m_mapRequestMgr.end(); itr++ )
//    {
//        std::string str = itr->second->info;
//        if( str == filename){
//            return false;
//        }
//    }
//    Http_UnLock(m_httpLock);
    
    if( m_bResumeForFileDownload )                                                      //--断点传序--
        req->tempfile = fopen( strFileSavePath.c_str(),"ab+" );                         //--采用追加方式打开文件--
    else
        req->tempfile = fopen( strFileSavePath.c_str(),"wb" );                          //--采用追加方式打开文件--
    
    if( NULL == req->tempfile )
    {
        HTTP_ERROR( "%s","tempfile 临时文件创建错误 路径错误"); return false;
    }
    /* pipu */
    local_file_len = -1 ;
    // before
//    curl_off_t local_file_len = -1 ;
    /**/
    local_file_len = ftell(req->tempfile);                                              //--获取temp文件大小--
    if(local_file_len <= 0) local_file_len = 0;
    
    req->tempfilename = strFileSavePath;                                                //--temp文件保存路径--
    req->requestType = eRT_DownLoad;                                                    //--请求类型--
    req->url += url;                                                                    //--请求服务器url--
    req->info = filename;                                                               //--下载文件名字--
    req->action = action;                                                               //--作用协议--
    req->bActive = false;
    req->easyHandle = curl_easy_init();                                                 //--libcurl 请求句柄--
    req->nOldSize = local_file_len;
    req->nMaxSize = -1;
    
    CURLcode code;
    code = curl_easy_setopt(req->easyHandle, CURLOPT_HTTPHEADER,m_pCurlheaders);           //--设置头的信息--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_HTTPHEADER 属性错误\0",code);
		return false;
    }
    code = curl_easy_setopt(req->easyHandle, CURLOPT_VERBOSE,1 );                          //--debug output--
    //code = curl_easy_setopt(req->easyHandle, CURLOPT_HEADER,1 );                           //--headers in the output--
    //设置url
    code = curl_easy_setopt(req->easyHandle, CURLOPT_URL, req->url.c_str() );              //--设置url--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_URL 属性错误\0",code);
		return false;
    }
    
    code = curl_easy_setopt(req->easyHandle, CURLOPT_FOLLOWLOCATION,1 );
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_FOLLOWLOCATION 属性错误\0",code);
        return false;
    }
    
    curl_easy_setopt(req->easyHandle, CURLOPT_CONNECTTIMEOUT, 10 );         //--设置连接超时--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_CONNECTTIMEOUT 属性错误\0",code);
		return false;
    }
    code=curl_easy_setopt(req->easyHandle, CURLOPT_TIMEOUT, m_nTimeOut );                    //--正在下载的时候超时--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_TIMEOUT 属性错误\0",code);
		return false;
    }
    code = curl_easy_setopt(req->easyHandle, CURLOPT_NOSIGNAL, 1L);                          //--多线程超时崩溃 需要写这个--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_NOSIGNAL 属性错误\0",code);
		return false;
    }
    //recv 和 header 回调函数
    code = curl_easy_setopt(req->easyHandle, CURLOPT_HEADERFUNCTION,writeHeader  );           //--设置头回调函数--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_HEADERFUNCTION 属性错误\0",code);
		return false;
    }
    code = curl_easy_setopt(req->easyHandle, CURLOPT_HEADERDATA,req );                       //--设置头回调函数参数--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_HEADERDATA 属性错误\0",code);
		return false;
    }
    
    if( m_bResumeForFileDownload )curl_easy_setopt(req->easyHandle, CURLOPT_RESUME_FROM_LARGE ,local_file_len);//--断点传序专用告诉服务器需要传序的位置--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_RESUME_FROM_LARGE 属性错误\0",code);
		return false;
    }
    code = curl_easy_setopt(req->easyHandle, CURLOPT_WRITEFUNCTION, writeFile );            //--设置下载回调函数--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_WRITEFUNCTION 属性错误\0",code);
		return false;
    }
    code = curl_easy_setopt(req->easyHandle, CURLOPT_WRITEDATA,req  );                      //--设置给下载回调函数的最后一个参数--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_WRITEDATA 属性错误\0",code);
		return false;
    }
    //下载进度显示
    code = curl_easy_setopt(req->easyHandle, CURLOPT_NOPROGRESS,0  );                       //--设置0说明下载进度显示--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_NOPROGRESS 属性错误\0",code);
		return false;
    }
    code = curl_easy_setopt(req->easyHandle, CURLOPT_PROGRESSFUNCTION,progress_callback  ); //--设置下载进度显示回调函数--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_PROGRESSFUNCTION 属性错误\0",code);
		return false;
    }
    code = curl_easy_setopt(req->easyHandle, CURLOPT_PROGRESSDATA,req  );                   //--下载进度回调函数的第一个参数传输--
    if (code != CURLE_OK){
        HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_PROGRESSDATA 属性错误\0",code);
		return false;
    }
    if( m_isHttps ){
        code = curl_easy_setopt(req->easyHandle, CURLOPT_SSL_VERIFYPEER,0L);                    //--https--
        if (code != CURLE_OK){
            HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_SSL_VERIFYPEER 属性错误\0",code);
            return false;
        }
        code = curl_easy_setopt(req->easyHandle, CURLOPT_SSLKEY,m_keyFileName.c_str());                 //--https key--
        if (code != CURLE_OK){
            HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_SSLKEY 属性错误\0",code);
            return false;
        }
        code = curl_easy_setopt(req->easyHandle, CURLOPT_SSLCERT,m_crtFileName.c_str());                //--https crt--
        if (code != CURLE_OK){
            HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_SSLCERT 属性错误\0",code);
            return false;
        }
        code = curl_easy_setopt(req->easyHandle, CURLOPT_CAINFO,m_caCrtFileName.c_str());               //--https ca crt--
        if (code != CURLE_OK){
            HTTP_ERROR( "%s%d","curl_easy_setopt 设置 CURLOPT_CAINFO 属性错误\0",code);
            return false;
        }
    }
    
    // 开始计时
	gettimeofday(&g_start, NULL) ;
    g_nDownNum = 0;
    g_nSpeed = 0;
    
    ::gettimeofday(&req->start_time, NULL) ;
    curl_multi_add_handle(m_pMulti_handle, req->easyHandle);
    Http_Lock(m_httpLock);
    m_mapRequestMgr.insert(URL_PACKET_MAP_ValueType( req->easyHandle,req ));
    Http_UnLock(m_httpLock);
    
    sem_post( s_pSem );
    return true;
}

/* pipu */
void CHttp::setLocalFileLen(int len)
{
    local_file_len = len;
}

void CHttp::setDownLoadTotal(double total)
{
    m_DownLoadTotal = total;
}
/**/