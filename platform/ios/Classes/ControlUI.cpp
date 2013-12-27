

#include "ControlUI.h"
#include "cocos-ext.h"
#include "SceneRender.h"
#include "SceneHelper.h"
#include "ZBarInterface.h"
#include "FileInfo.h"
using namespace cocos2d;
using namespace cocos2d::extension;


ControlUI::ControlUI()
{
}
ControlUI::~ControlUI()
{
	if(m_GUILayer)
		m_GUILayer->removeFromParent();

}
bool ControlUI::init()
{
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    CCSize size = pEGLView->getFrameSize();
        
    m_GUILayer = UILayer::create();
    
    uiDownloadText = UILabel::create();
	uiDownloadText->setFontName("Helvetica");
	uiDownloadText->setFontSize(40);
	uiDownloadText->setAnchorPoint(ccp(0,0.5));
    uiDownloadText->setPosition(ccp(size.width / 2, size.height - uiDownloadText->getSize().height * 2));
	m_GUILayer->addWidget(uiDownloadText);
    
    uiLoadingBar = UILoadingBar::create();
    uiLoadingBar->loadTexture("sliderProgress.png");
    uiLoadingBar->setPosition(ccp(size.width / 2, uiLoadingBar->getSize().height * 3));
    uiLoadingBar->setPercent(0);
    m_GUILayer->addWidget(uiLoadingBar);

    m_pConnectButton = UIButton::create();
    m_pConnectButton->setTouchEnabled(true);
    m_pConnectButton->loadTextures("buttonNormal.png","buttonPress.png","buttonDisble.png");
    m_pConnectButton->setText("Connect");
    m_pConnectButton->addTouchEventListener(this,toucheventselector(ControlUI::pick));
    m_pConnectButton->setScale(2.0);
	m_pConnectButton->setFontSize(18);
	cocos2d::CCSize connect_size = m_pConnectButton->getContentSize();
    m_pConnectButton->setPosition(ccp(size.width / 6, size.height - connect_size.height * 4));
	m_GUILayer->addWidget(m_pConnectButton);
	
	m_pRenderButton = UIButton::create();
    m_pRenderButton->setName("name render");
    m_pRenderButton->setTouchEnabled(true);
    m_pRenderButton->loadTextures("buttonNormal.png","buttonPress.png","buttonDisble.png");
    m_pRenderButton->setText("Render");
    CCSize render_size = m_pRenderButton->getSize();
    m_pRenderButton->setPosition(ccp(size.width - size.width / 4.5, size.height - render_size.height * 4));
    m_pRenderButton->addTouchEventListener(this,toucheventselector(HelloWorld::Render));
    m_pRenderButton->setScale(2.0);
	m_pRenderButton->setFontSize(18);
    m_GUILayer->addWidget(m_pRenderButton);
	
	m_pCleanButton = UIButton::create();
    m_pCleanButton->setTouchEnabled(true);
    m_pCleanButton->loadTextures("buttonNormal.png","buttonPress.png","buttonDisble.png");
    m_pCleanButton->setText("Reset");
    m_pCleanButton->addTouchEventListener(this,toucheventselector(ControlUI::Clean));
    m_pCleanButton->setScale(2.0);
	m_pCleanButton->setFontSize(18);
	cocos2d::CCSize reset_size = m_pCleanButton->getContentSize();
    m_pCleanButton->setPosition(ccp(size.width - size.width / 4.5, reset_size.height * 6));
    m_GUILayer->addWidget(m_pCleanButton);
    
    /* pipu */
    m_pIP_button = UIButton::create();
    m_pIP_button->setTouchEnabled(true);
    m_pIP_button->loadTextures("buttonNormal.png","buttonPress.png","buttonDisble.png");
    m_pIP_button->setText("Input IP");
    m_pIP_button->addTouchEventListener(this,toucheventselector(ControlUI::ipEvent));
    m_pIP_button->setScale(2.0);
	m_pIP_button->setFontSize(18);
    CCSize ip_size = m_pIP_button->getSize();
    m_pIP_button->setPosition(ccp(size.width / 6, ip_size.height * 6));
    m_GUILayer->addWidget(m_pIP_button);
    
    m_pIP_layout = UILayout::create();
    m_pIP_layout->setVisible(false);
    m_pIP_layout->setBackGroundImageScale9Enabled(true);
    m_pIP_layout->setBackGroundImage("green_edit.png");
    m_pIP_layout->setSize(CCSizeMake(size.width / 2, size.height / 2));
    CCSize layout_size = m_pIP_layout->getSize();
    m_pIP_layout->setPosition(ccp((size.width - layout_size.width) / 2,
                                  (size.height - layout_size.height) / 2 + layout_size.height / 8));
    m_GUILayer->addWidget(m_pIP_layout);
    
    m_pIP_Title_label = UILabel::create();
    m_pIP_Title_label->setText("Input IP please");
    m_pIP_Title_label->setFontSize(30);
    m_pIP_Title_label->setFontName("Marker Felt");
    m_pIP_Title_label->setPosition(ccp(layout_size.width / 2,
                                       layout_size.height - m_pIP_Title_label->getSize().height * 1.75));
    m_pIP_layout->addChild(m_pIP_Title_label);
    
    m_pIP_Ok_button = UIButton::create();
    m_pIP_Ok_button->setTouchEnabled(true);
    m_pIP_Ok_button->loadTextures("buttonNormal.png","buttonPress.png","buttonDisble.png");
    m_pIP_Ok_button->setText("Ok");
    m_pIP_Ok_button->addTouchEventListener(this,toucheventselector(ControlUI::okEvent));
    m_pIP_Ok_button->setScale(1.5);
	m_pIP_Ok_button->setFontSize(18);
    CCSize ok_size = m_pIP_Ok_button->getSize();
    m_pIP_Ok_button->setPosition(ccp(layout_size.width / 2 - (ok_size.width + ok_size.width / 8),
                                     ok_size.height + ok_size.height / 2));
    m_pIP_layout->addChild(m_pIP_Ok_button);
    
    m_pIP_Cancel_button = UIButton::create();
    m_pIP_Cancel_button->setTouchEnabled(true);
    m_pIP_Cancel_button->loadTextures("buttonNormal.png","buttonPress.png","buttonDisble.png");
    m_pIP_Cancel_button->setText("Cancel");
    m_pIP_Cancel_button->addTouchEventListener(this,toucheventselector(ControlUI::cancelEvent));
    m_pIP_Cancel_button->setScale(1.5);
	m_pIP_Cancel_button->setFontSize(18);
    CCSize cancel_size = m_pIP_Cancel_button->getSize();
    m_pIP_Cancel_button->setPosition(ccp(layout_size.width / 2 + (cancel_size.width + cancel_size.width / 8),
                                          cancel_size.height + cancel_size.height / 2));
    m_pIP_layout->addChild(m_pIP_Cancel_button);
    
    CCSize editBoxSize = CCSizeMake(layout_size.width / 1.4, 60);
    
    m_pIP_editbox = CCEditBox::create(editBoxSize, CCScale9Sprite::create("yellow_edit.png"));
    m_pIP_editbox->setPosition(ccp(layout_size.width / 2.0f,
                                   layout_size.height / 2.0f + m_pIP_editbox->getContentSize().height / 4));
    m_pIP_editbox->setFontName("Marker Felt");
    m_pIP_editbox->setFontSize(30);
    m_pIP_editbox->setFontColor(ccWHITE);
    m_pIP_editbox->setPlaceHolder("Input IP here:");
    m_pIP_editbox->setPlaceholderFontColor(ccc3(128, 128, 128));
    m_pIP_editbox->setInputMode(kEditBoxInputModeNumeric);
    m_pIP_editbox->setReturnType(kKeyboardReturnTypeDone);
    CCSize editbox_size = m_pIP_editbox->getContentSize();
    m_pIP_layout->addRenderer(m_pIP_editbox, 0);
    
    m_pIP_Remove_button = UIButton::create();
    m_pIP_Remove_button->setTouchEnabled(true);
    m_pIP_Remove_button->loadTextures("x_alt_pressed.png","x_alt.png","");
    m_pIP_Remove_button->addTouchEventListener(this,toucheventselector(ControlUI::removeEvent));
	m_pIP_Remove_button->setFontSize(18);
    CCSize remove_size = m_pIP_Remove_button->getSize();
    m_pIP_Remove_button->setPosition(ccp(m_pIP_editbox->getPosition().x + editbox_size.width / 2 + remove_size.width,
                                     m_pIP_editbox->getPosition().y));
    m_pIP_layout->addChild(m_pIP_Remove_button);
    
    m_pConnect_image = UIImageView::create();
    m_pConnect_image->setVisible(false);
    m_pConnect_image->loadTexture("connectsuccessful.png");
    CCSize status_connect_size = m_pConnect_image->getSize();
    m_pConnect_image->setPosition(ccp(status_connect_size.width / 2, status_connect_size.height / 2));
    m_GUILayer->addWidget(m_pConnect_image);
    
    m_pDisconnect_image = UIImageView::create();
    m_pDisconnect_image->setVisible(true);
    m_pDisconnect_image->loadTexture("disconnect.png");
    CCSize status_disconnect_size = m_pDisconnect_image->getSize();
    m_pDisconnect_image->setPosition(ccp(status_disconnect_size.width / 2, status_disconnect_size.height / 2));
    m_GUILayer->addWidget(m_pDisconnect_image);
    /**/
	
	UIButton*	pCloseButton = UIButton::create();
    pCloseButton->setTouchEnabled(true);
    pCloseButton->loadTextures("CloseNormal.png","CloseNormal.png","CloseSelected.png");
    pCloseButton->setPosition(ccp(size.width - 70,50));
    pCloseButton->addTouchEventListener(this,toucheventselector(ControlUI::menuCloseCallback));
	pCloseButton->setFontSize(18);
    m_GUILayer->addWidget(pCloseButton);
	addChild(m_GUILayer,2);
	return true;
}
void ControlUI::disableConnectButton()
{
    if (m_pConnectButton->isBright())
    {
        m_pConnectButton->setBright(false);
        m_pConnectButton->setTouchEnabled(false);
    }
	m_pConnectButton->setBright(false);
}
void ControlUI::enableConnectButton()
{
    if (!m_pConnectButton->isBright())
    {
        m_pConnectButton->setBright(true);
        m_pConnectButton->setTouchEnabled(true);
    }
}
//
void ControlUI::disableRenderButton()
{
	if(m_pRenderButton->isBright())
	{
		m_pRenderButton->setBright(false);
		m_pRenderButton->setTouchEnabled(false);
	}
}
//
void ControlUI::enableRenderButton()
{
	if(!m_pRenderButton->isBright())
	{
		m_pRenderButton->setBright(true);
		m_pRenderButton->setTouchEnabled(true);
	}
}

