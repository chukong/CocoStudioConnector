
#include "acts.h"

using namespace cocos2d;
using namespace cocos2d::extension;


IMPLEMENT_CLASS_INFO(PlayMusic)
PlayMusic::PlayMusic(void)
:_nTag(-1)
{
}

PlayMusic::~PlayMusic(void)
{
}

bool PlayMusic::init()
{
    return true;
}

void PlayMusic::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		CCComAudio *audio = (CCComAudio*)(pNode->getComponent(_comName.c_str()));
		CC_BREAK_IF(audio == NULL);
		if (_nType == 0)
		{
			audio->playBackgroundMusic();
		}
		else if (_nType == 1)
		{
			audio->playEffect();
		}

	} while (0);
}

void PlayMusic::serialize(const rapidjson::Value &val)
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
		else if (key == "type")
		{
			_nType = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
	}
}

void PlayMusic::removeAll()
{
}

IMPLEMENT_CLASS_INFO(TMoveTo)
TMoveTo::TMoveTo(void)
:_nTag(-1)
,_fDuration(0.0f)
{
}

TMoveTo::~TMoveTo(void)
{
}

bool TMoveTo::init()
{
	return true;
}

void TMoveTo::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		CCActionInterval*  actionTo = CCMoveTo::create(_fDuration, _pos);
		CC_BREAK_IF(actionTo == NULL);
		pNode->runAction(actionTo);
	} while (0);
}

void TMoveTo::serialize(const rapidjson::Value &val)
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
		else if (key == "Duration")
		{
			_fDuration = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "x")
		{
			_pos.x = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "y")
		{
			_pos.y = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
	}
}

void TMoveTo::removeAll()
{
	CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
	pNode->getActionManager()->removeAllActions();
}

IMPLEMENT_CLASS_INFO(TMoveBy)
TMoveBy::TMoveBy(void)
:_nTag(-1)
,_fDuration(0.0f)
,_bReverse(false)
{
}

TMoveBy::~TMoveBy(void)
{
}

bool TMoveBy::init()
{
    return true;
}

void TMoveBy::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		CCActionInterval*  actionBy = CCMoveBy::create(_fDuration, _pos);
		CC_BREAK_IF(actionBy == NULL);
		if (_bReverse == true)
		{
			CCActionInterval*  actionByBack = actionBy->reverse();
			if (_bIsRepeatForever)
			{
				pNode->runAction(CCRepeatForever::create(static_cast<CCSequence *>(CCSequence::create(actionBy, actionByBack,NULL))));  
			}
			else
			{
				pNode->runAction( CCSequence::create(actionBy, actionByBack, NULL));
			}
		}
		else
		{
			pNode->runAction(actionBy);
		}
	} while (0);
}

void TMoveBy::serialize(const rapidjson::Value &val)
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
		else if (key == "Duration")
		{
			_fDuration = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "x")
		{
			_pos.x = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "y")
		{
			_pos.y = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "IsReverse")
		{
			_bReverse = DICTOOL->getIntValue_json(subDict, "value") != 0? true:false;
			continue;
		}
		else if (key == "IsRepeatForever")
		{
			_bIsRepeatForever = (DICTOOL->getIntValue_json(subDict, "value") == 1);
			continue;
		}
	}
}

void TMoveBy::removeAll()
{
	CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
	pNode->getActionManager()->removeAllActions();
}



IMPLEMENT_CLASS_INFO(TRotateTo)
TRotateTo::TRotateTo(void)
: _nTag(-1)
, _fDuration(0.0f)
, _fDeltaAngle(0.0f)
{
}

TRotateTo::~TRotateTo(void)
{
}

bool TRotateTo::init()
{
    return true;
}

void TRotateTo::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		CCActionInterval*  actionTo = CCRotateTo::create(_fDuration, _fDeltaAngle);
		CC_BREAK_IF(actionTo == NULL);
		pNode->runAction(actionTo);
	} while (0);
}

void TRotateTo::serialize(const rapidjson::Value &val)
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
		else if (key == "Duration")
		{
			_fDuration = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "DeltaAngle")
		{
			_fDeltaAngle = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
	}
}

void TRotateTo::removeAll()
{
	CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
	pNode->getActionManager()->removeAllActions();
}



