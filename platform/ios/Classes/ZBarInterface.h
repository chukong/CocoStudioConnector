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

#ifndef __CCSPreviewer__ZBarInterface__
#define __CCSPreviewer__ZBarInterface__

#include <iostream>
#include "VitConnect.h"
//extern SConnectArg *g_sArg;

class ZBarInterface
{
	
	public:
	ZBarInterface();
	~ZBarInterface();
	static ZBarInterface* sharedZBarInterface();
	static ZBarInterface* s_ZBarInterface;
	
	void pick();

	void DownLoadZip(const char* zip, const char*version, const char* project, const char* width, const char* height);

	void ReadPath();
	void CleanPath();
	bool checkFile(const char* file);
	const char* getDeviceName();
    
    /* pipu */
    bool isConnected();
    int CreateConnect(const char* ip, int port);
    void DisConnect();
    void TaskCancel();
    void DisRender();
    void setIP(const char* ip);
    const char* getIP() const { return m_strIP.c_str(); };        
    void stopDownLoad();
    
    void setAppEnterBackground(bool value) { m_bAppEnterBackground = value; };
    const bool isAppEnterBackground() const { return m_bAppEnterBackground; };
    void setAppEnterForeground(bool value) { m_bAppEnterForeground = value; };
    const bool isAppEnterForeground() const { return m_bAppEnterForeground; };
    void setReConnectWithAppEnterForeground(bool value) { m_bReConnectWithAppEnterForeground = value; };
    const bool isReConnectWithAppEnterForeground() const { return m_bReConnectWithAppEnterForeground; };
    /**/
protected:
    std::string m_sDeviceName;
	
    /* pipu */
    std::string m_strIP;
    bool m_bAppEnterBackground;
    bool m_bAppEnterForeground;
    bool m_bReConnectWithAppEnterForeground;
    /**/
};

//class M


#endif /* defined(__CCSPreviewer__ZBarInterface__) */
