//#include "HelloWorldScene.h"
#include "FileInfo.h"
#include "ControlUI.h"
#include "SceneHelper.h"
#include "ZBarInterface.h"
#include "unzip.h"
#include <sys/stat.h>
#include <sys/errno.h>
#include "SceneRender.h"
#include "CCArmature.h"
#include "CHttp.h"
USING_NS_CC;


CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	CCSprite*sp;
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
//	if(size.width == 1136)
//	{
		sp = CCSprite::create("background.png");
//	}
//	else if(size.width == 1024)
//	{
//		sp = CCSprite::create("Default-Landscape~ipad.png");
//	}
//	else
	{
//		sp = CCSprite::create("background960_640.png");
	}
	
	
	
	sp->setPosition(ccp(480,320));
	addChild(sp);
	
	m_pGUI = ControlUI::create();
	addChild(m_pGUI);
    
    SceneHelper* sceneHelper = SceneHelper::sharedSceneHelper();
    sceneHelper->setHelloWorldState(NORMAL);

    return true;
}
void HelloWorld::update(float delta)
{
	
    /* refactoring */
    /*
	CHttp * htp = CHttp::getSingletonPtr();
	
	Http_Lock(htp->m_errorHttpLock);
	for( int i = 0; i < htp->m_vecError.size();i++ )
	{
		htp->m_mainVecError.push_back(htp->m_vecError[i]);
	}
	Http_UnLock(htp->m_errorHttpLock);
	htp->m_vecError.clear();
	
	for( int i = 0; i < htp->m_mainVecError.size(); i++ )
	{
		const char* str = htp->m_mainVecError[i].action.c_str();
		if( str )
		{
			CHttp * htp = CHttp::getSingletonPtr();
	//		htp->addDownloadRequest("http://192.168.22.179:1195/Package.zip", "down",NULL);
			htp->addDownloadRequest(FileInfo::sharedFileInfo()->getURL(),"down",NULL);
		}
	}
     */
    /**/
    
    /* refactoring */
    SceneHelper* sceneHelper = SceneHelper::sharedSceneHelper();    
    
    switch (sceneHelper->getHelloWorldState())
    {
        case INPUT_IP:
        {
            m_pGUI->disableConnectButton();
            m_pGUI->disableRenderButton();
            m_pGUI->disableResetButton();
            m_pGUI->disableIPButton();
        }
            break;                    
            
        default:
        {
            m_pGUI->setLoadingBarVisible(false);
            m_pGUI->enableConnectButton();
            m_pGUI->enableResetButton();
            m_pGUI->enableRenderButton();
            m_pGUI->enableIPButton();
            
            if(!SceneHelper::sharedSceneHelper()->isHaveResources())
            {
                m_pGUI->disableRenderButton();
            }
        }
            break;
    }
    
    
    switch (sceneHelper->getSocketState())
    {
        case SOCKET_SERVER_IS_DOWN:
        {
            ZBarInterface* zbar = ZBarInterface::sharedZBarInterface();
            zbar->DisConnect();
            SceneHelper::sharedSceneHelper()->setSocketState(SOCKET_DISCONNECT);            
            m_pGUI->setTitle("Server is down, connect server agian");
        }
            break;
            
        case SOCKET_DISCONNECT:
        {
            sceneHelper->setSocketState(SOCKET_NONE);
            m_pGUI->getConnectImage()->setVisible(false);
            m_pGUI->getDisconnectImage()->setVisible(true);
        }
            break;
            
        case SOCKET_CONNECT:
        {
            m_pGUI->getConnectImage()->setVisible(true);
            m_pGUI->getDisconnectImage()->setVisible(false);
            
            switch (sceneHelper->getDownLoadState())
            {
                case HTTP_SERVER_IS_DOWN:
                {
                    sceneHelper->setDownLoadState(DOWNLOAD_NONE);
                    
                    char temp[512];memset(temp, 0x00, sizeof(temp));
                    if (SceneHelper::sharedSceneHelper()->getErrorType() == 28)
                    {
                        sprintf(temp," TimeOut，Maybe disconnect！Errotype: %d",SceneHelper::sharedSceneHelper()->getErrorType());
                    }
                    else if(SceneHelper::sharedSceneHelper()->getErrorType() == 10000)
                    {
                        sprintf(temp,"Have not resources!");
                    }
                    else
                    {
                        sprintf(temp,"Action: %s; Info: %s; Errotype: %d", SceneHelper::sharedSceneHelper()->getAction(), SceneHelper::sharedSceneHelper()->getInfo(),
                                SceneHelper::sharedSceneHelper()->getErrorType());
                    }
                    m_pGUI->setTitle(temp);
                }
                    break;
                    
                case WAIT_DOWNLOAD_START:
                {
                    m_pGUI->setDownLoadTextVisible(true);
                    m_pGUI->setTitle("Waiting for pushing down start of scene editor");
                }
                    break;
                    
                case DOWNLOADING:
                {                                        
                    m_pGUI->setLoadingBarVisible(true);
                    m_pGUI->setProgress(SceneHelper::sharedSceneHelper()->getDownLoadByte());
                    m_pGUI->disableConnectButton();
                    m_pGUI->disableRenderButton();
                    m_pGUI->disableResetButton();
                    m_pGUI->disableIPButton();
                }
                    break;
                    
                case DOWNLOADED_FINISHED:
                {
                    sceneHelper->setDownLoadState(DOWNLOADED_UNCOMPRESS);
                    
                    m_pGUI->setTitle("Uncompressing");
                    sceneHelper->setHaveResources(true);
                    
                    m_pGUI->disableConnectButton();
                    m_pGUI->disableRenderButton();
                    m_pGUI->disableResetButton();
                    m_pGUI->disableIPButton();
                }
                    break;
                    
                case DOWNLOADED_UNCOMPRESS:
                {
                    sceneHelper->setDownLoadState(DOWNLOAD_NONE);
                    
                    m_pGUI->setTitle("Connect server or render if app can render");
                    m_pGUI->disableConnectButton();
                    m_pGUI->disableRenderButton();
                    m_pGUI->disableResetButton();
                    m_pGUI->disableIPButton();
                    
                    uncompress();
                    sceneHelper->saveSerchPath();
                    sceneHelper->saveConfig();
                    if (strcmp(FileInfo::sharedFileInfo()->getFilename(), "") == 0)
                    {
                        std::string str = "ProjectName Error!";
                        m_pGUI->setTitle(str.c_str());
//                        char temp[128];memset(temp, 0x00, sizeof(temp));
//                        sprintf(temp, "Have not resources!" );
//                        SceneHelper::sharedSceneHelper()->setLastError("", "", 10000, 0);
//                        SceneHelper::sharedSceneHelper()->setPrint(true);
                        return;
                    }
                    sceneHelper->setDownLoadByte(0);
                    sceneHelper->setNowRunning(RENDER);
                    SceneRender* pScene = new SceneRender(FileInfo::sharedFileInfo()->getFilename());
                    CCDirector::sharedDirector()->replaceScene((CCScene*)pScene);
                }
                    break;
                    
                case DOWNLOAD_STOP:
                {
                    sceneHelper->setDownLoadState(WAIT_DOWNLOAD_START);
                }
                    break;
                    
                default:
                    break;
            }
        }
            break;
            
        default:
            break;
    }
    /**/
	
    /*
    // pipu //
    m_pGUI->getConnectImage()->setVisible(ZBarInterface::sharedZBarInterface()->isConnected());
    m_pGUI->getDisconnectImage()->setVisible(!ZBarInterface::sharedZBarInterface()->isConnected());
    //
    
	if (SceneHelper::sharedSceneHelper()->isDownLoadFinished())
	{
//		CCDirector::sharedDirector()->
		
		SceneHelper::sharedSceneHelper()->setHaveResources(true);
		SceneHelper::sharedSceneHelper()->setDownLoadFinished(false);
		uncompress();
		m_pGUI->setTitle("DownLoad Finished");
		SceneHelper::sharedSceneHelper()->saveSerchPath();
		SceneHelper::sharedSceneHelper()->saveConfig();
		if (strcmp(FileInfo::sharedFileInfo()->getFilename(), "") == 0)
		{
			std::string str = "ProjectName Error!";
			m_pGUI->setTitle(str.c_str());
//			char temp[128];memset(temp, 0x00, sizeof(temp));
//			sprintf(temp, "Have not resources!" );
//			SceneHelper::sharedSceneHelper()->setLastError("", "", 10000, 0);
//			SceneHelper::sharedSceneHelper()->setPrint(true);
			return;
		}
		SceneRender* pScene = new SceneRender(FileInfo::sharedFileInfo()->getFilename());
		CCDirector::sharedDirector()->replaceScene((CCScene*)pScene);
		SceneHelper::sharedSceneHelper()->setNowRunning(RENDER);
		SceneHelper::sharedSceneHelper()->setDownloading(false);
//		SceneHelper::sharedSceneHelper()->setHaveResources(true);
		SceneHelper::sharedSceneHelper()->setDownLoadByte(0);
	}
	else if(SceneHelper::sharedSceneHelper()->isDownloading())
	{
        // pipu //
        m_pGUI->setIP(ZBarInterface::sharedZBarInterface()->getIP());
        m_pGUI->setLoadingBarVisible(true);
        //
		m_pGUI->setProgress(SceneHelper::sharedSceneHelper()->getDownLoadByte());
        // pipu //
        m_pGUI->disableConnectButton();
        m_pGUI->disableRenderButton();
        m_pGUI->disableResetButton();
        m_pGUI->disableIPButton();
        //
	}
    //=== pipu ===//
    else if (SceneHelper::sharedSceneHelper()->isWaitDownLoadStart())
    {
        m_pGUI->setDownLoadTextVisible(true);
        m_pGUI->setTitle("Waiting for pushing down start of scene editor");
    }
    //
    else if (SceneHelper::sharedSceneHelper()->isServerIsDown())
    {
        ZBarInterface* zbar = ZBarInterface::sharedZBarInterface();
        if (zbar->isConnected())
        {
            zbar->DisConnect();
        }
        m_pGUI->setTitle("Server is down, connect server agian");
        m_pGUI->setLoadingBarVisible(false);
        m_pGUI->enableConnectButton();
        m_pGUI->enableResetButton();
        m_pGUI->enableRenderButton();
        m_pGUI->enableIPButton();
    }
    else
    {
        // pipu //
        if (!m_pGUI->isInputLayoutVisible())
        {
            m_pGUI->setTitle("Connect server or render if app can render");
            m_pGUI->setLoadingBarVisible(false);
            m_pGUI->enableConnectButton();
            m_pGUI->enableResetButton();
            m_pGUI->enableRenderButton();
            m_pGUI->enableIPButton();
        }
        //
    }
	if(SceneHelper::sharedSceneHelper()->getPrint())
	{
		SceneHelper::sharedSceneHelper()->setPrint(false);
		SceneHelper::sharedSceneHelper()->setDownloading(false);
		
		char temp[512];memset(temp, 0x00, sizeof(temp));
		if (SceneHelper::sharedSceneHelper()->getErrorType() == 28)
		{
			sprintf(temp," TimeOut，Maybe disconnect！Errotype: %d",SceneHelper::sharedSceneHelper()->getErrorType());
		}
		else if(SceneHelper::sharedSceneHelper()->getErrorType() == 10000)
		{
			sprintf(temp,"Have not resources!");
		}
		else
		{
			sprintf(temp,"Action: %s; Info: %s; Errotype: %d", SceneHelper::sharedSceneHelper()->getAction(), SceneHelper::sharedSceneHelper()->getInfo(),
													SceneHelper::sharedSceneHelper()->getErrorType());
		}
		m_pGUI->setTitle(temp);
		
	}
	
	if(SceneHelper::sharedSceneHelper()->isHaveResources())
	{
        // pipu
        if (!m_pGUI->isInputLayoutVisible()
            && !SceneHelper::sharedSceneHelper()->isDownloading())
        {
            m_pGUI->enableRenderButton();
        }
        // before
//        m_pGUI->enableRenderButton();
        //
	}
	else
	{
		m_pGUI->disableRenderButton();
	}
     */
}
void HelloWorld::onEnter()
{
    CCSize *size = SceneHelper::sharedSceneHelper()->getOldSize();
    CCLOG("size width = %f, height = %f", size->width, size->height);
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(size->width,size->height, kResolutionShowAll);
    
	CCLayer::onEnter();
	scheduleUpdate();
    
    /* pipu */
    m_pGUI->setTitle("Connect server or render if app can render");
    m_pGUI->setLoadingBarPercent(0);
    m_pGUI->enableConnectButton();
    m_pGUI->enableResetButton();
    m_pGUI->enableRenderButton();
    m_pGUI->enableIPButton();
    /**/
}
void HelloWorld::onExit()
{
	cocos2d::extension::CCArmatureDataManager::purge();
    cocos2d::extension::SceneReader::sharedSceneReader()->purgeSceneReader();
//    UIHelper::purgeUIHelper();
	cocos2d::extension::ActionManager::shareManager()->purgeActionManager();

	unscheduleUpdate();
	CCLayer::onExit();
}
void HelloWorld::Render(CCObject* sender, cocos2d::extension::TouchEventType type)
{
	if (type == cocos2d::extension::TOUCH_EVENT_ENDED)
	{
		if(SceneHelper::sharedSceneHelper()->isTouchRender())
		{
			return;
		}
	  	if (SceneHelper::sharedSceneHelper()->isDownloading())
		{
			return;
		}
		if(!SceneHelper::sharedSceneHelper()->isHaveResources())
		{
//			m_pGUI->setTitle("Cannot Find Resouces!");
//			((HelloWorld*)SceneHelper::sharedSceneHelper()->getHelloWorld())->m_pGUI->setTitle("Cannot Find Resouces!");
			return;
		}
        
        /* pipu */
        cocos2d::extension::UIButton* render = static_cast<cocos2d::extension::UIButton*>(sender);
        render->setBright(false);
        render->setTouchEnabled(false);
        /**/
		//加载工程名称
		SceneHelper::sharedSceneHelper()->loadConfig();
		SceneHelper::sharedSceneHelper()->saveSerchPath();
		
		if (strcmp(FileInfo::sharedFileInfo()->getFilename(), "") == 0)
		{
            /* pipu */
            render->setBright(false);
            render->setTouchEnabled(true);
            /**/
            
//			std::string str = "Have not resources!";
//			m_pGUI->setTitle(str.c_str());
//			char temp[128];memset(temp, 0x00, sizeof(temp));
//			sprintf(temp, "Have not resources!" );
//			SceneHelper::sharedSceneHelper()->setLastError("", "", 10000, 0);
//			SceneHelper::sharedSceneHelper()->setPrint(true);
			return;
		}
		SceneHelper::sharedSceneHelper()->setTouchRender(true);
		SceneRender* pScene = new SceneRender(FileInfo::sharedFileInfo()->getFilename());
		unscheduleAllSelectors();
        CCDirector::sharedDirector()->replaceScene((CCScene*)pScene);
		SceneHelper::sharedSceneHelper()->setNowRunning(RENDER);
	}
}
bool HelloWorld::uncompress()
{
	std::string _storagePath = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath();
    // Open the zip file
    std::string outFileName = _storagePath + "Package.zip";
    unzFile zipfile = unzOpen(outFileName.c_str());
    if (! zipfile)
    {
        CCLOG("can not open downloaded zip file %s", outFileName.c_str());
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("can not read file global info of %s", outFileName.c_str());
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[8192];
    
    CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[512];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  512,
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            return false;
        }
		
        std::string fullPath = _storagePath + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!createDirectory(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            // Entry is a file, so extract it.
        
			  // Entry is a file, so extract it.
			if (fileName[filenameLength-1] != '/')
			{
				int pos = fullPath.find_last_of("/");
//				printf("pos : %d\n", pos);
				std::string realPath = fullPath.substr(0, pos);
//				printf("path: %s\n", realPath.c_str());
				if (!createDirectory(realPath.c_str()))
				{
					CCLOG("can not create directory %s", fullPath.c_str());
					unzClose(zipfile);
					return false;
				}
			}
			   
				    
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            printf("zipfile %s\n", fullPath.c_str());
            // Create a file to store current file.
            FILE *out = fopen(fullPath.c_str(), "wb");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, 8192);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    unzClose(zipfile);
    CCLOG("end uncompressing");
    
    return true;
}
bool HelloWorld::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#else
    BOOL ret = CreateDirectoryA(path, NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
    return true;
#endif
}

/* pipu */
long long HelloWorld::getCurrentTime()
{
    struct cc_timeval tv;
    CCTime::gettimeofdayCocos2d(&tv, NULL);
    long long  time = ((long long)tv.tv_sec) * 1000+ tv.tv_usec / 1000;
    
    return time;

}
/**/