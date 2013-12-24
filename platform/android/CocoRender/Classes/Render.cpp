#include "Render.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

Render::Render(const char* filename)
{
	name = CCString::create(filename);

	initSceneLayer(name->getCString());
}

Render::~Render(void)
{
}

void Render::initSceneLayer(const char* filename)
{
    CCLayer::init();
	CCNode* pScene = SceneReader::sharedSceneReader()->createNodeWithSceneFile(filename);
	this->addChild(pScene);
}
