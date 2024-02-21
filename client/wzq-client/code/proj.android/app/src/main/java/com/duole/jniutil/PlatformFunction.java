package com.duole.jniutil;

import java.util.HashMap;
import java.util.Map;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.reflect.Method;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHelper;

import com.duole.sdk.shareSdk.ShareSdkUtil;
import com.duole.sdk.umeng.UMengUtil;
import com.lahm.library.EasyProtectorLib;
import com.duole.paohuzihd.R;

import android.app.AlarmManager;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.ClipData;
import android.content.ClipDescription;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.os.Vibrator;
import android.provider.MediaStore;
import android.util.Log;

import cn.udesk.UdeskSDKManager;
import cn.udesk.config.UdeskConfig;
import udesk.core.UdeskConst;

public class PlatformFunction {
	 
	private static Cocos2dxActivity thisActivity = null;
	private static BroadcastReceiver batteryChangedReceiver = null;
	private static float batteryLevel = 0;
	private static boolean emulator = false;

	// 剪切板
	private static ClipboardManager clipboard  = null;
	private static ClipboardManager.OnPrimaryClipChangedListener clipboardListener = null;
	private static String clipText = "";
	
	public static void init(Cocos2dxActivity activity) {
		thisActivity = activity;

		emulator = EasyProtectorLib.checkIsRoot();
		
		batteryChangedReceiver = new BroadcastReceiver() {
			
			@Override
			public void onReceive(Context context, Intent intent) {
				int level = intent.getIntExtra("level", -1);
				int scale = intent.getIntExtra("scale", -1);
				if (level >= 0 && scale > 0) {
					batteryLevel = (float)level / scale;
				}
			}
		};
		thisActivity.registerReceiver(batteryChangedReceiver, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
		// 剪切板初始化
		initClipboard();
	}
	
	public static void destroy() {
		thisActivity.unregisterReceiver(batteryChangedReceiver);
		batteryChangedReceiver = null;

		// 剪切板
		if (clipboard != null) {
			clipboard.removePrimaryClipChangedListener(clipboardListener);
		}
		clipboard = null;
		clipboardListener = null;
		
		thisActivity = null;
	}

	public static void openURL(final String url) {
		thisActivity.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
				thisActivity.startActivity(intent);
			}
		});
	}
	
	public static void openApplication(final String packageName, final String sDownLoadLink) {
        thisActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                boolean bSuccess = false;
                try {
					PackageManager manager = thisActivity.getPackageManager();
					PackageInfo packageInfo = manager.getPackageInfo(packageName, 0);
					if (null != packageInfo) {
						thisActivity.startActivity(manager.getLaunchIntentForPackage(packageName));
						thisActivity.finish();
						Cocos2dxHelper.terminateProcess();
						bSuccess = true;
					}
                } catch (Exception e) {
                	bSuccess = false;
				}
                if (!bSuccess)
                {
                    openURL(sDownLoadLink);
                }
            }
        });
    }
	
	public static boolean canOpenApplication(final String packageName) {
		boolean bRet = false;
		try {
			PackageManager manager = thisActivity.getPackageManager();
			PackageInfo packageInfo = manager.getPackageInfo(packageName, 0);
			bRet = packageInfo != null;
		} catch (Exception e) {
			bRet = false;
		}
		return bRet;
    }
	
	public static float getBatteryLevel() {
		return batteryLevel;
	}

	public static void initClipboard() {
		// 剪切板
		clipboard = (ClipboardManager) thisActivity.getSystemService(Context.CLIPBOARD_SERVICE);
		clipboardListener = new ClipboardManager.OnPrimaryClipChangedListener() {
			@Override
			public void onPrimaryClipChanged() {
				thisActivity.getWindow().getDecorView().post(new Runnable() {
					@Override
					public void run() {
						if (clipboard == null || !clipboard.hasPrimaryClip()) {
							return;
						}
						ClipDescription clipDesc = clipboard.getPrimaryClipDescription();
						if (clipDesc == null || !clipDesc.hasMimeType(ClipDescription.MIMETYPE_TEXT_PLAIN)) {
							return;
						}
						ClipData clipData = clipboard.getPrimaryClip();
						if (clipData == null || clipData.getItemCount() == 0) {
							return;
						}
						ClipData.Item item = clipData.getItemAt(0);
						if (item == null || item.getText() == null) {
							return;
						}
						clipText = item.getText().toString();
					}
				});
			}
		};
		if (clipboard != null) {
			clipboard.addPrimaryClipChangedListener(clipboardListener);
			clipboardListener.onPrimaryClipChanged();
		}
	}

	public static void onResume() {
		if (clipboard == null) {
			initClipboard();
		}
		if(clipboardListener != null) {
			clipboardListener.onPrimaryClipChanged();
		}
	}
	
	public static void onActivityResult(int requestCode, int resultCode, Intent data) {

	}
	
	public static void copyText(final String sText) {
		thisActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if(clipboard == null) {
					initClipboard();
				}
				if (clipboard != null) {
					clipboard.setPrimaryClip(ClipData.newPlainText(null, sText));
				}
			}
		}); 
    }
	
	public static boolean joinQQGroup(final String sUrl){
		try{
			Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(sUrl));
			thisActivity.startActivity(intent);
			return true;
		}catch(Exception e){
			
		}
		return false;
	}

	public static void share(int platform, String shareURL, String text, String title, String taskid) {
		ShareSdkUtil.share(shareURL,platform,text,title,"", taskid);
	}
	
	public static void shareMiniGame(int platform, String shareURL, String text, String title, String taskid) {
		ShareSdkUtil.shareMini(shareURL,text,"",title,taskid);
	}

	public static void shareImage(int platform,String shareURL, String absoluteImagePath) {
		ShareSdkUtil.shareImage(absoluteImagePath,platform,"-1");
	}
	
	public static void showFeedBack(final String userid, final String device){
		Log.i("ghome", "showFeedback: " + userid + "_" + SysUtil.getInstance().getChannel() + "_" + SysUtil.getInstance().getVersionName() + "_" + device);
		thisActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				// 反馈游戏名称
				String gameName = "跑胡子";
				// 设备标识
				String deviceId = device;
				// 反馈用户昵称
				String nickName = userid;
				if (nickName.isEmpty()) {
					nickName = String.format("单机用户@%s", gameName);
				} else {
					nickName = String.format("联网用户%s@%s", nickName, gameName);
				}
				Map<String, String> defaultInfo = new HashMap<String, String>();
				defaultInfo.put(UdeskConst.UdeskUserInfo.USER_SDK_TOKEN, deviceId);
				defaultInfo.put(UdeskConst.UdeskUserInfo.NICK_NAME, nickName);
				// 配置客户自定义信息
				Map<String, String> definedInfo = new HashMap<String, String>();
				// 应用信息
				String appInfo = String.format("%s/%s/%s", thisActivity.getString(R.string.app_name), SysUtil.getInstance().getVersionName(), SysUtil.getInstance().getVersionCode());
				definedInfo.put("TextField_1871", appInfo);
				// 渠道信息
				String channelInfo = String.format("%s/%s", SysUtil.getInstance().getChannel(), "unknown");
				definedInfo.put("TextField_1872", channelInfo);
				// 设备信息
				String deviceInfo = deviceId;
				definedInfo.put("TextField_1873", deviceInfo);
				// 初始化
				UdeskSDKManager.getInstance().initApiKey(thisActivity, PlatformFunction.getUdeskDomain(), PlatformFunction.getUdeskAppKey(), PlatformFunction.getUdeskAppId());
				// 创建和更新客户信息
				UdeskConfig.Builder builder = new UdeskConfig.Builder();
				// 创建和更新客户基本信息
				builder.setDefualtUserInfo(defaultInfo);
				builder.setUpdateDefualtUserInfo(defaultInfo);
				// 创建和更新客户自定义信息
				builder.setDefinedUserTextField(definedInfo);
				builder.setUpdatedefinedUserTextField(definedInfo);
				// 启动
				UdeskSDKManager.getInstance().entryChat(thisActivity.getApplicationContext(), builder.build(), deviceId);
			}
		});
	}
	
	public static boolean isEmulator(){
		return emulator;
	}	
	
	// 删除指定本地推送
	public static void removeAllLocalNotification()
	{
		NotificationManager mNotifyMgr = (NotificationManager) thisActivity.getSystemService(Context.NOTIFICATION_SERVICE);
		mNotifyMgr.cancelAll();
	}

	// 添加本地推送
	public static void addLocalNotification(String sText, String sTitle, long nDelay, String sKey)
	{
        Intent intent = new Intent(thisActivity, NotificationReceiver.class);
        intent.setClass(thisActivity, NotificationReceiver.class);
        intent.setData(Uri.parse(sKey)); // 通知名字,作为通知id使用
        intent.putExtra("msg", "play_hskay");
        intent.putExtra("content", sText); // 通知内容
        PendingIntent pi = PendingIntent.getBroadcast(thisActivity, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);
        AlarmManager am = (AlarmManager) thisActivity.getSystemService(Context.ALARM_SERVICE);
        am.set(AlarmManager.RTC_WAKEUP , System.currentTimeMillis() + nDelay * 1000, pi); 
	}
	//手机振动
    public static void shake()
    {
		Vibrator vib = (Vibrator) thisActivity
				.getSystemService(Context.VIBRATOR_SERVICE);
		vib.vibrate(300);
    }

    //获取可读取路径
    public static String getSaveWritePath()
    {
    	File dir = thisActivity.getCacheDir();
		// 手机装有SDCard,并且可以进行读写
		if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())  
	            || !Environment.isExternalStorageRemovable()) {
			dir = thisActivity.getExternalCacheDir();
		}

		String path = dir.getAbsolutePath();
    	return path;
    }
    
    //获取相册读取图片保存地址
    public static String getSaveSpritePath()
    {
    	File dir = thisActivity.getCacheDir();
		// 手机装有SDCard,并且可以进行读写
		if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())  
	            || !Environment.isExternalStorageRemovable()) {
			dir = Environment.getExternalStorageDirectory();
		}

		String path = dir.getAbsolutePath();
    	return path;
    }
    
    public static void onSaveImage(final String sPath)
    {
        thisActivity.runOnUiThread(new Runnable() {
            
            @Override
            public void run() {
                if (sPath == null || sPath.equals("")) {
                    return;
                }
                
                Bitmap bmp = BitmapFactory.decodeFile(sPath);
                File appDir = new File(Environment.getExternalStorageDirectory(), "PaoHuZi");
                if (!appDir.exists()) {
                    appDir.mkdir();
                }
                String fileName = System.currentTimeMillis() + ".jpg";
                File file = new File(appDir, fileName);
                try {
                    FileOutputStream fos = new FileOutputStream(file);
                    bmp.compress(CompressFormat.JPEG, 100, fos);
                    fos.flush();
                    fos.close();
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                
                // 其次把文件插入到系统图库
                try {
                    MediaStore.Images.Media.insertImage(thisActivity.getContentResolver(),
                                                        file.getAbsolutePath(), fileName, null);
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                }
                final String saveAs = Environment.getExternalStorageDirectory().getPath() + "/PaoHuZi/" + fileName;
                Uri contentUri = Uri.fromFile(new File(saveAs));
                // 最后通知图库更新
                thisActivity.sendBroadcast(new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE, contentUri));
                boolean success = true;
                try
                {
                    File savefile = new File(saveAs);
                    if (!savefile.exists())
                    {
                        success = false;
                    }
                }catch (Exception e){
                    success = false;
                }
            }
        });
    }
    
    public static boolean hasNotch()
	{
		boolean hasNotch = false;
    	//oppo 判断是否有刘海
		String brand = (Build.BRAND).toUpperCase();
		if (brand.equals("OPPO"))
		{
			hasNotch = thisActivity.getPackageManager().hasSystemFeature("com.oppo.feature.screen.heteromorphism");
		}
		else if (brand.equals("VIVO"))
		{
			 try {
	             ClassLoader cl = thisActivity.getClassLoader();
	             Class ftFeature = cl.loadClass("android.util.FtFeature");
	             Method[] methods = ftFeature.getDeclaredMethods();
	             if (methods != null) {
	                 for (int i = 0; i < methods.length; i++) {
	                     Method method = methods[i];
	                     if (method.getName().equalsIgnoreCase("isFeatureSupport")) {
	                         hasNotch = (Boolean) method.invoke(ftFeature, 0x00000020);
	                         break;
	                     }
	                 }
	             }
	         } catch (Exception e) {
	             e.printStackTrace();
	             hasNotch = false;
	         }
		}
		else if (brand.equals("XIAOMI"))
		{
			String propName = "ro.miui.notch";
	        String line = "";
	        BufferedReader input = null;
	        try {
	            Process p = Runtime.getRuntime().exec("getprop " + propName);
	            input = new BufferedReader(
	                    new InputStreamReader(p.getInputStream()), 1024);
	            line = input.readLine();
	            input.close();
	        } catch (IOException ex) {
	        	
	        } finally {
	            if (input != null) {
	                try {
	                    input.close();
	                } catch (IOException e) {
	                    e.printStackTrace();
	                }
	            }
	        }
	        if (line.equals("1"))
	        {
	        	hasNotch = true;
	        }
		}
        //华为 不设置显示刘海区的话 系统自动适配，不做特殊处理
        return hasNotch;
    }

    public static void doWeChatAuth() {
		ShareSdkUtil.doWeChatAuth();
    }
    
    public static boolean isMusicActive() {
    	return false;
    }
    
	public static void onSaveSchemesResult(final String path, final String data) {
		thisActivity.runOnGLThread(new Runnable() {
			
			@Override
			public void run() {
				onSaveSchemesData(path, data);
			}
		});
	}

	public static final String getCopyText() {
		String sText = clipText;
		return sText;
	}

	public static void exitGame() {
		UMengUtil.onFinish();
		thisActivity.finish();
		ShareSdkUtil.onFinish();
		System.exit(0);
	}
	
    public static native void onAuthFinish(String accessToken, String refreshToken, String openId, boolean error);
    
	public static native String getUmengAppKey();
	
	public static native String getWechatAppId();
	public static native String getWechatAppSecret();
	public static native String getWechatMiniId();
	public static native String getQQAppId();
	public static native String getQQAppSecret();
	public static native String getUdeskDomain();
	public static native String getUdeskAppId();
	public static native String getUdeskAppKey();
	public static native String getChannelPackage();
	public static native String getServerHost();
	//分享结果
	public static native void onShareResult(boolean success, int taskId);
	//手机系统截图
	public static native void screenShotComplete();
	//事件上报版本号
	public static native String getReportEventVersion();
	public static native void onSaveSchemesData(String path, String data);
}
