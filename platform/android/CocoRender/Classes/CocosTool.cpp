#include <jni.h>
#include "../../Classes/HelloWorldScene.h"

using namespace cocos2d;

extern "C" void Java_cn_cocostudio_render_MyBroadcastReceiver_exitCocos(JNIEnv *, jclass, jstring)
	{
		CloseScene(false);
	}
