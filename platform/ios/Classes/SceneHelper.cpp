

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
: m_pRenderScene(NULL)
, m_nNowRunning(DEFAULT)
, m_bHaveResouces(false)
, m_bDownLoading(false)
, m_bDownLoadFinished(false)
, m_bWaitDownLoadStart(false)
, m_bLoginEnabled(false)
, m_bServerIsDown(false)
, m_eHelloWorldState(NORMAL)
, m_eSocketState(SOCKET_NONE)
, m_eDownLoadState(DOWNLOAD_NONE)
, m_nDownLoadByte(0)
, m_strAction("")
, m_strInfo("")
, m_nErrortype(-1)
, m_nRepose(-1)
, m_bPrint(false)
, m_OldSize(CCSizeZero)
, m_bTouchedRender(false)
{
    VitCreateLock(g_lock);
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
	std::vector<std::string> searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();
	searchPaths.insert(searchPaths.begin(), pathToSave);
	CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
}

void SceneHelper::setLastError(const char* action, const char* info, int errortype, int repose)
{
	m_strAction = action;
	m_strInfo = info;
	m_nErrortype = errortype;
	m_nRepose = repose;
}

const double SceneHelper::getDownLoadTotal() const
{
    CHttp * htp = CHttp::getSingletonPtr();
    return htp->getDownLoadTotal();
}

