//
//  ControlUI.h
//  CCSPreviewer
//
//  Created by jiang xiaohua on 13-11-1.
//
//

#ifndef __CCSPreviewer__ControlUI__
#define __CCSPreviewer__ControlUI__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"


class ControlUI : public cocos2d::CCLayer
{
public:
	ControlUI();
	virtual ~ControlUI();
	virtual bool init();
	CREATE_FUNC(ControlUI)
	//ui show
	void setProgress(float downloaded);
	void setTitle(const char*);
//	void Render(CCObject* sender, cocos2d::extension::TouchEventType type);
	//
	void Clean(CCObject* sender, cocos2d::extension::TouchEventType type);
	//ZBar
	void pick(CCObject* sender, cocos2d::extension::TouchEventType type);
    /* pipu */
    void ipEvent(CCObject* sender, cocos2d::extension::TouchEventType type);
    void okEvent(CCObject* sender, cocos2d::extension::TouchEventType type);
    void cancelEvent(CCObject* sender, cocos2d::extension::TouchEventType type);
    void removeEvent(CCObject* sender, cocos2d::extension::TouchEventType type);
    cocos2d::extension::UIImageView* getConnectImage() const { return m_pConnect_image; };
    cocos2d::extension::UIImageView* getDisconnectImage() const { return m_pDisconnect_image; };
    /**/
	//closet
	void menuCloseCallback(CCObject* sender, cocos2d::extension::TouchEventType type);
	//
	cocos2d::extension::UILabel* getTitle(){ return uiDownloadText; }
	//
	void disableConnectButton();
	//
	void enableConnectButton();
	
	//
	void disableRenderButton();
	//
	void enableRenderButton();
    
    /* pipu */
    void disableResetButton();
    void enableResetButton();
    void disableIPButton();
    void enableIPButton();
    
    bool isInputLayoutVisible() { return m_pIP_layout->isVisible(); } ;
    
    void setLoadingBarPercent(int percent);
    
    void setLoadingBarVisible(bool visible);
    void setDownLoadTextVisible(bool visible);
    void setIP(const char* ip);
    /**/
protected:
	
	cocos2d::extension::UILoadingBar* uiLoadingBar;
	cocos2d::extension::UILabel* uiDownloadText;
//	cocos2d::extension::UIButton* m_pSearchButton;
	cocos2d::extension::UIButton* m_pConnectButton;
	cocos2d::extension::UIButton* m_pCleanButton;
	cocos2d::extension::UIButton* m_pRenderButton;
    /* pipu */    
    cocos2d::extension::UIButton* m_pIP_button;
    cocos2d::extension::UILayout* m_pIP_layout;
    cocos2d::extension::UILabel* m_pIP_Title_label;
    cocos2d::extension::UIButton* m_pIP_Ok_button;
    cocos2d::extension::UIButton* m_pIP_Cancel_button;
    cocos2d::extension::CCEditBox* m_pIP_editbox;
    cocos2d::extension::UIButton* m_pIP_Remove_button;
    cocos2d::extension::UIImageView* m_pConnect_image;
    cocos2d::extension::UIImageView* m_pDisconnect_image;
    /**/
	cocos2d::extension::UILayer* m_GUILayer;
};

#endif /* defined(__CCSPreviewer__ControlUI__) */
