#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "FileInfo.h"
#include "CCApplication.h"		
#include "SceneHelper.h"
/* pipu */
#include "ZBarInterface.h"
/**/
		
USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

//	CCSize size = CCDirector::sharedDirector()->();
	CCSize size = pEGLView->getFrameSize();
//	FileInfo::sharedFileInfo()->setWidth(size.width);
//	FileInfo::sharedFileInfo()->setHeight(size.height);
//    //set application's width and height
    pEGLView->setDesignResolutionSize(FileInfo::sharedFileInfo()->getWidth(),
                                      FileInfo::sharedFileInfo()->getHeight()
                                      , kResolutionShowAll);

	
    SceneHelper::sharedSceneHelper()->setNowRunning(HELLOWORLD);
    // create a scene. it's an autorelease object
    /* refactoring */
    pDirector->runWithScene(HelloWorld::scene());
    // before
    /*
    SceneHelper::sharedSceneHelper()->setHelloWorld(pScene);
    // run
    pDirector->runWithScene(pScene);
     */
    /**/
	pDirector->setDisplayStats(false);
//	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	SceneHelper::sharedSceneHelper()->setOldSize( size.width, size.height);
	SceneHelper::sharedSceneHelper()->loadConfig();
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    /* pipu */
    ZBarInterface* zbar = ZBarInterface::sharedZBarInterface();
    zbar->setAppEnterBackground(true);
    zbar->setAppEnterForeground(false);
    /**/
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    /* pipu */
    ZBarInterface* zbar = ZBarInterface::sharedZBarInterface();
    zbar->setAppEnterBackground(false);
    zbar->setAppEnterForeground(true);
    /**/
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

