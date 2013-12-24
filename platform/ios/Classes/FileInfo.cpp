//
//  FileInfo.cpp
//  CCSPreviewer
//
//  Created by Solo on 13-10-28.
//
//

#include "FileInfo.h"


static FileInfo* m_sharedFileInfo = NULL;

FileInfo* FileInfo::sharedFileInfo()
{
    if (m_sharedFileInfo == NULL)
    {
        m_sharedFileInfo = new FileInfo();
    }
    return m_sharedFileInfo;
}

FileInfo::FileInfo()
{
	times=0;
	activityLock = false;
	screenHeight = 640;
	screenWidth = 960;
    url = "";
    filename = "";
    version = "";
}

void FileInfo::setFileInfo(const char* pUrl,const char* pVersion,const char* pFilename,int width,int height)
{
	url = std::string(pUrl);
	filename = std::string(pFilename);
	version = std::string(pVersion);
    screenHeight = height;
    screenWidth = width;
}
