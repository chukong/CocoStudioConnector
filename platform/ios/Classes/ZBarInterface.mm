

#include "ZBarInterface.h"
#import "ZBarViewController.h"
#import <UIKit/UIKit.h>
#import <UIKit/UIApplication.h>
#include "VitConnect.h"
#include <stdlib.h>
#include "FileInfo.h"
#include <string.h>
#include "cocos2d.h"
#include "SceneHelper.h"
#include "VitProtocol.h"
#include "CHttp.h"
#include "FileInfo.h"
#include "ZBarInterface.h"
#include "ControlUI.h"


#import <Foundation/Foundation.h>
#import "AsyncSocket.h"


#define SRV_CONNECTED       0
#define SRV_CONNECT_SUC     1
#define SRV_CONNECT_FAIL    2
#define HOST_IP             @"192.168.203.93"
#define HOST_PORT           36146


@interface ZBarInterfaceIOS : NSObject
{
    AsyncSocket* client;
    ZBarInterface* zbar;
}

@property (nonatomic, retain) AsyncSocket* client;

/** returns a shared instance of the director */
+(ZBarInterfaceIOS*)sharedInstance;

- (void)sendMsg:(NSString *)msg;
- (void)showMessage:(NSString *)msg;
- (BOOL)isConnected;
- (int)connectServer:(NSString*)hostIP port:(int)hostPort;
- (void)disConnect;
//- (void)reConnect;

- (void)downLoadZip:(NSString *)msg;

@end

@implementation ZBarInterfaceIOS

@synthesize client;


#pragma mark singleton
//
// singleton stuff
//
static ZBarInterfaceIOS* _sharedInstance = nil;

+ (ZBarInterfaceIOS *)sharedInstance
{
    if (!_sharedInstance)
    {
        _sharedInstance = [[self alloc] init];
    }
    
    return _sharedInstance;
}

#pragma mark dealloc

- (void)dealloc {
    
    [client setDelegate:nil];
    [client disconnect];
    [client release];
    [super dealloc];
}

- (void)sendMsg:(NSString *)msg
{
    NSLog(@"sendMsg");
    
    NSString* content = msg;
    NSLog(@"%@", content);
    NSData* data = [content dataUsingEncoding:NSUTF8StringEncoding];
//    NSData* data = [content dataUsingEncoding:NSISOLatin1StringEncoding];
    [client writeData:data withTimeout:-1 tag:0];
}

- (BOOL)isConnected
{
    if (client)
    {
        return [client isConnected];
    }
    return NO;
}

- (int)connectServer:(NSString *)hostIP port:(int)hostPort
{
    NSLog(@"connectServer");
    
    /* refactoring */
    SceneHelper* sceneHelper = SceneHelper::sharedSceneHelper();
    /**/
    
    if (client == nil)
    {
        client = [[AsyncSocket alloc] initWithDelegate:self];
        NSError* err = nil;
        // 192.168.110.128
        if (![client connectToHost:hostIP onPort:hostPort error:&err])
        {
            NSLog(@"%i %@", [err code], [err localizedDescription]);
            
            UIAlertView* alert = [[UIAlertView alloc] initWithTitle:[@"Connection failed to host "
                                                                     stringByAppendingString:hostIP]
                                                            message:[[[NSString alloc]initWithFormat:@"%i",[err code]] stringByAppendingString:[err localizedDescription]]
                                                           delegate:self
                                                  cancelButtonTitle:@"OK"
                                                  otherButtonTitles:nil];
            [alert show];
            [alert release];
            
            client = nil;
            
            /* refactoring */
            sceneHelper->setSocketState(SOCKET_NONE);
            sceneHelper->setLoginEnabled(false);
            // before
            /*
            SceneHelper::sharedSceneHelper()->setWaitDownLoadStart(false);
             */
            /**/
            
            return SRV_CONNECT_FAIL;
        }
        else
        {
            sceneHelper->setSocketState(SOCKET_CONNECT);
            NSLog(@"Connect to u!");
            return SRV_CONNECT_SUC;
        }
    }
    else
    {
        sceneHelper->setSocketState(SOCKET_CONNECT);
        if (sceneHelper->getDownLoadState() == DOWNLOAD_NONE)
        {
            sceneHelper->setDownLoadState(WAIT_DOWNLOAD_START);
        }
        [client readDataWithTimeout:-1 tag:0];
        return SRV_CONNECTED;
    }
}

