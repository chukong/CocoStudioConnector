//
//  ReadJSHelper.h
//  Samples
//
//

#ifndef __Samples__ReadJSHelper__
#define __Samples__ReadJSHelper__

#include "tinyxml.h"
#include "cocos2d.h"
#include <string>
#include <list>
#include <fstream>
#include <atlstr.h>

//e_FlushOnly 
enum FlushType
{
	e_FlushOnly = 0,
	e_FlushRun,
};
class ReadJSHelper
{
public:
	ReadJSHelper();
	~ReadJSHelper();
private:
    void Read();
	void GetCommandInfo(CStringA strCmdLine);
	void GetCommand(CStringA strCmdLine);
    bool FileIsExist(const char *filePath);
    std::wstring c2w(const char *pc);
    std::vector<std::string> split(std::string str,std::string pattern);
public:
	void DealCommandLine();
	static ReadJSHelper* Instance();
	static void Destroy();
	cocos2d::CCSize GetWinSize();
	std::string GetJsonPath();
private:
    cocos2d::CCSize m_WinSize;
    std::string m_Resources;
    std::string m_SceneJson;
	FlushType m_FT;
	CStringA m_XmlFullPath;
    std::vector<std::string> m_DllFullPath;
private:
	static ReadJSHelper *s_pHelper ;
};


#endif 