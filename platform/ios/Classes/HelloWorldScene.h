#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ControlUI.h"
#include "UIWidget.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
		
	virtual void update(float delta);
	
	void Render(CCObject* sender, cocos2d::extension::TouchEventType type);
	
	virtual void onEnter();
    virtual void onExit();
	
	bool createDirectory(const char *path);
	bool uncompress();
	
	ControlUI *m_pGUI;
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
    /* pipu */
    long long getCurrentTime();    
    /**/
};

#endif // __HELLOWORLD_SCENE_H__