- (void)disConnect
{
    [client disconnect];
    [client setDelegate:nil];
}

/*
- (void)reConnect
{
    int stat = [self connectServer:HOST_IP port:HOST_PORT];
    NSLog(@"reconnect: %i", stat);
    
    switch (stat)
    {
        case SRV_CONNECT_SUC:
            [self showMessage:@"connect success"];
            break;
            
        case SRV_CONNECTED:
            [self showMessage:@"It's connected, don't again"];
            break;
            
        default:
            break;
    }
}
 */

#pragma mark socket uitl

- (void)showMessage:(NSString *)msg
{
    UIAlertView * alert = [[UIAlertView alloc]initWithTitle:@"Alert!"
                                                    message:msg
                                                   delegate:nil
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil];
    [alert show];
    [alert release];
}

- (void)downLoadZip:(NSString *)msg
{
//    std::string msg = "INFO URL@http://192.168.203.93/Package.zip PROJNAME@projectName WIDTH@960 HEIGHT@640";
    
    NSString* temp = [NSString stringWithString:msg];
    NSLog(@"temp = %@", temp);
    
    NSRange leftBracketRange = [temp rangeOfString:@"["];
    NSRange rightBracketRange = [temp rangeOfString:@"]"];
    NSString* intercept = [temp substringWithRange:NSMakeRange(leftBracketRange.location + 1,
                                                               rightBracketRange.location - 1 - leftBracketRange.location)];
    NSLog(@"intercept = %@", intercept);
    NSArray* array =[intercept componentsSeparatedByString:NSLocalizedString(@" ", nil)];
    for (int i = 0; i < [array count]; ++i)
    {
        NSLog(@"%@", [array objectAtIndex:i]);
    }
    
    const int count = [array count];
    NSString* data[count];
    const char* c_data[count];
    NSString* tag = [[NSString alloc]initWithString:@"@"];
    for (int i = 1; i < count - 1; i++)
    {
        NSString* value = [array objectAtIndex:i];
        NSRange range = [value rangeOfString:tag];
        int location = range.location;
        data[i] = [value substringFromIndex:(location + 1)];
        c_data[i] = [data[i] UTF8String];
        printf("c_data[i] = %s\n", c_data[i]);
    }
    int value = 0;
    for (int i = 1; i < count - 1; ++i)
    {
        if (strcmp("", c_data[i]) != 0)
        {
            value++;
        }
    }
    if (value == count - 2)
    {
        [self sendMsg:@"STARTDOWNLOAD\n"];
    }
    ZBarInterface::sharedZBarInterface()->DownLoadZip(c_data[1], "", c_data[2], c_data[3], c_data[4]);
}

#pragma mark socket delegate

- (void)onSocket:(AsyncSocket *)sock didConnectToHost:(NSString *)host port:(UInt16)port
{
    [client readDataWithTimeout:-1 tag:0];
}

- (void)onSocket:(AsyncSocket *)sock willDisconnectWithError:(NSError *)err
{
    NSLog(@"Error");
    
    SceneHelper* sceneHelper = SceneHelper::sharedSceneHelper();
    sceneHelper->setLoginEnabled(false);
    /* refactoring */
//    SceneHelper::sharedSceneHelper()->setWaitDownLoadStart(false);
    /**/
}

