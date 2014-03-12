#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "cocos-ext.h"
#include "ReadJSHelper.h"
#include "ColliderRectDrawer.h"
#include "CocoStudio/Trigger/TriggerBase.h"
#include "trigger/EventDef.h"

USING_NS_CC;
USING_NS_CC_EXT;

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

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    setRandomSeed();
	cocos2d::extension::SceneReader::sharedSceneReader()->setTarget(this, callfuncOD_selector(HelloWorld::comCallBack));
	CCNode *pNode = cocos2d::extension::SceneReader::sharedSceneReader()->createNodeWithSceneFile(ReadJSHelper::Instance()->GetJsonPath().c_str());
	if (pNode != NULL)
	{
		this->addChild(pNode);
	}
	sendEvent(TRIGGEREVENT_INITSCENE);
	this->schedule(schedule_selector(HelloWorld::gameLogic));
	this->setTouchEnabled(true);
	this->setTouchMode(ccTouchesMode::kCCTouchesOneByOne);
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

void HelloWorld::setRandomSeed()
{
	cc_timeval psv;    
	CCTime::gettimeofdayCocos2d(&psv, NULL);    
	unsigned long int rand_seed = psv.tv_sec*1000 + psv.tv_usec/1000;    
	srand(rand_seed); 
}