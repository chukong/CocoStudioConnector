#include "SceneRender.h"
#include "cocos-ext.h"
#include "FileInfo.h"
#include "SceneHelper.h"
#include "SimpleAudioEngine.h"
#include "VitConnect.h"

#include "ZBarInterface.h"

using namespace cocos2d;
using namespace cocos2d::extension;

SceneRender::SceneRender(const char* filename)
{
	pScene = NULL;
	this->setContentSize(CCDirector::sharedDirector()->getWinSize());
	if (strcmp(filename, "") == 0)
	{
		return;
//		name = CCString::create("Fishjoy2.json");
//		initSceneLayer("Fishjoy2.json");
	}
	else
	{
		initSceneLayer(filename);
	}

}

static ActionObject* aa;

SceneRender::~SceneRender(void)
{
}

void SceneRender::initSceneLayer(const char* filename)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    pCloseItem = CCMenuItemImage::create(
                                                          "CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(SceneRender::menuCloseCallback));

	pCloseItem->setPosition(ccp(visibleSize.width - 70 , 50));
	pCloseItem->setScale(2.0);

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

	CCLayer* pLayer = CCLayer::create();
	this->addChild(pLayer);
	
	pScene = SceneReader::sharedSceneReader()->createNodeWithSceneFile(filename);
	pLayer->addChild(pScene);
	aa = ActionManager::shareManager()->playActionByName("startMenu_1.json", "Animation1");
}

void SceneRender::menuCloseCallback(CCObject* pSender)
{
    ZBarInterface::sharedZBarInterface()->DisRender();
    
    SceneHelper* sceneHelper = SceneHelper::sharedSceneHelper();
    /* pipu */
    sceneHelper->setHelloWorld(SceneHelper::sharedSceneHelper()->getHelloWorld());
    sceneHelper->setNowRunning(HELLOWORLD);
    /**/
//	CCDirector::sharedDirector()->popScene();
	sceneHelper->setTouchRender(false);
	CCDirector::sharedDirector()->replaceScene(sceneHelper->getHelloWorld());
}
void SceneRender::onEnter()
{
	CCLayer::onEnter();
	CCLog("width: %d height: %d\n",FileInfo::sharedFileInfo()->getWidth(),
									FileInfo::sharedFileInfo()->getHeight());
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(FileInfo::sharedFileInfo()->getWidth(),
									FileInfo::sharedFileInfo()->getHeight(), kResolutionShowAll);
									
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	if(pCloseItem)
	{
		pCloseItem->setPosition(ccp(visibleSize.width - 70 , 50));
	}
}
void SceneRender::onExit()
{
	if(aa)
	{
		aa->stop();
	}
//	CC_SAFE_RELEASE(pScene);
//	cocos2d::extension::CCArmatureDataManager::purge();
//    cocos2d::extension::SceneReader::sharedSceneReader()->purgeSceneReader();
//    UIHelper::purgeUIHelper();
//	cocos2d::extension::ActionManager::shareManager()->purgeActionManager();
//	pScene->removeFromParentAndCleanup(true);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	CCLayer::onExit();
}
