package cn.cocostudio.render;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class MyBroadcastReceiver extends BroadcastReceiver{
	private static native void exitCocos();
	@Override
	public void onReceive(Context context, Intent intent)
	{
		exitCocos();
	}
}
