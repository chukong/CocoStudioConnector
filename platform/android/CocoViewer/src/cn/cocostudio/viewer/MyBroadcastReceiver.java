package cn.cocostudio.viewer;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class MyBroadcastReceiver extends BroadcastReceiver
{

	private static native void exitCocos();
	private Context mContext;
	@Override
	public void onReceive(Context context, Intent intent) 
	{
		ConnectService.sendRenderExitMsg();
	}
}
