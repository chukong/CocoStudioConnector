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
	
	CCSprite *sp = CCSprite::create("background.png");
	
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
}
void HelloWorld::onEnter()
{
    CCSize *size = SceneHelper::sharedSceneHelper()->getOldSize();
    CCLOG("size width = %f, height = %f", size->width, size->height);
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(size->width,size->height, kResolutionShowAll);
    
	CCLayer::onEnter();
	scheduleUpdate();
    
    m_pGUI->setTitle("Connect server or render if app can render");
    m_pGUI->setLoadingBarPercent(0);
    m_pGUI->enableConnectButton();
    m_pGUI->enableResetButton();
    m_pGUI->enableRenderButton();
    m_pGUI->enableIPButton();
}
void HelloWorld::onExit()
{
	cocos2d::extension::CCArmatureDataManager::purge();
	cocos2d::extension::ActionManager::shareManager()->purge();

	unscheduleUpdate();
	CCLayer::onExit();
}
void HelloWorld::Render(CCObject* sender, ::cocos2d::gui::TouchEventType type)
{
	if (type == ::cocos2d::gui::TOUCH_EVENT_ENDED)
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
			return;
		}
        
        ::cocos2d::gui::UIButton* render = static_cast<cocos2d::gui::UIButton*>(sender);
        render->setBright(false);
        render->setTouchEnabled(false);
        SceneHelper::sharedSceneHelper()->loadConfig();
		SceneHelper::sharedSceneHelper()->saveSerchPath();
		
		if (strcmp(FileInfo::sharedFileInfo()->getFilename(), "") == 0)
		{
            render->setBright(false);
            render->setTouchEnabled(true);
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
				std::string realPath = fullPath.substr(0, pos);
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

long long HelloWorld::getCurrentTime()
{
    struct cc_timeval tv;
    CCTime::gettimeofdayCocos2d(&tv, NULL);
    long long  time = ((long long)tv.tv_sec) * 1000+ tv.tv_usec / 1000;
    
    return time;

}
