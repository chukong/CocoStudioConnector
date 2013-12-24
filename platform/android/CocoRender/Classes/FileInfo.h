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

    const char* getSavePath(){return savePath.c_str();}
    const char* getFilename(){return filename.c_str();}
    int getHeight(){return screenHeight;}
    int getWidth(){return screenWidth;}
    

private:
    std::string savePath;
    std::string filename;
    int screenHeight;
    int screenWidth;
};


#endif /* defined(__CCSPreviewer__FileInfo__) */
