//
//  ZBarInterface.h
//  CCSPreviewer
//
//  Created by jiang xiaohua on 13-10-30.
//
//

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
