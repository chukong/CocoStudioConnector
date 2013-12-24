//
//  FileInfo.cpp
//  CCSPreviewer
//
//  Created by Solo on 13-10-28.
//
//

#include "FileInfo.h"
#include "cocos2d.h"

using namespace cocos2d;

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
    savePath = CCUserDefault::sharedUserDefault()->getStringForKey("savepath");
	screenHeight =  atof(CCUserDefault::sharedUserDefault()->getStringForKey("height").c_str());
	screenWidth = atof(CCUserDefault::sharedUserDefault()->getStringForKey("width").c_str());
    filename = CCUserDefault::sharedUserDefault()->getStringForKey("projectname");
}


