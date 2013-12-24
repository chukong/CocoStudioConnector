
#ifndef COCOEXTENSIONS_COLLIDERRECTDRAWER_h
#define COCOEXTENSIONS_COLLIDERRECTDRAWER_h

#include "cocos2d.h"
#include "cocos-ext.h"

/**
 *  PrimitiveLayer is inherited from CCLayer, using for rendering all primitive
 */
class  ColliderRectDrawer : public cocos2d::CCLayer
{
public:
    static ColliderRectDrawer *create(cocos2d::extension::CCArmature *armature);
public:
    ColliderRectDrawer();
    /**
     *	@brief renders Primitive
     */
    virtual void draw();
private:
    CC_SYNTHESIZE(cocos2d::extension::CCArmature*, m_pArmature, Armature);
};


#endif