void ControlUI::disableResetButton()
{
    if (m_pCleanButton->isBright())
    {
        m_pCleanButton->setBright(false);
        m_pCleanButton->setTouchEnabled(false);
    }
}

void ControlUI::enableResetButton()
{
    if (!m_pCleanButton->isBright())
    {
        m_pCleanButton->setBright(true);
        m_pCleanButton->setTouchEnabled(true);
    }
}

void ControlUI::disableIPButton()
{
    if (m_pIP_button->isBright())
    {
        m_pIP_button->setBright(false);
        m_pIP_button->setTouchEnabled(false);
    }
}

void ControlUI::enableIPButton()
{
    if (!m_pIP_button->isBright())
    {
        m_pIP_button->setBright(true);
        m_pIP_button->setTouchEnabled(true);
    }
}

void ControlUI::setLoadingBarPercent(int percent)
{
    uiLoadingBar->setPercent(percent);
}

void ControlUI::setLoadingBarVisible(bool visible)
{
    uiLoadingBar->setVisible(visible);
}

void ControlUI::setDownLoadTextVisible(bool visible)
{
    uiDownloadText->setVisible(visible);
}

void ControlUI::setIP(const char *ip)
{
    m_pIP_editbox->setText(ip);
}
/**/

void ControlUI::setProgress(float downloaded)
{
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	uiDownloadText->setPosition(ccp(size.width/2 - 150, size.height - uiDownloadText->getSize().height * 2));
	char temp[128];memset(temp, 0x00, sizeof(temp));
	sprintf(temp, "DownLoading: %d kb", (int)downloaded/1024);
	setTitle(temp);
    double total = SceneHelper::sharedSceneHelper()->getDownLoadTotal();
    int percent = ceil(downloaded / total * 100);
    setLoadingBarPercent(percent);
}
void ControlUI::setTitle(const char* text)
{
	uiDownloadText->setText(text);
	CCSize visible = CCDirector::sharedDirector()->getVisibleSize();
	int len = uiDownloadText->getStringLength();
	uiDownloadText->setPosition(ccp(visible.width / 2 - len * 9, visible.height - uiDownloadText->getSize().height * 2));
}
void ControlUI::pick(CCObject* sender, cocos2d::extension::TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
        if (SceneHelper::sharedSceneHelper()->getDownLoadState() == DOWNLOADING)
        {
            return;
        }
        ZBarInterface::sharedZBarInterface()->pick();
	}
}

