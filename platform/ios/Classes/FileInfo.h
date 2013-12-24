//
//  FileInfo.h
//  CCSPreviewer
//
//  Created by Solo on 13-10-28.
//
//

#ifndef __CCSPreviewer__FileInfo__
#define __CCSPreviewer__FileInfo__

#include <iostream>

class FileInfo
{
public:
    static FileInfo* sharedFileInfo();
    FileInfo();
    void setFileInfo(const char* pUrl,const char* pVersion,const char* pFilename,int height,int width);
    int getTimes(){return times;}
    void setActivityLock(bool lock){activityLock=lock;times++;}
    bool getActivityLock(){return activityLock;}

    const char* getURL(){return url.c_str();}
    const char* getFilename(){return filename.c_str();}
	void setFilename(const char* name){ filename = name; }
    const char* getVersion(){return version.c_str();}
    int  getHeight(){return screenHeight;}
    int  getWidth(){return screenWidth;}
    void setHeight(int width){ screenHeight = width;}
    void setWidth(int height){ screenWidth = height;}
    
private:
    std::string url;
    std::string filename;
    std::string version;
    int screenHeight;
    int screenWidth;
    bool activityLock;
    int times;
};


#endif /* defined(__CCSPreviewer__FileInfo__) */