IMPLEMENT_CLASS_INFO(TRotateBy)
TRotateBy::TRotateBy(void)
: _nTag(-1)
, _fDuration(0.0f)
, _fDeltaAngle(0.0f)
, _bReverse(false)
{
}

TRotateBy::~TRotateBy(void)
{
}

bool TRotateBy::init()
{
    return true;
}

void TRotateBy::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		CCActionInterval*  actionBy = CCRotateBy::create(_fDuration, _fDeltaAngle);
		CC_BREAK_IF(actionBy == NULL);
		if (_bReverse == true)
		{
			CCActionInterval*  actionByBack = actionBy->reverse();
			pNode->runAction( CCSequence::create(actionBy, actionByBack, NULL));
		}
		else
		{
			pNode->runAction(actionBy);
		}
	} while (0);
}

void TRotateBy::serialize(const rapidjson::Value &val)
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
		else if (key == "Duration")
		{
			_fDuration = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "DeltaAngle")
		{
			_fDeltaAngle = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "IsReverse")
		{
			_bReverse = DICTOOL->getIntValue_json(subDict, "value")!= 0? true:false;
			continue;
		}
	}
}

void TRotateBy::removeAll()
{
	CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
	pNode->getActionManager()->removeAllActions();
}



IMPLEMENT_CLASS_INFO(TScaleTo)
TScaleTo::TScaleTo(void)
: _nTag(-1)
, _fDuration(0.0f)
{
}

TScaleTo::~TScaleTo(void)
{

}

bool TScaleTo::init()
{
    return true;
}

void TScaleTo::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		CCActionInterval*  actionTo = CCScaleTo::create(_fDuration, _scale.x, _scale.y);
		CC_BREAK_IF(actionTo == NULL);
		pNode->runAction(actionTo);
	} while (0);
}

void TScaleTo::serialize(const rapidjson::Value &val)
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
		else if (key == "Duration")
		{
			_fDuration = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "ScaleX")
		{
			_scale.x = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "ScaleY")
		{
			_scale.y = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
	}
}

void TScaleTo::removeAll()
{
	CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
	pNode->getActionManager()->removeAllActions();
}



IMPLEMENT_CLASS_INFO(TScaleBy)
TScaleBy::TScaleBy(void)
: _nTag(-1)
, _fDuration(0.0f)
, _bReverse(false)
{
}

TScaleBy::~TScaleBy(void)
{
}

bool TScaleBy::init()
{
    return true;
}

void TScaleBy::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		CCActionInterval*  actionBy = CCScaleBy::create(_fDuration, _scale.x, _scale.y);
		CC_BREAK_IF(actionBy == NULL);
		if (_bReverse == true)
		{
			CCActionInterval*  actionByBack = actionBy->reverse();
			pNode->runAction( CCSequence::create(actionBy, actionByBack, NULL));
		}
		else
		{
			pNode->runAction(actionBy);
		}
	} while (0);
}

void TScaleBy::serialize(const rapidjson::Value &val)
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
		else if (key == "Duration")
		{
			_fDuration = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "ScaleX")
		{
			_scale.x = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "ScaleY")
		{
			_scale.y = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "IsReverse")
		{
			_bReverse = DICTOOL->getIntValue_json(subDict, "value")!= 0? true:false;
			continue;
		}
	}
}

void TScaleBy::removeAll()
{
	CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
	pNode->getActionManager()->removeAllActions();
}



IMPLEMENT_CLASS_INFO(TSkewTo)
TSkewTo::TSkewTo(void)
: _nTag(-1)
, _fDuration(0.0f)
{
}

TSkewTo::~TSkewTo(void)
{
}

bool TSkewTo::init()
{
    return true;
}

void TSkewTo::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		CCActionInterval*  actionTo = CCSkewTo::create(_fDuration, _skew.x, _skew.y);
		CC_BREAK_IF(actionTo == NULL);
		pNode->runAction(actionTo);
	} while (0);
}

void TSkewTo::serialize(const rapidjson::Value &val)
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
		else if (key == "Duration")
		{
			_fDuration = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "SkewX")
		{
			_skew.x = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "SkewY")
		{
			_skew.y = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
	}
}

void TSkewTo::removeAll()
{
	CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
	pNode->getActionManager()->removeAllActions();
}