void ControlUI::Clean(CCObject* sender, cocos2d::extension::TouchEventType type)
{
    if (SceneHelper::sharedSceneHelper()->getDownLoadState() == DOWNLOADING)
    {
        return;
    }
	if(type == TOUCH_EVENT_ENDED)
	{
		SceneHelper::sharedSceneHelper()->setHaveResources(false);
		ZBarInterface::sharedZBarInterface()->ReadPath();
		ZBarInterface::sharedZBarInterface()->CleanPath();
		ZBarInterface::sharedZBarInterface()->ReadPath();
	
	}
}

void ControlUI::ipEvent(CCObject *sender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SceneHelper* sceneHelper = SceneHelper::sharedSceneHelper();
        if (sceneHelper->getDownLoadState() == DOWNLOADING)
        {
            return;
        }
        m_pIP_layout->setVisible(!m_pIP_layout->isVisible());
        CCObject* obj = NULL;
        CCARRAY_FOREACH(m_pIP_layout->getChildren(), obj)
        {
            UIWidget* widget = static_cast<UIWidget*>(obj);
            widget->setTouchEnabled(true);
        }
        
        m_pIP_editbox->setText(ZBarInterface::sharedZBarInterface()->getIP());
        
        m_pConnectButton->setTouchEnabled(false);
        m_pConnectButton->setBright(false);
        m_pCleanButton->setTouchEnabled(false);
        m_pCleanButton->setBright(false);
        m_pRenderButton->setTouchEnabled(false);
        m_pRenderButton->setBright(false);
        m_pIP_button->setTouchEnabled(false);
        m_pIP_button->setBright(false);
        
        sceneHelper->setHelloWorldState(INPUT_IP);
    }
}

