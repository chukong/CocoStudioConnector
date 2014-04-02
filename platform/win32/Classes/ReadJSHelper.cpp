#include "ReadJSHelper.h"
#include <comutil.h>
#include <io.h>`

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
    const char *_pFileContent = (char*)(cocos2d::CCFileUtils::sharedFileUtils()->getFileData(m_XmlFullPath.GetBuffer() , "r", &_size));
    if (_pFileContent == NULL) {
        return;
    }

    std::string strContent(_pFileContent);

    TiXmlDocument	_document;
    _document.Parse(strContent.c_str(), 0, TIXML_ENCODING_UTF8);
    
    TiXmlElement	*_root = _document.RootElement();
    TiXmlElement *CanvasSize = _root->FirstChildElement("CanvasSize");
    TiXmlElement *width = CanvasSize->FirstChildElement("Width");
    TiXmlElement *height = CanvasSize->FirstChildElement("Height");
    m_WinSize.width = atof(width->FirstChild()->Value());
    m_WinSize.height = atof(height->FirstChild()->Value());
    
    TiXmlElement *resRelativepath = _root->FirstChildElement("ResRelativePath");
    std::string version2 = "";
    std::string version1;
    version1.assign(resRelativepath->FirstChild()->Value());
    if (version2 != version1)
    {
        int nPos = m_XmlFullPath.Find("CocoStudio\\ccsprojs");
        if (nPos < 0)
        {
            return;
        }
        m_Resources = m_XmlFullPath.Left(nPos).Trim();
        m_Resources += "CocoStudio\\assets";
        SetCurrentDirectoryA(m_Resources.c_str());
        cocos2d::CCFileUtils::sharedFileUtils()->addSearchPath(m_Resources.c_str());
        std::string dir(m_Resources);

        TiXmlElement *SceneJson = _root->FirstChildElement("Name");
        m_SceneJson.assign(m_Resources);
        m_SceneJson.append("/publish/");
        m_SceneJson.append(SceneJson->FirstChild()->Value());
    }
    else
    {
        TiXmlElement *Resources = _root->FirstChildElement("Resources");
        m_Resources.assign(Resources->FirstChild()->Value());
        m_Resources.append("/");
        SetCurrentDirectoryA(m_Resources.c_str());
        cocos2d::CCFileUtils::sharedFileUtils()->addSearchPath(m_Resources.c_str());

        TiXmlElement *SceneJson = _root->FirstChildElement("Name");
        m_SceneJson.assign(m_Resources);
        m_SceneJson.append(SceneJson->FirstChild()->Value());
    }

	m_SceneJson.append(".json");
  
    for (std::vector<std::string>::iterator iter = m_DllFullPath.begin(); iter != m_DllFullPath.end(); ++iter)
    {
        if ((*iter) != "" && FileIsExist((*iter).c_str()))
        {
            std::wstring dllPath = c2w((*iter).c_str());
            LoadLibrary(dllPath.c_str());
        }
    }
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
			nPos = strCmdLine.Find("-XMLPATH");
            nPos -= 1;
			if ( nPos > 0 )
			{
				strParamContent = strCmdLine.Left(nPos).Trim();
				strCmdLine = strCmdLine.Right( strCmdLine.GetLength() - nPos - 1 ).Trim();
			}
			else
			{
				strParamContent = strCmdLine.Trim();
				strCmdLine.Empty();
			}
			m_FT = (FlushType)(atoi(strParamContent));
		}
		else if (strParamFlag == "-XMLPATH")
		{
			if (strCmdLine.IsEmpty())
			{
				return;
			}
            nPos = strCmdLine.Find("-DLLPATH");
            nPos -= 1;
            if ( nPos > 0 )
            {
                strParamContent = strCmdLine.Left(nPos).Trim();
                strCmdLine = strCmdLine.Right( strCmdLine.GetLength() - nPos - 1 ).Trim();
            }
            else
            {
                strParamContent = strCmdLine.Trim();
                strCmdLine.Empty();
            }
            CStringA xmlFullPath(strParamContent.GetBuffer());
            m_XmlFullPath = xmlFullPath;
        }
        else if (strParamFlag == "-DLLPATH")
        {
            if (strCmdLine.IsEmpty())
            {
                return;
            }
            m_DllFullPath = split(strCmdLine.GetBuffer(), "|");
        }
        GetCommandInfo(strCmdLine);
    }
    else
    {
    }
}


//字符串分割函数
std::vector<std::string> ReadJSHelper::split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();
    for(int i=0; i<size; i++)
    {
       pos=str.find(pattern,i);
       if(pos<size)
       {
          std::string s=str.substr(i,pos-i);
          result.push_back(s);
          i=pos+pattern.size()-1;
       }
    }
     return result;
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

bool ReadJSHelper::FileIsExist(const char *filePath)
{
    //#define F_OK 0 /* Test for existence. */
    return (access(filePath, 0) == 0);
}

std::wstring ReadJSHelper::c2w(const char *pc)
{
    std::wstring val = L"";  

    if(NULL == pc)  
    {  
        return val;  
    }  
    //size_t size_of_ch = strlen(pc)*sizeof(char);  
    //size_t size_of_wc = get_wchar_size(pc);  
    size_t size_of_wc;  
    size_t destlen = mbstowcs(0,pc,0);  
    if (destlen ==(size_t)(-1))  
    {  
        return val;  
    }  
    size_of_wc = destlen+1;  
    wchar_t * pw  = new wchar_t[size_of_wc];  
    mbstowcs(pw,pc,size_of_wc);  
    val = pw;  
    delete pw;  
    return val; 
}

void ReadJSHelper::DealCommandLine()
{
	GetCommand(GetCommandLine());
	Read();
	cocos2d::CCFileUtils::sharedFileUtils()->addSearchPath(m_Resources.c_str());
}