IMPLEMENT_CLASS_INFO(TSkewBy)
TSkewBy::TSkewBy(void)
: _nTag(-1)
, _fDuration(0.0f)
, _bReverse(false)
{
}

TSkewBy::~TSkewBy(void)
{
}

bool TSkewBy::init()
{
    return true;
}

void TSkewBy::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		CCActionInterval*  actionBy = CCSkewBy::create(_fDuration, _skew.x, _skew.y);
		CC_BREAK_IF(actionBy == NULL);
		if (_bReverse == true)
		{
			CCActionInterval*  actionByBack = actionBy->reverse();
			pNode->runAction( CCSequence::create(actionBy, actionByBack, NULL));
		}
		else
		{
			pNode->runAction(actionBy);
		}
	} while (0);
}

void TSkewBy::serialize(const rapidjson::Value &val)
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
		else if (key == "Duration")
		{
			_fDuration = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "SKewX")
		{
			_skew.x = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "SKewY")
		{
			_skew.y = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "IsReverse")
		{
			_bReverse = DICTOOL->getIntValue_json(subDict, "value")!= 0? true:false;
		}
	}
}

void TSkewBy::removeAll()
{
	CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
	pNode->getActionManager()->removeAllActions();
}



IMPLEMENT_CLASS_INFO(TriggerState)
TriggerState::TriggerState(void)
:_nID(-1)
,_nState(0)
{
}

TriggerState::~TriggerState(void)
{
}

bool TriggerState::init()
{
    return true;
}

void TriggerState::done()
{
    TriggerObj *obj = TriggerMng::getInstance()->getTriggerObj(_nID);
	if (obj != NULL)
	{
		if (_nState == 0)
		{
			obj->setEnable(false);
		}
		else if (_nState == 1)
		{
			obj->setEnable(true);
		}
		else if (_nState == 2)
		{
			TriggerMng::getInstance()->removeTriggerObj(_nID);
		}
		
	}
	
}

void TriggerState::serialize(const rapidjson::Value &val)
{
	int count = DICTOOL->getArrayCount_json(val, "dataitems");
	for (int i = 0; i < count; ++i)
	{
		const rapidjson::Value &subDict = DICTOOL->getSubDictionary_json(val, "dataitems", i);
		std::string key = DICTOOL->getStringValue_json(subDict, "key");
		if (key == "ID")
		{
			_nID = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "State")
		{
			_nState = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
	}
}

void TriggerState::removeAll()
{
	CCLOG("TriggerState::removeAll");
}

IMPLEMENT_CLASS_INFO(ArmaturePlayAction)
ArmaturePlayAction::ArmaturePlayAction(void)
: _nTag(-1)
{
}

ArmaturePlayAction::~ArmaturePlayAction(void)
{
}

bool ArmaturePlayAction::init()
{
	return true;
}

void ArmaturePlayAction::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		CCComRender *pRender = (CCComRender*)(pNode->getComponent(_ComName.c_str()));
		CC_BREAK_IF(pRender == NULL);
		CCArmature *pAr = (CCArmature *)(pRender->getNode());
		CC_BREAK_IF(pAr == NULL);
		pAr->getAnimation()->play(_aniname.c_str());
	} while (0);
}

void ArmaturePlayAction::serialize(const rapidjson::Value &val)
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
			_ComName = DICTOOL->getStringValue_json(subDict, "value");
			continue;
		}
		else if (key == "AnimationName")
		{
			_aniname = DICTOOL->getStringValue_json(subDict, "value");
			continue;
		}
	}
}

void ArmaturePlayAction::removeAll()
{
	CCLOG("ArmaturePlayAction::removeAll");
}



IMPLEMENT_CLASS_INFO(SequenceMoveTo)
SequenceMoveTo::SequenceMoveTo(void)
{

}

SequenceMoveTo::~SequenceMoveTo(void)
{
}

bool SequenceMoveTo::init()
{
    return true;
}

void SequenceMoveTo::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		CCMoveTo* pEndAction = CCMoveTo::create(_fEndDur, _ccpEndPos);
		CCMoveTo* pStartAction = CCMoveTo::create(_fStartDur, _ccpStartPos);

		CCSequence* action = NULL;
		if (_fDelayTime < 0)
		{
			action = CCSequence::create(pEndAction, pStartAction, NULL);
		}
		else
		{
			action = CCSequence::create(CCDelayTime::create(_fDelayTime), pEndAction, pStartAction, NULL);
		} 
		if (!_bIsRepeatForever)
		{
			pNode->runAction(action);
		}
		else
		{
			pNode->runAction(CCRepeatForever::create(action));
		}
	} while (0);
}

