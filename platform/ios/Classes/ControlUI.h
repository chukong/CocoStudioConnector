/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

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
	void setProgress(float downloaded);
	void setTitle(const char*);
	void Clean(CCObject* sender, cocos2d::extension::TouchEventType type);
	//ZBar
	void pick(CCObject* sender, cocos2d::extension::TouchEventType type);
    void ipEvent(CCObject* sender, cocos2d::extension::TouchEventType type);
    void okEvent(CCObject* sender, cocos2d::extension::TouchEventType type);
    void cancelEvent(CCObject* sender, cocos2d::extension::TouchEventType type);
    void removeEvent(CCObject* sender, cocos2d::extension::TouchEventType type);
    cocos2d::extension::UIImageView* getConnectImage() const { return m_pConnect_image; };
    cocos2d::extension::UIImageView* getDisconnectImage() const { return m_pDisconnect_image; };
	void menuCloseCallback(CCObject* sender, cocos2d::extension::TouchEventType type);
	cocos2d::extension::UILabel* getTitle(){ return uiDownloadText; }

	void disableConnectButton();
	void enableConnectButton();
	void disableRenderButton();
	void enableRenderButton();
    void disableResetButton();
    void enableResetButton();
    void disableIPButton();
    void enableIPButton();
    bool isInputLayoutVisible() { return m_pIP_layout->isVisible(); } ;
    void setLoadingBarPercent(int percent);
    void setLoadingBarVisible(bool visible);
    void setDownLoadTextVisible(bool visible);
    void setIP(const char* ip);
    
protected:
	
	cocos2d::extension::UILoadingBar* uiLoadingBar;
	cocos2d::extension::UILabel* uiDownloadText;
	cocos2d::extension::UIButton* m_pConnectButton;
	cocos2d::extension::UIButton* m_pCleanButton;
	cocos2d::extension::UIButton* m_pRenderButton;
    cocos2d::extension::UIButton* m_pIP_button;
    cocos2d::extension::UILayout* m_pIP_layout;
    cocos2d::extension::UILabel* m_pIP_Title_label;
    cocos2d::extension::UIButton* m_pIP_Ok_button;
    cocos2d::extension::UIButton* m_pIP_Cancel_button;
    cocos2d::extension::CCEditBox* m_pIP_editbox;
    cocos2d::extension::UIButton* m_pIP_Remove_button;
    cocos2d::extension::UIImageView* m_pConnect_image;
    cocos2d::extension::UIImageView* m_pDisconnect_image;
	cocos2d::extension::UILayer* m_GUILayer;
};

#endif