- (void)onSocketDidDisconnect:(AsyncSocket *)sock
{
    /*
    NSString *msg = @"Sorry this connect is failure";
    [self showMessage:msg];
    [msg release];
     */
    
    client = nil;
    
    if (ZBarInterface::sharedZBarInterface()->isAppEnterForeground()
        || ZBarInterface::sharedZBarInterface()->isAppEnterBackground())
    {
        if (ZBarInterface::sharedZBarInterface()->isAppEnterForeground())
        {
            ZBarInterface::sharedZBarInterface()->setAppEnterForeground(false);
        }
        if (ZBarInterface::sharedZBarInterface()->isAppEnterBackground())
        {
            ZBarInterface::sharedZBarInterface()->setAppEnterBackground(false);
        }
        
        /* refactoring */
        if (SceneHelper::sharedSceneHelper()->getDownLoadState() == DOWNLOADING)
        {
            ZBarInterface::sharedZBarInterface()->setReConnectWithAppEnterForeground(true);
        }
        // before
        /*
        if (SceneHelper::sharedSceneHelper()->isDownloading())
        {
            ZBarInterface::sharedZBarInterface()->setReConnectWithAppEnterForeground(true);
        }
         */
        /**/
        
        std::string value = ZBarInterface::sharedZBarInterface()->getIP();
        int colonIndex = value.find_last_of(":");
        std::string strIP = value.substr(0, colonIndex);
        std::string strPort = value.substr(colonIndex + 1, value.length());
        int port = atoi(strPort.c_str());
        ZBarInterface::sharedZBarInterface()->CreateConnect(strIP.c_str(), port);
    }
    else
    {
        NSString* msg = @"Sorry this connect is failure";
        [self showMessage:msg];
        [msg release];
        
        if (SceneHelper::sharedSceneHelper()->getDownLoadState() == DOWNLOADING)
        {
            ZBarInterface::sharedZBarInterface()->stopDownLoad();
        }
        SceneHelper::sharedSceneHelper()->setSocketState(SOCKET_SERVER_IS_DOWN);
    }
}

- (void)onSocketDidSecure:(AsyncSocket *)sock
{
    NSLog(@"onSocketDidSecure");
}

