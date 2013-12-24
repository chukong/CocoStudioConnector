#ifndef __SCENE_RENDER_H__
#define __SCENE_RENDER_H__

#include "cocos2d.h"

class SceneRender : public cocos2d::CCLayer
{
public:
	SceneRender(const char* filename);
	~SceneRender();
//	CREATE_FUNC(SceneRender)

	void menuCloseCallback(CCObject* pSender);
	
	
    virtual void onEnter();
    virtual void onExit();
private:
	void initSceneLayer(const char* filename);
	cocos2d::CCString* name;
	CCNode* pScene;
	cocos2d::CCMenuItemImage *pCloseItem;
};

#endif __SCENE_RENDER_H__
