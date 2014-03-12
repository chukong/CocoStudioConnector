
#include "cons.h"

using namespace cocos2d;
using namespace cocos2d::extension;


IMPLEMENT_CLASS_INFO(TimeElapsed)

TimeElapsed::TimeElapsed(void)
:_fTotalTime(0.0f)
,_fTmpTime(0.0f)
,_pScheduler(NULL)
,_bSuc(false)
{
	_pScheduler = CCDirector::sharedDirector()->getScheduler();
	_pScheduler->retain();
}

TimeElapsed::~TimeElapsed(void)
{
	CC_SAFE_RELEASE(_pScheduler);
}

bool TimeElapsed::init()
{
	_pScheduler->scheduleSelector(schedule_selector(TimeElapsed::update), this, 0.0f , kCCRepeatForever, 0.0f, false);
    return true;
}

bool TimeElapsed::detect()
{
    return _bSuc;
}

void TimeElapsed::serialize(const rapidjson::Value &val)
{
    int count = DICTOOL->getArrayCount_json(val, "dataitems");
	for (int i = 0; i < count; ++i)
	{
		const rapidjson::Value &subDict = DICTOOL->getSubDictionary_json(val, "dataitems", i);
		std::string key = DICTOOL->getStringValue_json(subDict, "key");
		if (key == "TotalTime")
		{
			_fTotalTime = DICTOOL->getFloatValue_json(subDict, "value");
		}
	}
}

void TimeElapsed::removeAll()
{
	_pScheduler->unscheduleUpdateForTarget(this);
}

void TimeElapsed::update(float dt)
{
	_fTmpTime += dt;
	if (_fTmpTime > _fTotalTime)
	{
		_fTmpTime = 0.0f;
		_bSuc = true;
	}
}

IMPLEMENT_CLASS_INFO(ArmatureActionState)
ArmatureActionState::ArmatureActionState(void)
: _nTag(-1)
, _nState(-1)
, _bSuc(false)
{
}

ArmatureActionState::~ArmatureActionState(void)
{
}

bool ArmatureActionState::init()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		CCComRender *pRender = (CCComRender*)(pNode->getComponent(_comName.c_str()));
		CC_BREAK_IF(pRender == NULL);
		CCArmature *pAr = (CCArmature *)(pRender->getNode());
		CC_BREAK_IF(pAr == NULL);
		TriggerMng::getInstance()->addArmatureMovementCallBack(pAr, this, movementEvent_selector(ArmatureActionState::animationEvent));
	} while (0);
	
	return true;
}

bool ArmatureActionState::detect()
{
    return _bSuc;
}

void ArmatureActionState::serialize(const rapidjson::Value &val)
{
	int count = DICTOOL->getArrayCount_json(val, "dataitems");
	for (int i = 0; i < count; ++i)
	{
		const rapidjson::Value &subDict = DICTOOL->getSubDictionary_json(val, "dataitems", i);
		std::string key = DICTOOL->getStringValue_json(subDict, "key");
		if (key == "Tag")
		{
			_nTag = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "componentName")
		{
			_comName = DICTOOL->getStringValue_json(subDict, "value");
			continue;
		}
		else if (key == "AnimationName")
		{
			_aniname = DICTOOL->getStringValue_json(subDict, "value");
			continue;
		}
		else if (key == "ActionType")
		{
			_nState = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
	}
}

void ArmatureActionState::removeAll()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		CCComRender *pRender = (CCComRender*)(pNode->getComponent(_comName.c_str()));
		CC_BREAK_IF(pRender == NULL);
		CCArmature *pAr = (CCArmature *)(pRender->getNode());
		CC_BREAK_IF(pAr == NULL);
		TriggerMng::getInstance()->removeArmatureMovementCallBack(pAr, this, movementEvent_selector(ArmatureActionState::animationEvent));
	} while (0);
}

void ArmatureActionState::animationEvent(cocos2d::extension::CCArmature *armature, cocos2d::extension::MovementEventType movementType, const char *movementID)
{
	 std::string id = movementID;
	if (movementType == _nState && id.compare(_aniname) == 0)
	{
		_bSuc = true;
	}
}

IMPLEMENT_CLASS_INFO(NodeInRect)
NodeInRect::NodeInRect(void)
:_nTag(-1)
{
}

NodeInRect::~NodeInRect(void)
{
}

bool NodeInRect::init()
{

    return true;
}

bool NodeInRect::detect()
{
	CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
	if (pNode != NULL && abs(pNode->getPositionX() - _origin.x) <= _size.width && abs(pNode->getPositionY() - _origin.y) <= _size.height)
	{
		return true;
	}
    return false;
}

