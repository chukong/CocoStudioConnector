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

#ifndef __CCSPreviewer__SceneHelper__
#define __CCSPreviewer__SceneHelper__

#include <iostream>
#include "HelloWorldScene.h"
#include "SceneRender.h"
#include "cocos2d.h"
#include "VitLock.h"
#include <pthread.h>

enum SceneNowRunning
{
    DEFAULT = -1,
    HELLOWORLD = 0,
    RENDER = 1,
};

enum HelloWorldState
{
    NORMAL = 0,
    INPUT_IP = 1,
};

enum SceneSocketState
{
    SOCKET_NONE = -1,
    SOCKET_SERVER_IS_DOWN = 0,
    SOCKET_CONNECT,
    SOCKET_DISCONNECT,
};

enum SceneDownLoadState
{
    DOWNLOAD_NONE = -1,
    WAIT_DOWNLOAD_START = 0,
    HTTP_SERVER_IS_DOWN,
    DOWNLOADING,
    DOWNLOADED_FINISHED,
    DOWNLOADED_UNCOMPRESS,
    DOWNLOAD_STOP,
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
	
    void setNowRunning(SceneNowRunning cur) { m_nNowRunning = cur; }
	const SceneNowRunning getNowRunning() const { return m_nNowRunning; }
    
    /* refactoring */
    /*
	cocos2d::CCScene* getHelloWorld() { return m_pHelloWorldScene; }
    void setHelloWorld(cocos2d::CCScene* scene)
    {
        m_pHelloWorldScene = scene;
        m_pHelloWorldScene->retain();
    }
	SceneRender* getRenderScene() { return m_pRenderScene; }
	void setRenderScene(SceneRender* scene) { m_pRenderScene = scene; }
     */
    /**/
    
    void setHaveResources(bool b) { m_bHaveResouces = b; }
	bool isHaveResources() { return m_bHaveResouces; }
	
    
	bool isDownloading() { return m_bDownLoading; }
	void setDownloading(bool b) { m_bDownLoading = b; }
    bool isDownLoadFinished() { return m_bDownLoadFinished; }
	void setDownLoadFinished(bool b) {m_bDownLoadFinished = b;}
    
    /* pipu */
    const bool isServerIsDown() const { return m_bServerIsDown; };
    void setServerIsDown(bool isDown) { m_bServerIsDown = isDown; };
    const bool isLoginEnabled() const { return m_bLoginEnabled; };
    void setLoginEnabled(bool enabled) { m_bLoginEnabled = enabled; };
    const bool isWaitDownLoadStart() const { return m_bWaitDownLoadStart; };
    void setWaitDownLoadStart(bool start) { m_bWaitDownLoadStart = start; };
    
    void setHelloWorldState(HelloWorldState state) { m_eHelloWorldState = state; };
    const HelloWorldState getHelloWorldState() const { return m_eHelloWorldState; };
    
    void setSocketState(SceneSocketState state) { m_eSocketState = state; };
    const SceneSocketState getSocketState() const { return m_eSocketState; };
    
    void setDownLoadState(SceneDownLoadState state) { m_eDownLoadState = state; };
    const SceneDownLoadState getDownLoadState() const { return m_eDownLoadState; };
    
    const double getDownLoadTotal() const;
    /**/
    
    int  getDownLoadByte() { return m_nDownLoadByte; }
	void setDownLoadByte(int cn) { m_nDownLoadByte = cn; }
    
	void saveConfig();
	void loadConfig();
	void saveSerchPath();
	void cleanResources();
	
	void setLastError(const char* action, const char* info, int errortype, int repose);
	bool getPrint() { return m_bPrint; }
	void setPrint(bool b) { m_bPrint = b;}
	
	const char* getAction() { return m_strAction.c_str(); }
	const char* getInfo() { return m_strInfo.c_str(); }
	int getErrorType() { return m_nErrortype; }
	void setOldSize(int width, int height)
    {
        m_OldSize.width = width,
        m_OldSize.height = height;
    };
	cocos2d::CCSize* getOldSize(){ return &m_OldSize; }
	
	void setTouchRender(bool b) { m_bTouchedRender = b; }
	bool isTouchRender() { return m_bTouchedRender; }
    
protected:
    /* refactoring */
//	cocos2d::CCScene* m_pHelloWorldScene;
    /**/
	SceneRender* m_pRenderScene;
	static SceneHelper* s_SceneRender;
	SceneNowRunning   m_nNowRunning;
    
    bool  m_bHaveResouces;
    
	bool  m_bDownLoading;
	bool  m_bDownLoadFinished;
    /* pipu */
    bool m_bWaitDownLoadStart;
    bool m_bLoginEnabled;
    bool m_bServerIsDown;
    
    SceneSocketState m_eSocketState;
    SceneDownLoadState m_eDownLoadState;
    HelloWorldState m_eHelloWorldState;
    /**/
    int   m_nDownLoadByte;
	
	std::string m_strAction;
	std::string m_strInfo;
	int m_nErrortype;
	int m_nRepose;
	bool m_bPrint;
	cocos2d::CCSize m_OldSize;
	bool m_bTouchedRender;
	
};

#endif /* defined(__CCSPreviewer__SceneHelper__) */
