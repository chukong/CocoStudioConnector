#include "main.h"
#include "../Classes/AppDelegate.h"
#include "CCEGLView.h"
#include "ReadJSHelper.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();

	ReadJSHelper::Instance()->DealCommandLine();

    eglView->setFrameSize(ReadJSHelper::Instance()->GetWinSize().width, ReadJSHelper::Instance()->GetWinSize().height);
	int ret =  CCApplication::sharedApplication()->run();
	ReadJSHelper::Destroy();
	return ret;
}
