//
//  SceneHelper.h
//  CCSPreviewer
//
//  Created by jiang xiaohua on 13-11-13.
//
//

#ifndef __CCSPreviewer__SceneHelper__
#define __CCSPreviewer__SceneHelper__

#include <iostream>
#include "HelloWorldScene.h"
#include "SceneRender.h"
#include "cocos2d.h"
#include "VitLock.h"
#include <pthread.h>

enum  {
  HELLOWORLD = 0,
  RENDER = 1
};
extern SVitLock g_lock;

//pthread_mutex_t a;
//void test(){
//pthread_mutex_init(&a,NULL);
//}
//pthread_mutexattr_t mattr;
//void test(){
//pthread_mutexattr_init(&mattr); /* * resetting to its default value: private */
//}
struct lastErrot {
  
};
class SceneHelper
{
public:
	SceneHelper();
	~SceneHelper();
	static SceneHelper* sharedSceneHelper();
	
	cocos2d::CCScene* getHelloWorld(){ return m_pHelloWorldScene; }
	SceneRender* getRenderScene(){ return m_pRenderScene; }
	void setHelloWorld(cocos2d::CCScene* scene){ m_pHelloWorldScene = scene; m_pHelloWorldScene->retain(); }
	void setRenderScene(SceneRender* scene){ m_pRenderScene = scene; }
	void setNowRunning(int cur){ m_nNowRunning = cur; }
	int  getNowRunning(){ return m_nNowRunning; }
	bool isDownloading(){ return m_bDownLoading; }
	void setDownloading(bool b){ m_bDownLoading = b; }
	void setHaveResources(bool b){ m_bHaveResouces = b; }
	bool isHaveResources(){ return m_bHaveResouces; }
	int  getDownLoadByte(){ return m_nDownLoadByte; }
	void setDownLoadByte(int cn){ m_nDownLoadByte = cn; }
	void setDownLoadFinished(bool b){m_bDownLoadFinished = b;}
	bool isDownLoadFinished(){ return m_bDownLoadFinished; }
    const double getDownLoadTotal() const;
    /* pipu */
    void setWaitDownLoadStart(bool start) { m_bWaitDownLoadStart = start; };
    const bool isWaitDownLoadStart() const { return m_bWaitDownLoadStart; };
    void setLoginEnabled(bool enabled) { m_bLoginEnabled = enabled; };
    const bool isLoginEnabled() const { return m_bLoginEnabled; };
    void setServerIsDown(bool isDown) { m_bServerIsDown = isDown; };
    const bool isServerIsDown() const { return m_bServerIsDown; };
    /**/
	void saveConfig();
	void loadConfig();
	void saveSerchPath();
	void cleanResources();
	
	void setLastError(const char* action, const char* info, int errortype, int repose);
	bool getPrint(){return m_bPrint;}
	void setPrint(bool b){ m_bPrint = b;}
	
	const char* getAction(){ return m_strAction.c_str(); }
	const char* getInfo(){ return m_strInfo.c_str(); }
	int getErrorType(){ return m_nErrortype; }
	void setOldSize(int width, int height){ m_OldSize.width = width, m_OldSize.height = height; };
	cocos2d::CCSize* getOldSize(){ return &m_OldSize;}
	
	void setTouchRender(bool b){m_bTouchedRender = b;}
	bool isTouchRender(){ return m_bTouchedRender; }
protected:
	cocos2d::CCScene* m_pHelloWorldScene;
	SceneRender* m_pRenderScene;
	static SceneHelper* s_SceneRender;
	int   m_nNowRunning;
	bool  m_bDownLoading;
	bool  m_bHaveResouces;
	int   m_nDownLoadByte;
	bool  m_bDownLoadFinished;
    /* pipu */
    bool m_bWaitDownLoadStart;
    bool m_bLoginEnabled;
    bool m_bServerIsDown;
    /**/
	
	std::string m_strAction;
	std::string m_strInfo;
	int m_nErrortype;
	int m_nRepose;
	bool m_bPrint;
	cocos2d::CCSize m_OldSize;
	bool m_bTouchedRender;
	
};

#endif /* defined(__CCSPreviewer__SceneHelper__) */
