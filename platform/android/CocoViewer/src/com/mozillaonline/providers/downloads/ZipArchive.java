package com.mozillaonline.providers.downloads;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import android.content.Context;
import android.content.Intent;


public class ZipArchive {

    private ZipFile zipFile;
    private Context mContext;

    public ZipArchive(ZipFile zipFile)
    {
        this.zipFile = zipFile;
    }

    public ZipArchive(String zipFilePath,Context context) throws IOException
    {
    	this.mContext = context;
    	try
    	{
    		this.zipFile = new ZipFile(zipFilePath);
    	}
    	catch(IOException e)
    	{
        	NoticeError("Task Cancle");
    	}
    }

    public void close() throws IOException
    {
        zipFile.close();
    }
    
    public void unzip(String extractPath) throws IOException
    {
    	if(zipFile == null) 
    	{
    		NoticeError("Task Cancle");
    		return;
    	}
        File extractionDirectory = new File(extractPath);

        if(!extractionDirectory.exists() && !extractionDirectory.mkdirs()) {
            throw new IOException("Unable to create extraction directory");
        }
        if(!extractionDirectory.isDirectory()) {
            throw new IOException("Unable to extract ZipFile to non-directory");
        }

        Enumeration<? extends ZipEntry> zipEntries = zipFile.entries();
        while(zipEntries.hasMoreElements()) {
            ZipEntry zipEntry = zipEntries.nextElement();
            Notice("Uncompressing "+zipEntry.getName());
            if(zipEntry.isDirectory()) {
                File newDirectory = new File(extractPath + zipEntry.getName());
                newDirectory.mkdirs();
            }
            else {
                BufferedInputStream inputStream = new BufferedInputStream(zipFile.getInputStream(zipEntry));

                File outputFile = new File(extractPath + zipEntry.getName());
                File outputDirectory = new File(outputFile.getParent());

                if(!outputDirectory.exists() && !outputDirectory.mkdirs()) {
                    throw new IOException(new StringBuilder("unable to create directory for ").append(zipEntry.getName()).toString());
                }

                if(!outputFile.exists() && !outputFile.createNewFile()) {
                    throw new IOException(new StringBuilder("Unable to create file for ").append(zipEntry.getName()).toString());
                }

                BufferedOutputStream outputStream = new BufferedOutputStream(new FileOutputStream(outputFile));
                try {
                    int currByte;
                    while((currByte = inputStream.read()) != -1) {
                        outputStream.write(currByte);
                    }
                }
                finally {
                    outputStream.close();
                    inputStream.close();
                }
            }
        }
        close();
        Notice("Uncompress Finished");
    }
    
    private void Notice(String info)
	{
		  Intent broastIntent = new Intent();
		  broastIntent.putExtra("state", "Log");
		  broastIntent.putExtra("info", info);
	      broastIntent.setAction("com.cocoviewer.state");
	      mContext.sendBroadcast(broastIntent);
	}
	private void NoticeError(String info)
	{
		  Intent broastIntent = new Intent();
	      broastIntent.putExtra("state", "Error");
	      broastIntent.putExtra("log", info);
	      broastIntent.setAction("com.cocoviewer.state");
	      mContext.sendBroadcast(broastIntent);
	}
	
    
    public List<String> getFilePaths()
    {
        ArrayList<String> filePaths = new ArrayList<String>();

        Enumeration<? extends ZipEntry> zipEntries = zipFile.entries();
        while(zipEntries.hasMoreElements()) {
            ZipEntry zipEntry = zipEntries.nextElement();
            filePaths.add(zipEntry.getName());
        }

        return filePaths;
    }

    public InputStream getFileInputStream(String filePath) throws IOException
    {
        ZipEntry entry = zipFile.getEntry(filePath);

        if(entry == null) {
            throw new FileNotFoundException((new StringBuilder("Unable to find file ").append(filePath).append(" in zipfile ").append(zipFile.getName()).toString()));
        }

        return new BufferedInputStream(zipFile.getInputStream(entry));
    }
}