void SequenceMoveTo::serialize(const rapidjson::Value &val)
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
		else if (key == "EndX")
		{
			_ccpEndPos.x = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "EndY")
		{
			_ccpEndPos.y = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "EndDuration")
		{
			_fEndDur = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "StartX")
		{
			_ccpStartPos.x = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "StartY")
		{
			_ccpStartPos.y = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "StartDuration")
		{
			_fStartDur = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "DelayTime")
		{
			_fDelayTime = DICTOOL->getFloatValue_json(subDict, "value");
		}
		else if (key == "IsRepeatForever")
		{
			_bIsRepeatForever = (DICTOOL->getIntValue_json(subDict, "value") == 1);
			continue;
		}
	}
}

void SequenceMoveTo::removeAll()
{
	CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
	pNode->getActionManager()->removeAllActions();
}


IMPLEMENT_CLASS_INFO(JumpAction)
JumpAction::JumpAction(void)
{
}

JumpAction::~JumpAction(void)
{
}

bool JumpAction::init()
{
	return true;
}

void JumpAction::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		pNode->setRotation(_fRotationTo);
		CCMoveTo *pAction = CCMoveTo::create(_fRiseTime, ccp(pNode->getPositionX(), pNode->getPositionY() + _fRiseHeight)); 
		pNode->stopAllActions();  
		pNode->runAction(CCSpawn::create(  
			CCSequence::create(pAction, CCDelayTime::create(_fJumpDelayTime), NULL), NULL)  
			);
	} while (0);
}

void JumpAction::serialize(const rapidjson::Value &val)
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
		else if (key == "RiseHeight")
		{
			_fRiseHeight = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "RiseTime")
		{
			_fRiseTime = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "JumpDelayTime")
		{
			_fJumpDelayTime = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "RotationTo")
		{
			_fRotationTo = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
	}
}

void JumpAction::removeAll()
{
	CCLOG("JumpAction::removeAll");
}



IMPLEMENT_CLASS_INFO(FallAction)
FallAction::FallAction(void)
{
}

FallAction::~FallAction(void)
{
}

bool FallAction::init()
{
	return true;
}

void FallAction::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		pNode->stopAllActions();    
		float postionX = pNode->getPositionX();  
		float postionY = pNode->getPositionY();  
		float time = postionY / _fFallTimeParam;  
		pNode->runAction(CCSequence::create( 
			CCSpawn::create(CCRotateTo::create(time, _fRotationTo), CCMoveTo::create(time, ccp(postionX, _fFallTo)), NULL), NULL)  
			);  

	} while (0);
}

void FallAction::serialize(const rapidjson::Value &val)
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
		else if (key == "FallTimeParam")
		{
			_fFallTimeParam = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "RotationTo")
		{
			_fRotationTo = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "FallTo")
		{
			_fFallTo = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
	}
}

void FallAction::removeAll()
{
	CCLOG("FallAction::removeAll");
}



IMPLEMENT_CLASS_INFO(ChangeDoubleAttribute)
ChangeDoubleAttribute::ChangeDoubleAttribute(void)
{
}

ChangeDoubleAttribute::~ChangeDoubleAttribute(void)
{
}

bool ChangeDoubleAttribute::init()
{
	return true;
}

void ChangeDoubleAttribute::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		CCComAttribute *attri = static_cast<CCComAttribute*>(pNode->getComponent("CCComAttribute"));
		float value = attri->getFloat(_Key.c_str());
		if (_nType == 0)
		{
			value -= _fValue;

		}
		else if (_nType == 1)
		{
			value = _fValue;
		}
		else
		{
			value += _fValue;
		}

		if (_nLimit == 0)
		{
			if (value < 0)
			{
				value = 0;
			}
		}
		else if (_nLimit == 1)
		{
			if (value > 0)
			{
				value = 0;
			}
		}
		
		attri->setFloat("UpSpeed", value);

	} while (0);
}

