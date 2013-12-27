#include "SceneRender.h"
#include "cocos-ext.h"
#include "FileInfo.h"
#include "SceneHelper.h"
#include "SimpleAudioEngine.h"
#include "VitConnect.h"
#include "ZBarInterface.h"
#include "CocoStudio/Trigger/TriggerBase.h"
#include "trigger/EventDef.h"
#include "ColliderRectDrawer.h"

using namespace cocos2d;
using namespace cocos2d::extension;

SceneRender::SceneRender(const char* filename)
{
	pScene = NULL;
	this->setContentSize(CCDirector::sharedDirector()->getWinSize());
	if (strcmp(filename, "") == 0)
	{
		return;
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
//	pCloseItem->setScale(2.0);

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 9999);

	CCLayer* pLayer = CCLayer::create();
	this->addChild(pLayer);
	
	pScene = SceneReader::sharedSceneReader()->createNodeWithSceneFile(filename);
	pLayer->addChild(pScene);
    
    sendEvent(TRIGGEREVENT_INITSCENE);
	this->schedule(schedule_selector(SceneRender::gameLogic));
	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);
}

void SceneRender::menuCloseCallback(CCObject* pSender)
{
    /* pipu */
    ZBarInterface::sharedZBarInterface()->DisRender();
    /**/
    
//    CCDirector::sharedDirector()->popScene();
    SceneHelper* sceneHelper = SceneHelper::sharedSceneHelper();
	sceneHelper->setTouchRender(false);
    /* pipu */
    // refactoring
//    sceneHelper->setHelloWorld(SceneHelper::sharedSceneHelper()->getHelloWorld());
    //
    sceneHelper->setNowRunning(HELLOWORLD);
    /**/
    /* refactoring */
    sceneHelper->setHelloWorldState(NORMAL);
    CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
    // before
//	CCDirector::sharedDirector()->replaceScene(sceneHelper->getHelloWorld());
    /**/
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
    sendEvent(TRIGGEREVENT_ENTERSCENE);
}
void SceneRender::onExit()
{
	if(aa)
	{
		aa->stop();
	}
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    sendEvent(TRIGGEREVENT_LEAVESCENE);
    cocos2d::extension::SceneReader::sharedSceneReader()->purgeSceneReader();
    CCLayer::onExit();
}


bool SceneRender::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	sendEvent(TRIGGEREVENT_TOUCHBEGAN);
	return true;
}

void SceneRender::comCallBack(cocos2d::CCObject *tar, void *dict)
{
	if (dict == NULL || tar == NULL)
	{
		return;
	}
	CCArmature *pAr = dynamic_cast<CCArmature*>(tar);
	if (pAr == NULL)
	{
		return;
	}
	rapidjson::Value *v = (rapidjson::Value *)dict;
    bool isShowColliderRect = DICTOOL->getBooleanValue_json(*v, "isShowColliderRect");
	if (isShowColliderRect == false)
	{
		return;
	}
	
	ColliderRectDrawer *drawer = ColliderRectDrawer::create(pAr);
	if (pAr->getParent())
	{
		pAr->getParent()->addChild(drawer);
	}
	drawer->setVisible(true);
}

void SceneRender::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	sendEvent(TRIGGEREVENT_TOUCHMOVED);
}

void SceneRender::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	sendEvent(TRIGGEREVENT_TOUCHENDED);
}

void SceneRender::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	sendEvent(TRIGGEREVENT_TOUCHCANCELLED);
}

void SceneRender::gameLogic(float dt)
{
	sendEvent(TRIGGEREVENT_UPDATESCENE);
}
