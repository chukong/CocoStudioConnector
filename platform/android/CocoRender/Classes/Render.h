#ifndef __RENDER_H__
#define __RENDER_H__

#include "cocos2d.h"

class Render : public cocos2d::CCLayer
{
public:
	Render(const char* filename);
	~Render();

private:
	void initSceneLayer(const char* filename);
	cocos2d::CCString* name;
};

#endif 