void ChangeDoubleAttribute::serialize(const rapidjson::Value &val)
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
		else if (key == "Key")
		{
			_Key = DICTOOL->getStringValue_json(subDict, "value");
			continue;
		}
		else if (key == "Value")
		{
			_fValue = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "ChangeType")
		{
			_nType = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "Limit")
		{
			_nLimit = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
	}
}

void ChangeDoubleAttribute::removeAll()
{
	CCLOG("ChangeDoubleAttribute::removeAll");
}



IMPLEMENT_CLASS_INFO(AddAccToPositionY)
AddAccToPositionY::AddAccToPositionY(void)
{
}

AddAccToPositionY::~AddAccToPositionY(void)
{
}

bool AddAccToPositionY::init()
{
	return true;
}

void AddAccToPositionY::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		CCComAttribute *attri = dynamic_cast<CCComAttribute*>(pNode->getComponent(_comName.c_str()));
		CC_BREAK_IF(attri == NULL);
		float value = attri->getFloat(_key.c_str());
		attri->setFloat(_key.c_str(), value + _fAcceleration * CCDirector::sharedDirector()->getDeltaTime());
		value = pNode->getPositionY() - attri->getFloat(_key.c_str());

		float t = attri->getFloat("Acceleration");
		pNode->setPositionY(value);
	} while (0);
}

void AddAccToPositionY::serialize(const rapidjson::Value &val)
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
		else if (key == "ComName")
		{
			_comName = DICTOOL->getStringValue_json(subDict, "value");
			continue;
		}
		else if (key == "Key")
		{
			_key = DICTOOL->getStringValue_json(subDict, "value");
			continue;
		}
		else if (key == "Acceleration")
		{
			_fAcceleration = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
	}

}

void AddAccToPositionY::removeAll()
{
	CCLOG("AddAccToPositionY::removeAll");
}


IMPLEMENT_CLASS_INFO(SequenceMoveToAndChangePositionY)
SequenceMoveToAndChangePositionY::SequenceMoveToAndChangePositionY(void)
{
}

SequenceMoveToAndChangePositionY::~SequenceMoveToAndChangePositionY(void)
{
}

bool SequenceMoveToAndChangePositionY::init()
{
	return true;
}

void SequenceMoveToAndChangePositionY::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		int y = getRandNum(_nMinPositionY, _nMaxPositionY);
		pNode->setPositionY(y);
		_ccpEndPos.y = y;
		CCMoveTo* pEndAction = CCMoveTo::create(_fEndDur, _ccpEndPos);
		_ccpStartPos.y = y;
		CCMoveTo* pStartAction = CCMoveTo::create(_fStartDur, _ccpStartPos);
		CCAction* pAction = NULL;
		
		if (_fDelayTime < 0)
		{
			pAction = CCSequence::create(pEndAction, pStartAction, CCCallFuncND::create(this, callfuncND_selector(SequenceMoveToAndChangePositionY::actionOver), pNode), NULL);  
		}
		else
		{
			pAction = CCSequence::create(CCDelayTime::create(_fDelayTime), pEndAction, pStartAction, CCCallFuncND::create(this, callfuncND_selector(SequenceMoveToAndChangePositionY::actionOver), pNode), NULL);  
		} 

		pNode->runAction(pAction);
	} while (0);
}

void SequenceMoveToAndChangePositionY::serialize(const rapidjson::Value &val)
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
		else if (key == "EndX")
		{
			_ccpEndPos.x = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "EndDuration")
		{
			_fEndDur = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "StartX")
		{
			_ccpStartPos.x = DICTOOL->getIntValue_json(subDict, "value");
			continue;
		}
		else if (key == "StartDuration")
		{
			_fStartDur = DICTOOL->getFloatValue_json(subDict, "value");
			continue;
		}
		else if (key == "DelayTime")
		{
			_fDelayTime = DICTOOL->getFloatValue_json(subDict, "value");
		}
		else if (key == "MaxPositonY")
		{
			_nMaxPositionY =  DICTOOL->getIntValue_json(subDict, "value");
		}
		else if (key == "MinPositonY")
		{
			_nMinPositionY =  DICTOOL->getIntValue_json(subDict, "value");
		}
	}
}

void SequenceMoveToAndChangePositionY::removeAll()
{
	CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
	pNode->getActionManager()->removeAllActions();
}

