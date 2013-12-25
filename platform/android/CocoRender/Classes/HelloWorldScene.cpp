
#include "cocos-ext.h"
#include "HelloWorldScene.h"
#include "Render.h"
#include "FileInfo.h"
#include "trigger/EventDef.h"
#include "ColliderRectDrawer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "../cocos2dx/platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif

using namespace cocos2d;
using namespace cocos2d::extension;

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
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                          "CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(HelloWorld::menuCloseCallback));
    pCloseItem->setScale(2.0);
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width,
                                origin.y + pCloseItem->getContentSize().height));
    
    std::string path = std::string(FileInfo::sharedFileInfo()->getSavePath());
    if(path.empty())
    {
    	CCLabelTTF *pLabel = CCLabelTTF::create("File Error! File unzip error or null",
    	                                            "Arial",30);
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		pLabel->setPosition(ccp(size.width/2, size.height/2));

		addChild(pLabel);
    }
    else
    {
		//save file path
		std::vector<std::string> searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();
		searchPaths.insert(searchPaths.begin(), path);
		CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
		cocos2d::extension::SceneReader::sharedSceneReader()->setTarget(this, callfuncOD_selector(HelloWorld::comCallBack));
		Render* scene = new Render(FileInfo::sharedFileInfo()->getFilename());
		this->addChild(scene,2);
    }
    sendEvent(TRIGGEREVENT_INITSCENE);
    this->schedule(schedule_selector(HelloWorld::gameLogic));
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 9999);
    return true;
}

void HelloWorld::comCallBack(cocos2d::CCObject *tar, void *dict)
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

void HelloWorld::onEnter()
{
	CCLayer::onEnter();
	sendEvent(TRIGGEREVENT_ENTERSCENE);
}

void HelloWorld::onExit()
{
	CCLayer::onExit();
	sendEvent(TRIGGEREVENT_LEAVESCENE);
}

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	sendEvent(TRIGGEREVENT_TOUCHBEGAN);
	return true;
}

void HelloWorld::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	sendEvent(TRIGGEREVENT_TOUCHMOVED);
}

void HelloWorld::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	sendEvent(TRIGGEREVENT_TOUCHENDED);
}

void HelloWorld::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	sendEvent(TRIGGEREVENT_TOUCHCANCELLED);
}

void HelloWorld::gameLogic(float dt)
{
    sendEvent(TRIGGEREVENT_UPDATESCENE);
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	CloseScene(true);
}

void CloseScene(bool autonomous)
{
if(autonomous)
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	#define  LOG_TAG    "JniHelper"
	#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
		JniMethodInfo minfo;
		jobject jobj;
		bool b = JniHelper::getStaticMethodInfo(minfo,
												"cn/cocostudio/render/CocoRender",
												"exitRenderScene",
												"()V");
		if (!b) {
			LOGD("JniHelper::getStaticMethodInfo error...");
		}else{
			jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		}
	#endif
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
