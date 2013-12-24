package cn.cocostudio.viewer;
 
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.List;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.net.Uri;
import android.os.Binder;
import android.os.Build;
import android.os.Environment;
import android.os.IBinder;
import android.os.Message;
import android.widget.Button;

import com.mozillaonline.providers.DownloadManager;
import com.mozillaonline.providers.DownloadManager.Request;
import com.mozillaonline.providers.downloads.DownloadService;
 
public class ConnectService extends Service 
{

    Button mStartDownloadButton;
    static DownloadManager mDownloadManager;
	
	Intent qrReader;
	
	private static boolean isConnecting = false;
	
	private Thread mThreadClient = null;
	private static Socket mSocketClient = null;
	static BufferedReader mBufferedReaderClient	= null;
	static PrintWriter mPrintWriterClient = null;
	private String recvMessageClient = "";
	private String[] ipport ={};
	private static long mDownloadId = 0;
	
    @Override
    public void onCreate() 
    {        	
    	mThreadClient = new Thread(mRunnable);
        // 创建 SimpleBinder
        sBinder = new SimpleBinder();
        mDownloadManager = new DownloadManager(getContentResolver(),
				getPackageName());
        startDownloadService();
    }
    
    
	@Override
    public void onStart(Intent intent, int startId) {
        super.onStart(intent, startId);
    }
     