- (void)onSocket:(AsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag
{
    
//    NSLog(@" (void)onSocket:(AsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag");
    
    NSString* content = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    NSLog(@"Hava received datas is :%@", content);
    
    
    NSArray* array =[content componentsSeparatedByString:NSLocalizedString(@" ", nil)];
    /*
    for(int i = 0; i < [array count]; i++)
    {
        NSLog(@"%@",[array objectAtIndex:i]);
    }
     */
    NSString* start = [array objectAtIndex:0];
    NSString* mark = [[NSString alloc]initWithString:@"["];
    NSRange range = [start rangeOfString:mark];
    NSString* title = [start substringFromIndex:(range.location + 1)];

    NSLog(@"title = %@", title);
    if ([title isEqualToString:@"LOGINOK"])
    {
        SceneHelper* sceneHelper = SceneHelper::sharedSceneHelper();
        sceneHelper->setLoginEnabled(true);
        /* refactoring */
        if (sceneHelper->getDownLoadState() == DOWNLOAD_NONE)
        {
            sceneHelper->setDownLoadState(WAIT_DOWNLOAD_START);
        }
        // before
//        SceneHelper::sharedSceneHelper()->setWaitDownLoadStart(true);
        /**/
    }
    else if ([title isEqualToString:@"STOP"])
    {
        /* refactoring */
        if (SceneHelper::sharedSceneHelper()->getDownLoadState() == DOWNLOADING)
        // before
//        if (SceneHelper::sharedSceneHelper()->isDownloading()) // during downloaing
        /**/
        {
            ZBarInterface::sharedZBarInterface()->stopDownLoad();
            NSLog(@"stop during downloading");
        }
        else // in Scene
        {
            SceneHelper::sharedSceneHelper()->setNowRunning(HELLOWORLD);
            cocos2d::CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
        }
    }
    else if ([title isEqualToString:@"CHECKCONNECTION"])
    {
        
    }
    else if ([title isEqualToString:@"INFO"])
    {
        SceneHelper* sceneHelper = SceneHelper::sharedSceneHelper();
        
        /* pipu */
        if (sceneHelper->getNowRunning() == RENDER)
        {
            sceneHelper->setTouchRender(false);
            /* refactoring */
//            sceneHelper->setHelloWorld(SceneHelper::sharedSceneHelper()->getHelloWorld());
            /**/
            sceneHelper->setHelloWorldState(NORMAL);
            sceneHelper->setNowRunning(HELLOWORLD);
            cocos2d::CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
        }
        /**/
        /* refactoring */
//        sceneHelper->setWaitDownLoadStart(false);
        /**/
        [self downLoadZip:content];
    }
    
    [content release];
    [client readDataWithTimeout:-1 tag:0];
}

@end


using namespace cocos2d;

void	error_cb(const char* action,const char* info,int errortype,int repose )
{
	if(errortype == 7)
	{
        SceneHelper* sceneHelper = SceneHelper::sharedSceneHelper();
        sceneHelper->setLastError(action, info, errortype, repose);
        /* refactoring */
        sceneHelper->setDownLoadState(HTTP_SERVER_IS_DOWN);
        // before
//		SceneHelper::sharedSceneHelper()->setPrint(true);
        /**/
	}
    else if (errortype == 2000)
    {
        CCLOG("errertype 2000 down HTTP START");
        CHttp * htp = CHttp::getSingletonPtr();
        htp->addDownloadRequest(FileInfo::sharedFileInfo()->getURL(),"down",NULL);
    }
	else
	{
        /* pipu */
        CHttp* http = CHttp::getSingletonPtr();
        http->setDownLoadRequestCount(http->getDownLoadRequestCount() + 1);
        if (http->getDownLoadRequestCount() == 2)
        {
            /* refactoring */
            SceneHelper::sharedSceneHelper()->setSocketState(SOCKET_SERVER_IS_DOWN);
            // before
//            SceneHelper::sharedSceneHelper()->setServerIsDown(true);
            /**/
            ZBarInterface* zbar = ZBarInterface::sharedZBarInterface();
            zbar->stopDownLoad();
        }
        else
        {
            CCLOG("down HTTP START");
            CHttp * htp = CHttp::getSingletonPtr();
            htp->addDownloadRequest(FileInfo::sharedFileInfo()->getURL(),"down",NULL);
        }
        // before
        /*
        CCLOG("down HTTP START");
		//===============HTTP START===============
		CHttp * htp = CHttp::getSingletonPtr();
		htp->addDownloadRequest(FileInfo::sharedFileInfo()->getURL(),"down",NULL);
         */
        /**/
	}
}
void    callback_cb( const char* action,const char* info,const char* data,bool isfile )
{
    if( isfile )
    {
        //data 是文件名字
        //进入这里说名指定的文件下载成功
        printf("==%s 下载成功\n",data);
		
        /* refactoring */
        SceneHelper::sharedSceneHelper()->setDownLoadState(DOWNLOADED_FINISHED);
        // before
        /*
        SceneHelper::sharedSceneHelper()->setDownLoadFinished(true);
		SceneHelper::sharedSceneHelper()->setDownloading(false);
         */
        /**/
    }
    else
    {
       if( strcmp(action, "login") == 0 )
       {
          // printf("从服务器反馈login请求 数据是%s",data);
       }
        printf("data %s\n", data);
    }
}
void    getCurUpdateProgress_cb( const char* action,const char* filename,float amount )
{
//     CHttp * htp = CHttp::getSingletonPtr();
//   int speed =  htp->getDownSpeed();
//    printf("当前下载进度 文件名字 %s===%f=-------%d \n",filename,amount, speed);
	printf("amount = %f\n", amount);
	SceneHelper::sharedSceneHelper()->setDownLoadByte(amount);
}

ZBarInterface* ZBarInterface::s_ZBarInterface = NULL;

ZBarInterface::ZBarInterface()
: m_strIP("")
, m_bAppEnterBackground(false)
, m_bAppEnterForeground(false)
, m_bReConnectWithAppEnterForeground(false)
{
}
ZBarInterface::~ZBarInterface()
{
}
void ZBarInterface::pick()
{
	[[ViewController sharedViewController] pick];
}
void ZBarInterface::DownLoadZip(const char* zip, const char*version, const char* project, const char* width, const char* height)
{
	int iwidth = std::atoi(width);
	int iheight = std::atoi(height);
	//设置文件信息
	FileInfo::sharedFileInfo()->setFileInfo(zip, version, project, iwidth, iheight);

	//*****************HTTP START**********************
    CHttp * htp = CHttp::getSingletonPtr();
    htp->registerCallback(callback_cb,error_cb,getCurUpdateProgress_cb);
    htp->setAllowResumeForFileDownloads(true);
	
	std::string path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath();             //--下载资源需要保存的指定路径--
	CCLog("save path: %s\n", path.c_str());
//	std::string path =  "/Users/jxhgzs/Music/";             //--下载资源需要保存的指定路径--
//	FileDownload::sharedFileDownload()->createDownloadedDir();
    htp->setFileSavePath((char*)path.c_str());
    htp->setTimeOut(30);
    htp->addDownloadRequest((char*)zip,"down",NULL);
	
    /* refactoring */
    SceneHelper::sharedSceneHelper()->setDownLoadState(DOWNLOADING);
    // before
//	SceneHelper::sharedSceneHelper()->setDownloading(true);
    /**/
}


ZBarInterface* ZBarInterface::sharedZBarInterface()
{
	if(s_ZBarInterface == NULL)
	{
		s_ZBarInterface = new ZBarInterface();
	}
	return s_ZBarInterface;
}
bool ZBarInterface::checkFile(const char* file)
{
	NSString *temp = [[NSString alloc] initWithCString:file encoding:NSASCIIStringEncoding];
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	documentsDirectory = [documentsDirectory  stringByAppendingString:@"/"];
	NSString *realPath = [documentsDirectory  stringByAppendingString:temp] ;// + temp;
	NSData *reader = [NSData dataWithContentsOfFile:realPath];
	if (reader == nil)
	{
		return false;
	}
	return true;
}
void ZBarInterface::CleanPath()
{
//	NSString *appDocDir = [[NSString alloc] initWithCString:dir encoding:NSASCIIStringEncoding];
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *appDocDir = [paths objectAtIndex:0];
	NSString *relative = [[[[NSFileManager defaultManager] URLsForDirectory: NSDocumentDirectory inDomains:NSUserDomainMask] lastObject] relativePath];
//	NSString *appDocDir = @"/Users/jxhgzs/Desktop/";
	NSLog(@"relateivePaht %@\n", relative);
        NSArray *contentOfFolder = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:appDocDir error:NULL];
        int count = 1;
        for (NSString *aPath in contentOfFolder) {
            NSString * fullPath = [appDocDir stringByAppendingPathComponent:aPath];
            BOOL isDir = NO;
            if ([[NSFileManager defaultManager] fileExistsAtPath:fullPath isDirectory:&isDir])
            {
//                if (isDir == YES) {
					NSLog(@"dir-%d: %@", count, aPath);
//						NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
//	NSString *documentsDirectory = [paths objectAtIndex:0];
//	NSLog(@"documentsDirectory%@",documentsDirectory);
	NSFileManager *fileManager = [NSFileManager defaultManager];
//	NSString *testDirectory = [documentsDirectory stringByAppendingPathComponent:path];
//	NSLog(@"path: %@  testDirectory: %@\n", path, testDirectory);
	// 创建目录
					[fileManager removeItemAtPath:fullPath error:NULL];
                    count++;
//                }
//				else
//				{
//				}
            }
        }

}
void ZBarInterface::ReadPath()
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];    
//	NSLog(@"documentsDirectory%@",documentsDirectory);    
	NSFileManager *fileManage = [NSFileManager defaultManager];
