//
//  SceneHelper.cpp
//  CCSPreviewer
//
//  Created by jiang xiaohua on 13-11-13.
//
//

#include "SceneHelper.h"
#include "FileInfo.h"
#include "ZBarInterface.h"
/* pipu */
#include "CHttp.h"
/**/

using namespace cocos2d;
SVitLock g_lock;

SceneHelper* SceneHelper::s_SceneRender = NULL;

SceneHelper::SceneHelper()
{
	m_nNowRunning = -1;
	m_bDownLoading = false;
//	m_bHaveResouces = false;
	VitCreateLock(g_lock);
	m_bDownLoadFinished = false;
    /* pipu */
    m_bLoginEnabled = false;
    m_bWaitDownLoadStart = false;
    m_bServerIsDown = false;
    /**/
	m_bPrint = false;
	m_nDownLoadByte = 0;
	m_bHaveResouces = false;
	m_bTouchedRender = false;
}
SceneHelper::~SceneHelper()
{
}
SceneHelper* SceneHelper::sharedSceneHelper()
{
	if (s_SceneRender== NULL)
	{
		s_SceneRender = new SceneHelper();
	}
	return s_SceneRender;
}
void SceneHelper::saveConfig()
{
	std::string sKey = "ProjectName";
    CCLog("存储之前数据 , key: %s  value: %s",sKey.c_str(),FileInfo::sharedFileInfo()->getFilename());
    CCUserDefault::sharedUserDefault()->setStringForKey(sKey.c_str(),FileInfo::sharedFileInfo()->getFilename());
    CCUserDefault::sharedUserDefault()->flush();
}
void SceneHelper::loadConfig()
{
	std::string sKey = "ProjectName";
	std::string ProjectName = CCUserDefault::sharedUserDefault()->getStringForKey(sKey.c_str());
	CCLog("read: %s\n", ProjectName.c_str());
	
	bool res = ZBarInterface::sharedZBarInterface()->checkFile(ProjectName.c_str());
	if(res)
	{
		FileInfo::sharedFileInfo()->setFilename(ProjectName.c_str());
		sharedSceneHelper()->setHaveResources(true);
	}
}
void SceneHelper::saveSerchPath()
{
	std::string pathToSave = CCFileUtils::sharedFileUtils()->getWritablePath();
//    pathToSave += "tmpdir";
	std::vector<std::string> searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();
	searchPaths.insert(searchPaths.begin(), pathToSave);
	CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
}
void SceneHelper::cleanResources()
{
//	SceneHelper::sharedSceneHelper()->setHaveResources(false);
//	std::string path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath();
	
//	ZBarInterface::sharedZBarInterface()->CleanPath(path.c_str());
	// Remove downloaded files
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
//    std::string command = "rm -r ";
    // Path may include space.
//    command += "\"" + path + "*\"";
//    system(command.c_str());
//	ZBarInterface::sharedZBarInterface()->CleanPath(path.c_str());
//#else
//    string command = "rd /s /q ";
//    // Path may include space.
//    command += "\"" + pathToSave + "\"";
//    system(command.c_str());
//#endif
}
void SceneHelper::setLastError(const char* action, const char* info, int errortype, int repose)
{
	m_strAction = action;
	m_strInfo = info;
	m_nErrortype = errortype;
	m_nRepose = repose;
}

/* pipu */
const double SceneHelper::getDownLoadTotal() const
{
    CHttp * htp = CHttp::getSingletonPtr();
    return htp->getDownLoadTotal();
}
/**/