    @Override
    public void onDestroy() {
		try {
			
			mSocketClient.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        super.onDestroy();
    }
    
    public class SimpleBinder extends Binder{
        /**
         * 获取 Service 实例
         * @return
         */
        public ConnectService getService(){
            return ConnectService.this;
        }
        
        public void setIP(String[] ip)
        {
        	ipport = ip;
        	mThreadClient.start();
        }
    }
     
    public SimpleBinder sBinder;
    
     
    @Override
    public IBinder onBind(Intent intent) {
        // 返回 SimpleBinder 对象
        return sBinder;
    }
    
    
    public void WriteSharedInfo(String key,String value)
	{
		SharedPreferences sp = getSharedPreferences("Cocos2dxPrefsFile",Context.MODE_PRIVATE);
		Editor editor = sp.edit();
		editor.putString(key, value);
		editor.commit();
	}
    
    private void startDownloadService() 
    {
    	Intent intent = new Intent();
    	intent.setClass(this, DownloadService.class);
    	startService(intent);
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
	
	private static String getSavePath()
	{
	    String sdcardPath = getSystemStorage();
	    if(sdcardPath == null)
	    {
	    	return null;
	    }
	    sdcardPath += "/SceneEditorPreview";
		return sdcardPath;
	}
	
	public static void DeleteFile()
	{
		String path = getSavePath();
		File file = new File(path);
	    if(!file.exists()) {
	    	return;
	    }
	    else DeleteFile(file);
	}
	
	private void startDownload(String url)
	{
		endRender();
		
		Uri srcUri = Uri.parse(url);
		DownloadManager.Request request = new Request(srcUri);

		DeleteFile();
		String path = getSavePath();
		if(path == null) return;
	    File file = new File(path);
	    if(!file.exists()) {
	    	file.mkdirs();
	    }
	    //create download folder
	    String absolutePath = file.getAbsoluteFile()+"/Resources";
	    new File(absolutePath).mkdirs();
	    
	    WriteSharedInfo("savepath",absolutePath);
	    
	    request.setDestinationFromBase(file, "/");
		
		request.setDescription("");
		Notice("Download");
		sendDownloadStartMsg();
		mDownloadId = mDownloadManager.enqueue(request);
	    }
	    
	    
      
	//线程:监听服务器发来的消息
	private Runnable mRunnable = new Runnable() 
	{
		public void run()
		{			
			try 
			{			
				int port = Integer.parseInt(ipport[1]);	
				//连接服务器
				mSocketClient = new Socket(ipport[0], port);
				//取得输入、输出流
				mBufferedReaderClient = new BufferedReader(new InputStreamReader(mSocketClient.getInputStream()));
				
				mPrintWriterClient = new PrintWriter(mSocketClient.getOutputStream());
			
				isConnecting = true;
				
				recvMessageClient = "已经连接server!\n";//消息换行
				
				
				Message msg = new Message();
                msg.what = 0;
                handleMessage(msg);		
			}
			catch (Exception e) 
			{
				NoticeError("Connect Error");
				recvMessageClient = "连接IP异常:" + e.toString() + e.getMessage() + "\n";//消息换行
				Message msg = new Message();
                msg.what = 2;
                handleMessage(msg);	
				return;
			}			

			char[] buffer = new char[256];
			int count = 0;
			while (isConnecting)
			{
				try
				{
					if((count = mBufferedReaderClient.read(buffer))>0)
					{						
						recvMessageClient = getInfoBuff(buffer, count) + "\n";//消息换行

						recvMessageClient = MsgFilter(recvMessageClient);
						Message msg = new Message();
		                msg.what = 1;
		                handleMessage(msg);	
					}
					else
					{
						removeDownloadTask();
						isConnecting = false;
						Notice("Server Close");
					}
				}
				catch (Exception e)
				{
					recvMessageClient = "接收异常:" + e.getMessage() + "\n";//消息换行
					Message msg = new Message();
	                msg.what = 2;
	                handleMessage(msg);	
				} 
			}
			Notice("Server Close");
		}
	};
	

	public static void removeDownloadTask()
	{
		if(mDownloadId != 0)
		{
			mDownloadManager.remove(mDownloadId);
			mDownloadId = 0;
		}
	}
	
	public static void disconncetNetwork()
	{
		isConnecting = false;
	}
	 
	private String MsgFilter(String src)
	{
		int index = src.indexOf("[");
		String resultStr = src.substring(index+1, src.length());
		index = resultStr.indexOf("]");
		resultStr = resultStr.substring(0, index);
		return resultStr;
	}
	  
	//send msg 
	private void Notice(String info)
	{
		Intent broastIntent = new Intent();
	    broastIntent.putExtra("state", info);
	    broastIntent.setAction("com.cocoviewer.state");
	    sendBroadcast(broastIntent);
	}
	
	private void NoticeError(String info)
	{
		Intent broastIntent = new Intent();
		broastIntent.putExtra("log",info);
		broastIntent.putExtra("state","Error");
	    broastIntent.setAction("com.cocoviewer.state");
	    sendBroadcast(broastIntent);
	}
	
	private int getRenderIndex()
	{
		ActivityManager activityManager = (ActivityManager) this
				  .getSystemService(Context.ACTIVITY_SERVICE);
		  List<RunningAppProcessInfo> appProcesses = activityManager
				  .getRunningAppProcesses(); 
		  int index = 0;
		  for (RunningAppProcessInfo appProcess : appProcesses)
		  {
			  index++;
			  if (appProcess.processName.equals("cn.cocostudio.render"))
			  {
				  return index;
			  }
		  }
		  return -1;
	}
	
	public static void sendRenderExitMsg()
	{
		if(isConnecting)
			sendMsg("DISRENDER\n");
	}
	public static void sendTaskCancleMsg()
	{
		if(isConnecting)
			sendMsg("TASKCANCLE\n");
	}
	private void sendDownloadStartMsg()
	{
		if(isConnecting)
			sendMsg("STARTDOWNLOAD\n");
	}
    
    public void handleMessage(Message msg)										
	{		
	      //LOGIN
		  if(msg.what == 0)
		  {
			  //get Device ID
			  Build bd = new Build();
			  String model = bd.MODEL;
			  System.out.println("Client: "+recvMessageClient);
			  sendMsg("LOGIN 137EECCB-4239-47FA-A2E5-9C51F9C0A98F "+model+"\n");
		  }		 
		  //ERROR
		  else if(msg.what == 1)
		  {
			  String[] MsgInfo = recvMessageClient.split(" ");
			  if(MsgInfo[0].equals("LOGINOK"))
			  {
				  Notice("Server Start");
				  sendMsg("GETINFO\n");
			  }
			  else if(MsgInfo[0].equals("STOP"))
			  {
				  removeDownloadTask();
				  //exit render scene
				  endRender();
			  }
			  else if(MsgInfo[0].equals("CHECKCONNECTION"))
			  {

			  }
			  else if(MsgInfo[0].equals("INFO"))
			  {
				  String url = null;
				  //filter and write download info 
				  for(int i = 1;i<MsgInfo.length;i++)
				  {
					  String tempString[] = MsgInfo[i].split("@");
					  if(tempString.length!=2) continue;
					  
					  if(tempString[0].equals("URL")) url = tempString[1];
					  if(tempString[0].equals("PROJNAME")) WriteSharedInfo("projectname",tempString[1]);
					  if(tempString[0].equals("HEIGHT")) WriteSharedInfo("height",tempString[1]);
					  if(tempString[0].equals("WIDTH")) WriteSharedInfo("width",tempString[1]); 
				  }
				  //finished init info and start to download file
				  startDownload(url);
			  }
			  else
			  {
				  System.out.println("Client: "+ "undefine Msg " + MsgInfo[1]);  
			  }
		  }
		  //ERROR
		  else
		  {
			  System.out.println("Client: "+recvMessageClient);
		  }
	}					
    
    private void endRender()
    {
		if(getRenderIndex() == -1) return;
		Intent intent = new Intent();
		intent.setAction("cn.coco.viewer.stop");
		sendBroadcast(intent);
    }

	private String getInfoBuff(char[] buff, int count)
	{
		char[] temp = new char[count];
		for(int i=0; i<count; i++)
		{
			temp[i] = buff[i];
		}
		return new String(temp);
	}
		
    private static void sendMsg(String msg)
    {
      	mPrintWriterClient.print(msg);//发送给服务器
    	mPrintWriterClient.flush();
    }
    
	private static void DeleteFile(File file) {

		if (file.isFile()) 
		{
			file.delete();
			return;
		}
			if (file.isDirectory()) {
			File[] childFile = file.listFiles();
			if (childFile == null || childFile.length == 0) {
			file.delete();
			return;
		}
			for (File f : childFile) {
			DeleteFile(f);
		}
			file.delete();
		}
	}
 
}