void ControlUI::okEvent(CCObject *sender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        m_pIP_layout->setVisible(false);
        CCObject* obj = NULL;
        CCARRAY_FOREACH(m_pIP_layout->getChildren(), obj)
        {
            UIWidget* widget = static_cast<UIWidget*>(obj);
            widget->setTouchEnabled(false);
        }
     
        m_pConnectButton->setTouchEnabled(true);
        m_pConnectButton->setBright(true);
        m_pCleanButton->setTouchEnabled(true);
        m_pCleanButton->setBright(true);
        m_pRenderButton->setTouchEnabled(true);
        m_pRenderButton->setBright(true);
        m_pIP_button->setTouchEnabled(true);
        m_pIP_button->setBright(true);
        
        SceneHelper* sceneHelper = SceneHelper::sharedSceneHelper();
        sceneHelper->setHelloWorldState(NORMAL);
        ZBarInterface* zbar = ZBarInterface::sharedZBarInterface();
        std::string value = m_pIP_editbox->getText();
        
        if (strcmp(value.c_str(), zbar->getIP()) != 0)
        {
            if (zbar->isConnected())
            {
                zbar->DisConnect();
                SceneHelper::sharedSceneHelper()->setSocketState(SOCKET_DISCONNECT);
            }
        }
        
        int colonIndex = value.find_last_of(":");
        std::string strIP = value.substr(0, colonIndex);
        std::string strPort = value.substr(colonIndex + 1, value.length());
        int port = atoi(strPort.c_str());
        ZBarInterface::sharedZBarInterface()->setIP(value.c_str());
        CCLOG("strIP = %s", strIP.c_str());
        CCLOG("port = %d", port);
        int result = ZBarInterface::sharedZBarInterface()->CreateConnect(strIP.c_str(), port);
        CCLOG("result = %d", result);
    }
}

void ControlUI::cancelEvent(CCObject *sender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        m_pIP_layout->setVisible(false);
        
        SceneHelper* sceneHelper = SceneHelper::sharedSceneHelper();
        sceneHelper->setHelloWorldState(NORMAL);
    }
}

void ControlUI::removeEvent(CCObject *sender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        m_pIP_editbox->setText("");
    }
}

void ControlUI::menuCloseCallback(CCObject* sender, cocos2d::extension::TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
        ZBarInterface::sharedZBarInterface()->TaskCancel();
		exit(0);
	}
}


