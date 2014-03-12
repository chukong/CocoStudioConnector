#ifndef __ACTIONS_H__
#define __ACTIONS_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ExtensionMacros.h"


class  PlayMusic: public cocos2d::extension::BaseTriggerAction
{
    DECLARE_CLASS_INFO
public:
     PlayMusic(void);
     virtual ~PlayMusic(void);

     virtual bool init();
     virtual void done();
	 virtual void serialize(const rapidjson::Value &val);
     virtual void removeAll();
private:
	 int  _nTag;
	 std::string _comName;
	 int  _nType;
};

class  TMoveTo: public cocos2d::extension::BaseTriggerAction
{
	DECLARE_CLASS_INFO
public:
	TMoveTo(void);
	virtual ~TMoveTo(void);

	virtual bool init();
	virtual void done();
	virtual void serialize(const rapidjson::Value &val);
	virtual void removeAll();
private:
	int  _nTag;
	float _fDuration;
	cocos2d::CCPoint _pos;
};


class TMoveBy : public cocos2d::extension::BaseTriggerAction
{
    DECLARE_CLASS_INFO
public:
     TMoveBy(void);
     virtual ~TMoveBy(void);

     virtual bool init();
     virtual void done();
	 virtual void serialize(const rapidjson::Value &val);
     virtual void removeAll();
private:
	int  _nTag;
	float _fDuration;
	cocos2d::CCPoint _pos;
	bool _bReverse;
	bool _bIsRepeatForever;
};


class TRotateTo : public cocos2d::extension::BaseTriggerAction
{
    DECLARE_CLASS_INFO
public:
     TRotateTo(void);
     virtual ~TRotateTo(void);

     virtual bool init();
     virtual void done();
	 virtual void serialize(const rapidjson::Value &val);
     virtual void removeAll();
private:
	int  _nTag;
	float _fDuration;
	float _fDeltaAngle;
};


class TRotateBy : public cocos2d::extension::BaseTriggerAction
{
    DECLARE_CLASS_INFO
public:
     TRotateBy(void);
     virtual ~TRotateBy(void);

     virtual bool init();
     virtual void done();
	 virtual void serialize(const rapidjson::Value &val);
     virtual void removeAll();
private:
	int  _nTag;
	float _fDuration;
	float _fDeltaAngle;
	bool _bReverse;
};


class TScaleTo : public cocos2d::extension::BaseTriggerAction
{
    DECLARE_CLASS_INFO
public:
     TScaleTo(void);
     virtual ~TScaleTo(void);

     virtual bool init();
     virtual void done();
	 virtual void serialize(const rapidjson::Value &val);
     virtual void removeAll();
private:
	int  _nTag;
	float _fDuration;
	cocos2d::CCPoint _scale;
};


class TScaleBy : public cocos2d::extension::BaseTriggerAction
{
    DECLARE_CLASS_INFO
public:
     TScaleBy(void);
     virtual ~TScaleBy(void);

     virtual bool init();
     virtual void done();
	 virtual void serialize(const rapidjson::Value &val);
     virtual void removeAll();
private:
	int  _nTag;
	float _fDuration;
	cocos2d::CCPoint _scale;
	bool _bReverse;

};


class TSkewTo : public cocos2d::extension::BaseTriggerAction
{
    DECLARE_CLASS_INFO
public:
     TSkewTo(void);
     virtual ~TSkewTo(void);

     virtual bool init();
     virtual void done();
	 virtual void serialize(const rapidjson::Value &val);
     virtual void removeAll();
private:
	int  _nTag;
	float _fDuration;
	cocos2d::CCPoint _skew;
};


class TSkewBy : public cocos2d::extension::BaseTriggerAction
{
    DECLARE_CLASS_INFO
public:
     TSkewBy(void);
     virtual ~TSkewBy(void);

     virtual bool init();
     virtual void done();
	 virtual void serialize(const rapidjson::Value &val);
     virtual void removeAll();
private:
	int  _nTag;
	float _fDuration;
	cocos2d::CCPoint _skew;
	bool _bReverse;
};


class TriggerState : public cocos2d::extension::BaseTriggerAction
{
    DECLARE_CLASS_INFO
public:
     TriggerState(void);
     virtual ~TriggerState(void);

     virtual bool init();
     virtual void done();
	 virtual void serialize(const rapidjson::Value &val);
     virtual void removeAll();
private:
	int  _nID;
	int _nState;
};

class ArmaturePlayAction : public cocos2d::extension::BaseTriggerAction
{
	DECLARE_CLASS_INFO
public:
	ArmaturePlayAction(void);
	virtual ~ArmaturePlayAction(void);

