#include "ReadJSHelper.h"
#include <comutil.h>

static char s_pszResourcePath[MAX_PATH] = {0};

static void _CheckPath()
{
	if (! s_pszResourcePath[0])
	{
		WCHAR  wszPath[MAX_PATH] = {0};
		int nNum = WideCharToMultiByte(CP_ACP, 0, wszPath, 
			GetCurrentDirectoryW(sizeof(wszPath), wszPath), 
			s_pszResourcePath, MAX_PATH, NULL, NULL);
		s_pszResourcePath[nNum] = '\\'; 
	}
}

ReadJSHelper* ReadJSHelper::s_pHelper = NULL ;

ReadJSHelper::ReadJSHelper()
{
    m_Resources = "";
    m_SceneJson = "";
}

ReadJSHelper::~ReadJSHelper()
{
    
}

ReadJSHelper* ReadJSHelper::Instance()
{
	if (NULL == s_pHelper)
	{
		s_pHelper = new ReadJSHelper() ;
		_CheckPath();
	}
	return s_pHelper ;
}

void ReadJSHelper::Destroy()
{
	if( s_pHelper )
	{
		delete s_pHelper;
		s_pHelper = NULL;
	}
}


cocos2d::CCSize ReadJSHelper::GetWinSize()
{
    return m_WinSize;
}

std::string ReadJSHelper::GetJsonPath()
{
	return m_SceneJson;
}

void ReadJSHelper::Read()
{
    unsigned long _size;
    const char *_pFileContent = (char*)(cocos2d::CCFileUtils::sharedFileUtils()->getFileData(m_XmlFullPath.c_str() , "r", &_size));
    if (_pFileContent == NULL) {
        return;
    }
    TiXmlDocument	_document;
    _document.Parse(_pFileContent, 0, TIXML_ENCODING_UTF8);
    
    TiXmlElement	*_root = _document.RootElement();
    TiXmlElement *CanvasSize = _root->FirstChildElement("CanvasSize");
    TiXmlElement *width = CanvasSize->FirstChildElement("Width");
    TiXmlElement *height = CanvasSize->FirstChildElement("Height");
    m_WinSize.width = atof(width->FirstChild()->Value());
    m_WinSize.height = atof(height->FirstChild()->Value());
    
    TiXmlElement *Resources = _root->FirstChildElement("Resources");
    m_Resources.assign(Resources->FirstChild()->Value());
	m_Resources.append("/");
	SetCurrentDirectoryA(m_Resources.c_str());
	cocos2d::CCFileUtils::sharedFileUtils()->addSearchPath(m_Resources.c_str());
    
	TiXmlElement *SceneJson = _root->FirstChildElement("Name");
	m_SceneJson.assign(m_Resources);
    m_SceneJson.append(SceneJson->FirstChild()->Value());
	m_SceneJson.append(".json");
  
}

void ReadJSHelper::GetCommandInfo(CStringA strCmdLine)
{
	CStringA strParamFlag;		
	CStringA strParamContent;	
	int nPos = strCmdLine.Find( ' ' );
	if ( nPos > 0 )
	{
		strParamFlag = strCmdLine.Left( nPos ).Trim();
		strCmdLine = strCmdLine.Right( strCmdLine.GetLength() - nPos - 1 ).Trim();
		strParamFlag.MakeUpper();
		if (strParamFlag == "-FLUSHTYPE")
		{
			if (strCmdLine.IsEmpty())
			{
				return;
			}
			nPos = strCmdLine.Find(' ');
			if ( nPos > 0 )
			{
				strParamContent = strCmdLine.Left( nPos ).Trim();
				strCmdLine = strCmdLine.Right( strCmdLine.GetLength() - nPos - 1 ).Trim();
			}
			else
			{
				strParamContent = strCmdLine.Trim();
				strCmdLine.Empty();
			}
			m_FT = (FlushType)(atoi(strParamContent));
		}

		if (strParamFlag == "-XMLPATH")
		{
			if (strCmdLine.IsEmpty())
			{
				return;
			}
			m_XmlFullPath.assign(strCmdLine.GetBuffer());
		}
		GetCommandInfo(strCmdLine);
	}
	else
	{

	}
}

void ReadJSHelper::GetCommand(CStringA strCmdLine)
{
	if ( strCmdLine.IsEmpty() )
	{
		return;
	}
	strCmdLine.Remove( '"' );
	strCmdLine.Trim();
	int nIndex = strCmdLine.Find( '\\', 0 );
	if ( nIndex > 0 )
	{
		nIndex = strCmdLine.Find( ' ', 0 );
		if ( nIndex > 0 )
		{
			strCmdLine = strCmdLine.Right( strCmdLine.GetLength() - nIndex - 1 ).Trim();
			if ( strCmdLine.IsEmpty() )
			{
				return;
			}
		}
		else
		{
			return;
		}
	}

	if ( strCmdLine.IsEmpty() )
	{
		return;
	}

	GetCommandInfo(strCmdLine);
}

void ReadJSHelper::DealCommandLine()
{
	GetCommand(GetCommandLine());
	Read();
	cocos2d::CCFileUtils::sharedFileUtils()->addSearchPath(m_Resources.c_str());
}