//	NSString *myDirectory = [documentsDirectory stringByAppendingPathComponent:path];
	NSArray *file = [fileManage subpathsOfDirectoryAtPath: documentsDirectory error:nil];
	NSLog(@"%@",file);    
	NSArray *files = [fileManage subpathsAtPath: documentsDirectory ];
	NSLog(@"%@",files);
}
const char* ZBarInterface::getDeviceName()
{
//	 NSString * strModel  = [[UIDevice currentDevice] model];
	 NSString * strModel = [[UIDevice currentDevice] name];
	 NSLog(@"model: %@", strModel);
	 
	 m_sDeviceName = [strModel UTF8String];
	 return m_sDeviceName.c_str();
}


/* pipu */
bool ZBarInterface::isConnected()
{
    ZBarInterfaceIOS* ios = [ZBarInterfaceIOS sharedInstance];
    
    return [ios isConnected];
}

int ZBarInterface::CreateConnect(const char* ip, int port)
{
    /* refactoring */
    SceneHelper* sceneHelper = SceneHelper::sharedSceneHelper();
    if (sceneHelper->getSocketState() == SOCKET_SERVER_IS_DOWN)
    {
        sceneHelper->setSocketState(SOCKET_NONE);
        CHttp* http = CHttp::getSingletonPtr();
        http->setDownLoadRequestCount(0);
    }
    // before
    /*
    if (SceneHelper::sharedSceneHelper()->isServerIsDown())
    {
        SceneHelper::sharedSceneHelper()->setServerIsDown(false);
        CHttp* http = CHttp::getSingletonPtr();
        http->setDownLoadRequestCount(0);
    }
     */
    /**/
    
    ZBarInterfaceIOS* ios = [ZBarInterfaceIOS sharedInstance];
    NSString* nsIP = [NSString stringWithUTF8String:ip];
    int value = [ios connectServer:nsIP port:port];
    
    switch (value)
    {
        case SRV_CONNECTED:
        case SRV_CONNECT_SUC:
            {
                std::string login = "LOGIN 137EECCB-4239-47FA-A2E5-9C51F9C0A98F ";
                login.append(getDeviceName());
                login.append("\n");
                NSString* msg = [NSString stringWithUTF8String:login.c_str()];
                [ios sendMsg:msg];
                
                if (m_bReConnectWithAppEnterForeground)
                {
                    m_bReConnectWithAppEnterForeground = false;
                    
                    [ios sendMsg:@"STARTDOWNLOAD\n"];
                }
            }
            break;
            
        case SRV_CONNECT_FAIL:
            
            break;
            
        default:
            break;
    }
    
    return value;
}

