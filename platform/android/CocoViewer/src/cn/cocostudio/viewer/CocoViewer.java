package cn.cocostudio.viewer;

import java.io.File;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.os.IBinder;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;


public class CocoViewer extends Activity 
{
	@SuppressWarnings("unused")
    private static final String TAG = CocoViewer.class.getName();

	Intent qrReader;
	
	BroadcastReceiver mReceiver;
	
	private String[] ipport ={};
	Intent renderIntent; 
	private ServiceConnection sc;
	public static boolean isConnect = false;
	public TextView stateText = null;
	public ImageView stateConnect = null;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		ExitApplication.getInstance().addActivity(this);
		setContentView(R.layout.activity_coco_viewer);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
		renderIntent = new Intent();
		renderIntent.setComponent(new ComponentName("cn.cocostudio.render","cn.cocostudio.render.CocoRender"));

		initButtons();
		initService();
	}
	
	@Override
	public void onStart()
	{
		initBroadcastReceiver();
		super.onStart();
	}
	
	@Override
	protected void onResume()
	{
		setResetButtonEnable(true);
		setButtonEnable(true);
		super.onResume();
	}
	
	private void initBroadcastReceiver()
	{
		mReceiver = new BroadcastReceiver() {

		    @Override
		    public void onReceive(Context context, Intent intent) {
		    	
		    	String action = intent.getAction();
		    	if("com.cocoviewer.state".equals(action)){
		    		String state = intent.getStringExtra("state");
		    		if(state.equals("Server Start"))
		    		{
		    			isConnect = true;
		    			ConnectNotice();
		    			Notice("Press Play in CocoStudio");
		    		}
		    		else if(state.equals("Server Close"))
		    		{
		    			exitService();
		    			isConnect = false;
		    			ConnectNotice();
		    			ProgressNotice("0",false);
		    			setButtonEnable(true);
		    			
		    		}
		    		else if(state.equals("Download"))
		    		{
		    			Notice("Downloading");
		    			setButtonEnable(false);
		    			ProgressNotice("0",true);
		    		}
		    		else if(state.equals("Log"))
		    		{
		    			String info = intent.getStringExtra("info");
		    			if(info.startsWith("Uncompressing")) 
		    			{
		    				SmallNotice(info);
		    			}
		    			else if(info.equals("Uncompress Finished"))
			    		{
			    			Notice("Start to Render");
			    			ProgressNotice("0",false);
			    			changeToRender();
			    		}
		    			else if(info.startsWith("progress"))
		    			{
		    				String[] str = info.split(" ");
		    				if(str.length == 2) ProgressNotice(str[1],true);
		    			}
	    				else Notice(info);
		    			
		    		}
		    		else if(state.equals("Error"))
		    		{
		    			String log = intent.getStringExtra("log");
		    			if(log.equals("Task Cancle")) 
		    			{
		    				ConnectService.sendTaskCancleMsg();
		    				ProgressNotice("0",false);
		    			}
		    			Notice(log);
		    			setButtonEnable(true);
		    		}
		    		else Notice("Undefine Msg");
		    	}
		    }
		};
		IntentFilter intentFilter = new IntentFilter();
		intentFilter.addAction("com.cocoviewer.state");
		registerReceiver(mReceiver, intentFilter);
	}
	
	@Override
	public void onStop()
	{
		unregisterReceiver(mReceiver);
		super.onStop();
	}
	
	private void initButtons()
	{
		stateText = (TextView)findViewById(R.id.viewerstate);
		stateConnect = (ImageView)findViewById(R.id.connectstate);
		Button disconnect = (Button)findViewById(R.id.reset);
		disconnect.setOnClickListener(new ResetListener());
		
		Button camera = (Button)findViewById(R.id.connect);
		camera.setOnClickListener(new ConnectListener());
		
		Button render = (Button)findViewById(R.id.render);
		render.setOnClickListener(new RenderListener());
		
		Button input = (Button)findViewById(R.id.input);
		input.setOnClickListener(new InputListener());
		
		ImageButton exit = (ImageButton)findViewById(R.id.exit);
		exit.setOnClickListener(new ExitListener());
	}
	
	public void changeToRender()
	{	 
		if(!isAppInstalled("cn.cocostudio.render"))
		{
			Notice("Plase Install CocoRender");
			return;
		}
		//set appindex to let Service check for render isExsist
		SharedPreferences sp = getSharedPreferences("Cocos2dxPrefsFile",Context.MODE_PRIVATE);
		renderIntent.putExtra("savepath", sp.getString("savepath",null));
		renderIntent.putExtra("projectname",sp.getString("projectname",null));
		renderIntent.putExtra("height",sp.getString("height","320"));
		renderIntent.putExtra("width",sp.getString("width","480"));
		startActivity(renderIntent);
	}
    
	private void setButtonEnable(boolean clickable)
	{
		Button camera = (Button)findViewById(R.id.connect);
		camera.setEnabled(clickable);
		Button render = (Button)findViewById(R.id.render);
		render.setEnabled(clickable);
		Button input = (Button)findViewById(R.id.input);
		input.setEnabled(clickable);
	}
	
	private void setResetButtonEnable(boolean clickable)
	{
		Button reset = (Button)findViewById(R.id.reset);
		reset.setEnabled(clickable);
	}
	
	
	public void connectService(String ip)
	{
		  ipport = ip.split(":");
	  	  if(ipport.length!=2)
	  	  {
	  		  Notice("IP Error");
	  		  return;
	  	  }
			if(isConnect) 
			{
				exitService();
			}
	  	  WriteSharedInfo("default",ip);
	  	  this.startService(new Intent(this, ConnectService.class));
	  	  this.bindService(new Intent(CocoViewer.this, ConnectService.class), sc, Context.BIND_AUTO_CREATE);
	}
	
	//init button listeners
	private class ConnectListener implements OnClickListener{
		public void onClick(View arg0) 
		{
			if(!isAppInstalled("com.google.zxing.client.android"))
			{
				Notice("Plase Install BarcodeScanner");
				return;
			}
			
		    qrReader = new Intent("com.google.zxing.client.android.SCAN");
		    qrReader.putExtra("SCAN_MODE", "QR_CODE_MODE");
		    startActivityForResult(qrReader, 0);    //Barcode Scanner to scan for us  
		}
	}
	
	private class InputListener  implements OnClickListener
	{
		public void onClick(View arg0) 
		{
			createEditBox();
		}
	}
	
	private void createEditBox()
	{
		final EditText inputServer = new EditText(this);
		SharedPreferences sp = getSharedPreferences("Cocos2dxPrefsFile",Context.MODE_PRIVATE);
		String info = sp.getString("default",null);
		if(info != null) inputServer.setText(info);
		else inputServer.setText("192.168.");
		inputServer.setSelection(inputServer.getText().length());
		
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Input Your IP").setIcon(R.drawable.ic_launcher).setView(inputServer)
                .setNegativeButton("Cancel", null);
        builder.setPositiveButton("Ok", new DialogInterface.OnClickListener() 
        {
            public void onClick(DialogInterface dialog, int which)
            {
               String ip = inputServer.getText().toString();
               if(ip == null) 
            	   {
	            	   Notice("NULL IP");
	            	   return;
            	   }
               connectService(ip);
            }
        });
        builder.show();
	}
	
	private class ResetListener implements OnClickListener{
		public void onClick(View arg0) 
		{
			ProgressNotice("0",false);
			Notice("Press Connect to Start");
			ConnectService.DeleteFile();
			ConnectService.removeDownloadTask();
			setButtonEnable(true);
		}
	}
	
	private class RenderListener implements OnClickListener
	{
		@Override
		public void onClick(View v)
		{	  
			if(getFileReady())
			{
				setButtonEnable(false);
				setResetButtonEnable(false);
				changeToRender();
			}
			else Notice("Cannot Find Resources!");
		}	
	}
	private class ExitListener implements OnClickListener
	{
		@Override
		public void onClick(View v)
		{	
	    	exitService();
			ExitApplication.getInstance().exit();
		}
	}
	
	
	public void onActivityResult(int requestCode, int resultCode, Intent intent) 
	{
		    if (requestCode == 0) 
		    {
		      if (resultCode == RESULT_OK) 
		      {		
		    	  String ip = intent.getStringExtra("SCAN_RESULT");
		    	  connectService(ip);
		      } 
		      else if (resultCode == RESULT_CANCELED) 
		      {
		    	  System.out.println("Press a button to start a scan.");
		    	  System.out.println("Scan cancelled.");
		      }
		  }
	}
	
	private void initService()
	{
	      sc = new ServiceConnection() 
	      {
	    	  ConnectService.SimpleBinder sBinder;
	          @Override
	          public void onServiceDisconnected(ComponentName name)
	          {
	          }
			@Override
			public void onServiceConnected(ComponentName name, IBinder service) {
	        	  sBinder = (ConnectService.SimpleBinder)service;
	        	  sBinder.setIP(ipport);
			}
	      };
	}
	  
	private boolean getFileReady()
	{
	    String sdcardPath = getSystemStorage();
	    if(sdcardPath == null)
	    {
	    	return false;
	    }
	    
	    sdcardPath += "/SceneEditorPreview";
	    File file = new File(sdcardPath);
	    if(!file.exists()) {
	    	return false;
	    }
	    //create download folder
	    String absolutePath = file.getAbsoluteFile()+"/Resources";
	    File fileres = new File(absolutePath);
	    if(!fileres.exists()){
	    	return false;
	    }
	    return true;
	}
	
	private static String getSystemStorage() {
		String str = null;
		boolean sdCardExist = Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED); //判断sd卡是否存在 
		if(sdCardExist)
		{
			File sdcardPath = Environment.getExternalStorageDirectory();
			str = sdcardPath.getAbsolutePath();
		}
		return str;
	}

	public void Notice(final String notice)
	{
		stateText.setTextSize(23);
		stateText.setText(notice);
	}
	public void SmallNotice(final String notice)
	{
		stateText.setTextSize(13);
		stateText.setText(notice);
	}
	public void ConnectNotice()
	{
		if(isConnect)	stateConnect.setImageResource(R.drawable.connectsuccessful);
		else stateConnect.setImageResource(R.drawable.disconnect);
	}
	private void ProgressNotice(String percent,boolean isDisplay)
	{
		ProgressBar bar = (ProgressBar)findViewById(R.id.progress); 
		if(isDisplay == false) 
			bar.setVisibility(View.GONE);
		else 
		{
			bar.setVisibility(View.VISIBLE);
			int progress = Integer.parseInt(percent);
			if(progress == 100) setResetButtonEnable(false);
			bar.setProgress(progress);
		}
	}
	
	public void WriteSharedInfo(String key,String value)
	{
		SharedPreferences sp = getSharedPreferences("Cocos2dxPrefsFile",Context.MODE_PRIVATE);
		Editor editor = sp.edit();
		editor.putString(key, value);
		editor.commit();
	}
	
    private void exitService()
    {
    	ConnectService.removeDownloadTask();
    	if(isConnect) 
    	{
            isConnect=false;
            ConnectNotice();
    		setButtonEnable(true);
    		
    		ConnectService.disconncetNetwork();
    		unbindService(sc);
    		stopService(new Intent(this, ConnectService.class));
    	}
    }
    
	
	private boolean isAppInstalled(String uri) {
		PackageManager pm = getPackageManager();
		boolean installed =false;
		try 
		{
			pm.getPackageInfo(uri,PackageManager.GET_ACTIVITIES);
			installed =true;
		} catch(PackageManager.NameNotFoundException e) 
		{
			installed =false;
		}
		return installed;
	}
	
	
    @Override
	protected void onDestroy()
    {
		super.onDestroy();
	}
}