	virtual bool init();
	virtual void done();
	virtual void serialize(const rapidjson::Value &val);
	virtual void removeAll();
private:
	int _nTag;
	std::string _ComName;
	std::string _aniname;
};


class SequenceMoveTo : public cocos2d::extension::BaseTriggerAction
{
    DECLARE_CLASS_INFO
public:
     SequenceMoveTo(void);
     virtual ~SequenceMoveTo(void);

     virtual bool init();
     virtual void done();
	 virtual void serialize(const rapidjson::Value &val);
     virtual void removeAll();
private:
	int _nTag;
	cocos2d::CCPoint _ccpEndPos;
	float  _fEndDur;
	cocos2d::CCPoint _ccpStartPos;
	float  _fStartDur;
	bool _bIsRepeatForever;
	float  _fDelayTime;
};

class JumpAction : public cocos2d::extension::BaseTriggerAction
{
	DECLARE_CLASS_INFO
public:
	JumpAction(void);
	virtual ~JumpAction(void);

	virtual bool init();
	virtual void done();
	virtual void serialize(const rapidjson::Value &val);
	virtual void removeAll();
private:
	int   _nTag;
	float _fRiseHeight;
	float _fRiseTime;
	float _fJumpDelayTime;
	float _fRotationTo;
};


class FallAction : public cocos2d::extension::BaseTriggerAction
{
	DECLARE_CLASS_INFO
public:
	FallAction(void);
	virtual ~FallAction(void);

	virtual bool init();
	virtual void done();
	virtual void serialize(const rapidjson::Value &val);
	virtual void removeAll();
private:
	int   _nTag;
	float _fFallTimeParam;
	float _fRotationTo;
	float _fFallTo;
};


class ChangeDoubleAttribute : public cocos2d::extension::BaseTriggerAction
{
	DECLARE_CLASS_INFO
public:
	ChangeDoubleAttribute(void);
	virtual ~ChangeDoubleAttribute(void);

	virtual bool init();
	virtual void done();
	virtual void serialize(const rapidjson::Value &val);
	virtual void removeAll();
private:
	int   _nTag;
	std::string _Key;
	float _fValue;
	int _nType;
	int _nLimit;
};


class AddAccToPositionY : public cocos2d::extension::BaseTriggerAction
{
	DECLARE_CLASS_INFO
public:
	AddAccToPositionY(void);
	virtual ~AddAccToPositionY(void);

	virtual bool init();
	virtual void done();
	virtual void serialize(const rapidjson::Value &val);
	virtual void removeAll();
private:
	int   _nTag;
	std::string _comName;
	std::string _key;
	float _fAcceleration;
};



class SequenceMoveToAndChangePositionY : public cocos2d::extension::BaseTriggerAction
{
	DECLARE_CLASS_INFO
public:
	SequenceMoveToAndChangePositionY(void);
	virtual ~SequenceMoveToAndChangePositionY(void);

	virtual bool init();
	virtual void done();
	virtual void serialize(const rapidjson::Value &val);
	virtual void removeAll();
private:
	int getRandNum(int min, int max);
	void actionOver(cocos2d::CCNode* pSender, void* data);
private:
	int _nTag;
	cocos2d::CCPoint _ccpEndPos;
	float  _fEndDur;
	cocos2d::CCPoint _ccpStartPos;
	float  _fStartDur;
	float  _fDelayTime;
	int    _nMaxPositionY;
	int    _nMinPositionY;
};

class StopAllActions : public cocos2d::extension::BaseTriggerAction
{
	DECLARE_CLASS_INFO
public:
	StopAllActions(void);
	virtual ~StopAllActions(void);

	virtual bool init();
	virtual void done();
	virtual void serialize(const rapidjson::Value &val);
	virtual void removeAll();
private:
	void split(const std::string& s,const std::string& delim,std::vector<int>* ret);
private:
	std::vector<int> _vecTags;
};


class SetNodeVisible : public cocos2d::extension::BaseTriggerAction
{
	DECLARE_CLASS_INFO
public:
	SetNodeVisible(void);
	virtual ~SetNodeVisible(void);

	virtual bool init();
	virtual void done();
	virtual void serialize(const rapidjson::Value &val);
	virtual void removeAll();
private:
	int _nTag;
	bool _bShow;
};


class PlayUIAnimation : public cocos2d::extension::BaseTriggerAction
{
	DECLARE_CLASS_INFO
public:
	PlayUIAnimation(void);
	virtual ~PlayUIAnimation(void);

	virtual bool init();
	virtual void done();
	virtual void serialize(const rapidjson::Value &val);
	virtual void removeAll();
private:
	std::string _uiJsonName;
	std::string _animaitionName;
};


#endif
