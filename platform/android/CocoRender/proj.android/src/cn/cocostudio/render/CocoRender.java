/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

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
package cn.cocostudio.render;


import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;

public class CocoRender extends Cocos2dxActivity
{
	static Context mContext;
    protected void onCreate(Bundle savedInstanceState)
    {
    	Intent intent = getIntent();  
        WriteSharedInfo("savepath",intent.getStringExtra("savepath"));
		WriteSharedInfo("projectname",intent.getStringExtra("projectname"));
		WriteSharedInfo("height",intent.getStringExtra("height"));
		WriteSharedInfo("width",intent.getStringExtra("width")); 
		super.onCreate(savedInstanceState);	
		mContext = getApplicationContext();
	}
    
	public void WriteSharedInfo(String key,String value)
	{
		SharedPreferences sp = getSharedPreferences("Cocos2dxPrefsFile",Context.MODE_PRIVATE);
		Editor editor = sp.edit();
		editor.putString(key, value);
		
		editor.commit(); 
	}

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// RenderScene should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }
    
    public static void exitRenderScene()
    {
    	Intent intent = new Intent();
    	intent.setAction("cn.coco.render.stop");
    	mContext.sendBroadcast(intent);
    }
    
    static 
    {
        System.loadLibrary("cocos2dcpp");
    }   

}