void NodeInRect::serialize(const rapidjson::Value &val)
{
	int count = DICTOOL->getArrayCount_json(val, "dataitems");
	for (int i = 0; i < count; ++i)
	{
		const rapidjson::Value &subDict = DICTOOL->getSubDictionary_json(val, "dataitems", i);
		std::string key = DICTOOL->getStringValue_json(subDict, "key");
		if (key == "Tag")
		{
			_nTag = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "originX")
		{
			_origin.x = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "originY")
		{
			_origin.y = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "sizeWidth")
		{
			_size.width = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "sizeHeight")
		{
			_size.height = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
	}
}

void NodeInRect::removeAll()
{
	CCLOG("NodeInRect::removeAll");
}


IMPLEMENT_CLASS_INFO(NodeVisible)
NodeVisible::NodeVisible(void)
: _nTag(-1)
, _bVisible(false)
{
}

NodeVisible::~NodeVisible(void)
{
}

bool NodeVisible::init()
{
	return true;
}

bool NodeVisible::detect()
{
	CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
	if (pNode != NULL && pNode->isVisible() == _bVisible)
	{
		return true;
	}
	return false;
}

void NodeVisible::serialize(const rapidjson::Value &val)
{
	int count = DICTOOL->getArrayCount_json(val, "dataitems");
	for (int i = 0; i < count; ++i)
	{
		const rapidjson::Value &subDict = DICTOOL->getSubDictionary_json(val, "dataitems", i);
		std::string key = DICTOOL->getStringValue_json(subDict, "key");
		if (key == "Tag")
		{
			_nTag = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "Visible")
		{
			_bVisible = DICTOOL->getIntValue_json(subDict, "value") != 0? true:false;
			continue;
		}
	}
}

void NodeVisible::removeAll()
{
	CCLOG("NodeVisible::removeAll");
}


IMPLEMENT_CLASS_INFO(RectangleCollisionTest)
	RectangleCollisionTest::RectangleCollisionTest(void)
{
}

RectangleCollisionTest::~RectangleCollisionTest(void)
{
}

bool RectangleCollisionTest::init()
{
	return true;
}

bool RectangleCollisionTest::detect()
{
	do 
	{
		CCNode *pNodeA = SceneReader::sharedSceneReader()->getNodeByTag(_nTag_A);
		CC_BREAK_IF(pNodeA == NULL);
		CCNode *pComNodeA = getNode(pNodeA, _strComName_A);
		CC_BREAK_IF(pNodeA == NULL);
		for (std::vector<int>::iterator iter = _vecTags.begin(); iter != _vecTags.end(); ++iter)
		{
			CCNode *pNodeB = SceneReader::sharedSceneReader()->getNodeByTag(*iter);
			CC_BREAK_IF(pNodeB == NULL);
			CCNode *pComNodeB = getNode(pNodeB, _strComName_B);
			CC_BREAK_IF(pNodeA == NULL);
			CCPoint p1 = pNodeA->getPosition();
			CCPoint p2 = pNodeB->getParent()->getPosition() + pNodeB->getPosition();
			CCRect ARect(p1.x, p1.y, pComNodeA->getContentSize().width + _nAOffsetX, pComNodeA->getContentSize().height + _nAOffsetY);
			CCRect BRect(p2.x, p2.y, pComNodeB->getContentSize().width + _nBOffsetX, pComNodeB->getContentSize().height + _nBOffsetY);
			if (isRectCollision(ARect, BRect))
			{
				CCLOG("peng!!!");
				return true;
			}
		}
		
	} while (0);
	
	return false;
}

void RectangleCollisionTest::serialize(const rapidjson::Value &val)
{
	int count = DICTOOL->getArrayCount_json(val, "dataitems");
	for (int i = 0; i < count; ++i)
	{
		const rapidjson::Value &subDict = DICTOOL->getSubDictionary_json(val, "dataitems", i);
		std::string key = DICTOOL->getStringValue_json(subDict, "key");
		if (key == "Tag_A")
		{
			_nTag_A = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "ComName_A")
		{
			_strComName_A = DICTOOL->getStringValue_json(subDict, "value");
			continue;
		}
		if (key == "AOffsetX")
		{
			_nAOffsetX = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "AOffsetY")
		{
			_nAOffsetY = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "Tags")
		{
			std::string strTags = DICTOOL->getStringValue_json(subDict, "value");
			split(strTags, ",", &_vecTags);
			continue;
		}
		else if (key == "ComName_B")
		{
			_strComName_B = DICTOOL->getStringValue_json(subDict, "value");
			continue;
		}
		if (key == "BOffsetX")
		{
			_nBOffsetX = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "BOffsetY")
		{
			_nBOffsetY = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
	}
}

void RectangleCollisionTest::removeAll()
{
	CCLOG("RectangleCollisionTest::removeAll");
}

bool RectangleCollisionTest::isRectCollision(CCRect rect1, CCRect rect2)
{
	float x1 = rect1.origin.x;
	float y1 = rect1.origin.y;
	float w1 = rect1.size.width;
	float h1 = rect1.size.height;
	float x2 = rect2.origin.x;
	float y2 = rect2.origin.y;
	float w2 = rect2.size.width;
	float h2 = rect2.size.height;

	if (fabs(x1 - x2) > (w1 * 0.5 + w2 * 0.5) || fabs(y1 - y2) > (h1 * 0.5 + h2 * 0.5))
	{
		return false;
	}

	return true;
}

cocos2d::CCNode* RectangleCollisionTest::getNode(cocos2d::CCNode *pNode, std::string comName)
{
	do 
	{
		CC_BREAK_IF(pNode == NULL);
		CCComponent *com = pNode->getComponent(comName.c_str());
		CC_BREAK_IF(com == NULL);
		CCComRender *render = dynamic_cast<CCComRender*>(com);
		CC_BREAK_IF(render == NULL);
		return render->getNode();
	} while (0);
	return NULL;
}

void RectangleCollisionTest::split(const std::string& s,const std::string& delim,std::vector<int>* ret)
{
	size_t last = 0;
	size_t index=s.find_first_of(delim,last);
	while (index!=std::string::npos)
	{
		ret->push_back(atoi(s.substr(last,index-last).c_str()));
		last=index+1;
		index=s.find_first_of(delim,last);
	}
	if (index-last>0)
	{
		ret->push_back(atoi(s.substr(last,index-last).c_str()));
	}
}


