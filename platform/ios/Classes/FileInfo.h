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