int SequenceMoveToAndChangePositionY::getRandNum(int min, int max)
{
	float i = CCRANDOM_0_1()*(max - min + 1) + min; 
	return (int)i; 
}


void SequenceMoveToAndChangePositionY::actionOver(CCNode* pSender, void* data)
{
	CCNode *pNode = (CCNode*)data;
	CCSequence *pAction = NULL;

	int y = getRandNum(_nMinPositionY, _nMaxPositionY);
	pNode->setPositionY(y);
	_ccpEndPos.y = y;
	CCMoveTo* pEndAction = CCMoveTo::create(_fEndDur, _ccpEndPos);
	_ccpStartPos.y = y;
	CCMoveTo* pStartAction = CCMoveTo::create(_fStartDur, _ccpStartPos);
	pAction = CCSequence::create(pEndAction, pStartAction, CCCallFuncND::create(this, callfuncND_selector(SequenceMoveToAndChangePositionY::actionOver), pNode), NULL);  
	pNode->runAction(pAction);

	return;
}

IMPLEMENT_CLASS_INFO(StopAllActions)
StopAllActions::StopAllActions(void)
{
}

StopAllActions::~StopAllActions(void)
{
}

bool StopAllActions::init()
{
	return true;
}

void StopAllActions::done()
{
	do 
	{
		for (std::vector<int>::iterator iter = _vecTags.begin(); iter != _vecTags.end(); ++iter)
		{
			CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(*iter);
			if (pNode == NULL)
			{
				continue;;
			}
			pNode->stopAllActions();
		}
	} while (0);
}

void StopAllActions::serialize(const rapidjson::Value &val)
{
	int count = DICTOOL->getArrayCount_json(val, "dataitems");
	for (int i = 0; i < count; ++i)
	{
		const rapidjson::Value &subDict = DICTOOL->getSubDictionary_json(val, "dataitems", i);
		std::string key = DICTOOL->getStringValue_json(subDict, "key");
		if (key == "Tags")
		{
			std::string strTags = DICTOOL->getStringValue_json(subDict, "value");
			split(strTags, ",", &_vecTags);
			continue;
		}
	}
}

void StopAllActions::removeAll()
{
	CCLOG("StopAllActions::removeAll");
}

void StopAllActions::split(const std::string& s,const std::string& delim,std::vector<int>* ret)
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

IMPLEMENT_CLASS_INFO(SetNodeVisible)
SetNodeVisible::SetNodeVisible(void)
{
}

SetNodeVisible::~SetNodeVisible(void)
{
}

bool SetNodeVisible::init()
{
	return true;
}

void SetNodeVisible::done()
{
	do 
	{
		CCNode *pNode = SceneReader::sharedSceneReader()->getNodeByTag(_nTag);
		CC_BREAK_IF(pNode == NULL);
		pNode->setVisible(_bShow);
	} while (0);
}

void SetNodeVisible::serialize(const rapidjson::Value &val)
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
			_bShow = DICTOOL->getIntValue_json(subDict, "value") == 1;
			continue;
		}
	}
}

void SetNodeVisible::removeAll()
{
	CCLOG("SetNodeVisible::removeAll");
}



IMPLEMENT_CLASS_INFO(PlayUIAnimation)
PlayUIAnimation::PlayUIAnimation(void)
{
}

PlayUIAnimation::~PlayUIAnimation(void)
{
}

bool PlayUIAnimation::init()
{
	return true;
}

void PlayUIAnimation::done()
{
	ActionManager::shareManager()->playActionByName(_uiJsonName.c_str(), _animaitionName.c_str());
}

void PlayUIAnimation::serialize(const rapidjson::Value &val)
{
	int count = DICTOOL->getArrayCount_json(val, "dataitems");
	for (int i = 0; i < count; ++i)
	{
		const rapidjson::Value &subDict = DICTOOL->getSubDictionary_json(val, "dataitems", i);
		std::string key = DICTOOL->getStringValue_json(subDict, "key");
		if (key == "UIJsonName")
		{
			_uiJsonName = DICTOOL->getStringValue_json(subDict, "value");
			continue;
		}
		else if (key == "AnimationName")
		{
			_animaitionName = DICTOOL->getStringValue_json(subDict, "value");
			continue;
		}
	}
}

void PlayUIAnimation::removeAll()
{
	CCLOG("PlayUIAnimation::removeAll");
}





