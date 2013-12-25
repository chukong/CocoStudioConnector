#ifndef __SCENE_RENDER_H__
#define __SCENE_RENDER_H__

#include "cocos2d.h"

class SceneRender : public cocos2d::CCLayer
{
public:
	SceneRender(const char* filename);
	~SceneRender();

	void menuCloseCallback(CCObject* pSender);
	
    virtual void onEnter();
    virtual void onExit();
    // CallBack
	void comCallBack(cocos2d::CCObject *tar, void *dict);

	// default implements are used to call script callback if exist
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	// gameLogic
	void gameLogic(float dt);
private:
	void initSceneLayer(const char* filename);
	cocos2d::CCString* name;
	CCNode* pScene;
	cocos2d::CCMenuItemImage *pCloseItem;
};

#endif