void ZBarInterface::DisConnect()
{
    ZBarInterfaceIOS* ios = [ZBarInterfaceIOS sharedInstance];
    
    if ([ios isConnected])
    {
        [ios disConnect];
    }
}

void ZBarInterface::TaskCancel()
{
    ZBarInterfaceIOS* ios = [ZBarInterfaceIOS sharedInstance];

    if ([ios isConnected])
    {
        [ios sendMsg:@"TASKCANCLE\n"];
    }
}

void ZBarInterface::DisRender()
{
    ZBarInterfaceIOS* ios = [ZBarInterfaceIOS sharedInstance];
    
    if ([ios isConnected])
    {
        [ios sendMsg:@"DISRENDER\n"];
    }
}

void ZBarInterface::setIP(const char *ip)
{
    m_strIP = ip;
}

void ZBarInterface::stopDownLoad()
{
    CHttp * htp = CHttp::getSingletonPtr();
    htp->stopRequest();
    htp->setLocalFileLen(0);
    CleanPath();
    SceneHelper::sharedSceneHelper()->setHaveResources(false);
    /* refactoring */
    SceneHelper* sceneHelper = SceneHelper::sharedSceneHelper();
    sceneHelper->setDownLoadState(DOWNLOAD_STOP);
    // before
    /*
    SceneHelper::sharedSceneHelper()->setWaitDownLoadStart(false);
    SceneHelper::sharedSceneHelper()->setDownloading(false);
     */
    /**/
}
/**/

