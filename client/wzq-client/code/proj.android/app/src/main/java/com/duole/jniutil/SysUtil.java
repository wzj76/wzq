package com.duole.jniutil;

import java.io.DataOutputStream;
import java.io.File;
import java.security.MessageDigest;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.util.HashMap;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiManager;
import android.os.Environment;
import android.provider.Settings;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.util.Log;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.BufferedWriter;
import java.io.OutputStreamWriter;

import com.duole.jniutil.PlatformFunction;
import android.content.ComponentName;
import com.duole.paohuzihd.AppActivity;

import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLSocketFactory;

public class SysUtil {
	// 常量定义
	public static final String TAG = "SysUtil";

	private static SysUtil instance = null;

	private static Activity context = null;


    private static String prePagename = "";
    private static String lastPagename = "";

	//信号强度
	public static int wifi_signalStrength = 0;
	public static int gsm_signalStrength = 0;
		
	/**
	 * A signal handler in native code has been triggered. As our last gasp,
	 * launch the crash handler (in its own process), because when we return
	 * from this function the process will soon exit.
	 */
	public static void nativeCrashed() {
		new RuntimeException("crashed here (native trace should follow after the Java trace)").printStackTrace();
	}

	public static Object rtnObject() {
		return SysUtil.getInstance();
	}

	public static SysUtil getInstance() {
		if (instance == null) {
			instance = new SysUtil();
		}
		return instance;
	}

	public void init(Activity activity) {
		context = activity;
	}

	public void destroy() {
		context = null;
	}
	
	/**
	 * 功能描述：获取渠道号
	 * @return 渠道号
	 */
	public static String getChannel() {
		String channel = null;
		ApplicationInfo info;
		try {
			info = context
					.getPackageManager()
					.getApplicationInfo(
							context.getPackageName(),
							PackageManager.GET_META_DATA);
			channel = info.metaData.getString("UMENG_CHANNEL");
		} catch (Exception e) {
			e.printStackTrace();
		}
		return channel;
	}

	/**
	 * 功能描述：获取签名信息
	 * @return 包名
	 */
	public static String getApkSignatureMd5() {
	    StringBuffer buffer = new StringBuffer();
	    try {
	        PackageInfo packageInfo = context.getPackageManager().getPackageInfo(getPackageName(), PackageManager.GET_SIGNATURES);
	        Signature[] signs = packageInfo.signatures;
	        byte[] signBytes = signs[0].toByteArray();
	        MessageDigest md5 = MessageDigest.getInstance("MD5");
	        byte[] md5Bytes = md5.digest(signBytes);
	        for (int i = 0; i < md5Bytes.length; i++) {
	            String hex = Integer.toHexString(md5Bytes[i] & 0xFF);
	            if (hex.length() == 1) {
	                buffer.append("0");
	            }
	            buffer.append(hex);
	        }
	    } catch (Exception e) {
	        e.printStackTrace();
	    }
	    return buffer.toString().toLowerCase();
	}

    public static void saveFileContent(String fileName,String data) {
        BufferedWriter writer = null;
        File dir = context.getCacheDir();
        String path = dir.getAbsolutePath();
        String createtimeFile = path + "/" + fileName + ".json";
        File file = new File(createtimeFile);
        //如果文件不存在，则新建一个
        if(!file.exists()){
            try {
                file.createNewFile();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        //写入
        try {
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(file,false), "UTF-8"));
            writer.write(data);
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            try {
                if(writer != null){
                    writer.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    //    System.out.println("文件写入成功！");
    }
	
	// 上报事件
    public static void reportPageEvent(final String pagename, final String playtype, final String screendir, final String result, final String deviceId, final String userId) {
		System.out.println("reportPageEvent = " + pagename);
		lastPagename = prePagename;
        prePagename = pagename;
        new Thread(new Runnable() {
             
             @Override
             public void run() {
                 try {
                     HashMap<String, String> map = new HashMap<String, String>();
                     map.put("sdkver", PlatformFunction.getReportEventVersion());
					 map.put("game", "paohuzi");
                     map.put("device", deviceId);
                     map.put("model", android.os.Build.MODEL);
                     map.put("channel", getChannel());
                     map.put("version", getVersionNameStatic());
                     map.put("platform", "android");
					 map.put("pagename", pagename);
					 map.put("lastpagename", lastPagename);
                     map.put("androidlevel", getAndroidSDkInt());
					 map.put("firstinstall", AppActivity.getFirstInstall());
                     map.put("wasttime", AppActivity.getTimeMillisAppStarted() + "");
                     map.put("userid", userId);
					 if (playtype != "")
					 {
						map.put("playtype", playtype);
					 }
					 if (screendir != "")
					 {
						 map.put("screentype", screendir);
					 }
					 if (result != "")
					 {
						 map.put("resulttype", result);
					 }
                     String params = generateHttpRequestParams(map);
                     // 新建一个URL对象
					 Log.d("", "服务器地址="+PlatformFunction.getServerHost());
                     URL url = new URL("https://event.duole.com/__enc/event/page");
                     // 打开一个HttpURLConnection连接
					 HttpsURLConnection conn = (HttpsURLConnection) url.openConnection();
					 conn.setSSLSocketFactory((SSLSocketFactory)SSLSocketFactory.getDefault());
					 // 设置连接超时时间
                     conn.setConnectTimeout(15 * 1000);
                     // 设置读取数据超时时间
                     conn.setReadTimeout(15 * 1000);
                     // POST请求不能使用缓存
                     conn.setUseCaches(false);
                     // POST请求必须设置允许输出，默认false
                     conn.setDoOutput(true);
                     // 设置请求允许输入，默认是true
                     conn.setDoInput(true);
                     // 设置本次连接是否自动处理重定向
                     conn.setInstanceFollowRedirects(true);
                     // 设置为POST请求
                     conn.setRequestMethod("POST");
                     // 配置请求Content-Type
                     conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded;charset=UTF-8");
                     // 开始连接
                     conn.connect();
                      // 发送请求参数
                     DataOutputStream os = new DataOutputStream(conn.getOutputStream());
                     os.write(params.getBytes());
                     os.flush();
                     os.close();
                     if (conn.getResponseCode() == 200) {
                         
                     } else {
                         
                     }
                     conn.disconnect();
                 } catch (Exception e) {
                     e.printStackTrace();
                     
                 }
             }
         }).start();	
    }

	public static void reportClickEvent(final String evtName, final String deviceId, final String userId) {
        new Thread(new Runnable() {
             
             @Override
             public void run() {
                 try {
                     HashMap<String, String> map = new HashMap<String, String>();
                     map.put("sdkver", PlatformFunction.getReportEventVersion());
					 map.put("game", "paohuzi");
                     map.put("device", deviceId);
                     map.put("model", android.os.Build.MODEL);
                     map.put("channel", getChannel());
                     map.put("version", getVersionNameStatic());
                     map.put("platform", "android");
					 map.put("evtname", evtName);
                     map.put("androidlevel", getAndroidSDkInt());
                     map.put("firstinstall", AppActivity.getFirstInstall());
                     map.put("userid", userId);
                     String params = generateHttpRequestParams(map);
                     
                     // 新建一个URL对象
					 Log.d("", "服务器地址="+PlatformFunction.getServerHost());
                     URL url = new URL("https://event.duole.com/__enc/event/click");
                     // 打开一个HttpURLConnection连接
					 HttpsURLConnection conn = (HttpsURLConnection) url.openConnection();
					 conn.setSSLSocketFactory((SSLSocketFactory)SSLSocketFactory.getDefault());
                     // 设置连接超时时间
                     conn.setConnectTimeout(15 * 1000);
                     // 设置读取数据超时时间
                     conn.setReadTimeout(15 * 1000);
                     // POST请求不能使用缓存
                     conn.setUseCaches(false);
                     // POST请求必须设置允许输出，默认false
                     conn.setDoOutput(true);
                     // 设置请求允许输入，默认是true
                     conn.setDoInput(true);
                     // 设置本次连接是否自动处理重定向
                     conn.setInstanceFollowRedirects(true);
                     // 设置为POST请求
                     conn.setRequestMethod("POST");
                     // 配置请求Content-Type
                     conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded;charset=UTF-8");
                     // 开始连接
                     conn.connect();
                      // 发送请求参数
                     DataOutputStream os = new DataOutputStream(conn.getOutputStream());
                     os.write(params.getBytes());
                     os.flush();
                     os.close();
                     if (conn.getResponseCode() == 200) {
                         
                     } else {
                         
                     }
                     conn.disconnect();
                 } catch (Exception e) {
                     e.printStackTrace();
                     
                 }
             }
         }).start();	
    }

	public static String generateHttpRequestParams(HashMap<String, String> map) {
		StringBuilder buffer = new StringBuilder();
		try {
			int pos = 0;
			for (String key : map.keySet()) {
				if (pos > 0) {
					buffer.append("&");
				}
				buffer.append(String.format("%s=%s", key, URLEncoder.encode(map.get(key), "UTF-8")));
				pos++;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return Rot47Manager(buffer.toString());
	}

	/**
     * ROT47 加密和解密.
     */
    public static String Rot47Manager(String data) {
        char[] chs = data.toCharArray();
        for (int i = 0; i < chs.length; i++) {
            char ch = chs[i];
            if (ch >= 33 && ch <= 126) {
                chs[i] = (char) (33 + (ch + 14) % 94);
            }
        }
        return String.valueOf(chs);
    }

	// 取设备信息
	public static String getMobileID() {
		System.out.println("getMobileID begin ");
		String sRet = Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID);
		return TextUtils.isEmpty(sRet) ? "__UNKNOWN__" : sRet;
	}
	
	// 取得手机号码
	public String getPhoneNumber() {
		TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
		return tm.getLine1Number();
	}

	// 取得IMSI
	public String getIMSI() {
		TelephonyManager tm = (TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE);
		return tm.getSubscriberId();
	}
	
	public String getCacheDir() {
		File cacheDir = context.getCacheDir();
		return cacheDir.getParent()
				+ File.separator
				+ cacheDir.getName()
				+ File.separator;
	}
	
	public String getExternalCacheDir() {
		File cacheDir = context.getExternalCacheDir();
		return cacheDir.getParent()
				+ File.separator
				+ cacheDir.getName()
				+ File.separator;
	}
	
	public String getFilesDir() {
		File fileDir = context.getFilesDir();
		return fileDir.getParent()
				+ File.separator
				+ fileDir.getName()
				+ File.separator;
	}
	
	public String getExternalStorageDirectory() {
		return Environment.getExternalStorageDirectory().getAbsolutePath()
				+ File.separator;
	}
	
	public static String getPackageName() {
		return context.getPackageName();
	}

	// 取得包版本号
	public static String getVersionNameStatic() {
		String version = "";
		try {
			// getPackageName()是你当前类的包名，0代表是获取版本信息
			PackageInfo packInfo = context.getPackageManager().getPackageInfo(getPackageName(), 0);
			version = packInfo.versionName;
		} catch (PackageManager.NameNotFoundException e) {
		}
		return version;
	}
	
	// 取得包版本号2
		public String getVersionName() {
			String version = "";
			try {
				// getPackageName()是你当前类的包名，0代表是获取版本信息
				PackageInfo packInfo = context.getPackageManager().getPackageInfo(getPackageName(), 0);
				version = packInfo.versionName;
			} catch (PackageManager.NameNotFoundException e) {
			}
			return version;
		}
	/**
	 * 功能描述：获取versioncode
	 * @return 当前应用版本号
	 */
	public static String getVersionCode() {
		String version = null;
		try {
			PackageManager manager = context
					.getPackageManager();
			PackageInfo info = manager.getPackageInfo(context
					.getPackageName(), 0);
			version = String.valueOf(info.versionCode);
			Log.i(version, "versionCode");
		} catch (Exception e) {
			e.printStackTrace();
		}
		return version;
	}
	// 取Application中meta-data
	public String getApplicationMetaData(String strName) {
		String strVal = "";
		try {
			ApplicationInfo appInfo = context.getPackageManager().getApplicationInfo(getPackageName(), PackageManager.GET_META_DATA);
			strVal = appInfo.metaData.getString(strName);
		} catch (PackageManager.NameNotFoundException e) {
		}
		return strVal;
	}

	// 获取手机型号
	public String getBuildModel() {
		return android.os.Build.MODEL;
	}

	// 获取手机厂商
	public String getBuildManufacturer() {
		return android.os.Build.MANUFACTURER;
	}
	
    // 取版本号
    public static String getAndroidSDkInt() {
        String sRet = "";
        try {
            sRet = android.os.Build.VERSION.SDK_INT + "";
        } catch (Exception e) {

        }
        return sRet;
    }

	public String ShowUrl(String strURL) {
		String strVal = "";
		Intent intent = new Intent();
		intent.setAction("android.intent.action.VIEW");
		intent.setData(Uri.parse(strURL));
		context.startActivity(intent);
		return strVal;
	}
	
	// 判断网络是否有效
	public boolean isNetworkConnected() {
		ConnectivityManager mConnectivityManager = (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo mNetworkInfo = mConnectivityManager.getActiveNetworkInfo();
		if (mNetworkInfo != null) {
			return mNetworkInfo.isAvailable();
		}
		return false;
	}

	// 判断WIFI网络是否可用
	public static boolean isWifiConnected() {
		ConnectivityManager mConnectivityManager = (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo mWiFiNetworkInfo = mConnectivityManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
		if (mWiFiNetworkInfo != null) {
			return mWiFiNetworkInfo.isAvailable();
		}
		return false;
	}
	
	// 判断MOBILE网络是否可用
	public static boolean isMobileConnected() {
		ConnectivityManager mConnectivityManager = (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo mMobileNetworkInfo = mConnectivityManager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
		if (mMobileNetworkInfo != null) {
			return mMobileNetworkInfo.isAvailable();
		}
		return false;
	}
	
	// 获取当前网络连接的类型信息
	public static int getConnectedType() {
		ConnectivityManager mConnectivityManager = (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo mNetworkInfo = mConnectivityManager.getActiveNetworkInfo();
		if (mNetworkInfo != null && mNetworkInfo.isAvailable()) {
			if(mNetworkInfo.getType()==ConnectivityManager.TYPE_WIFI){
				return 2;
		    }else if(mNetworkInfo.getType()==ConnectivityManager.TYPE_MOBILE) {
		    	return 1;
		    }
		}
		return -1;
	}

	public static int getSignalStrength(){
		if (getConnectedType() == 0){
			return getSignalStrengthGsm();
		}
		else{
			return getSignalStrengthWifi();
		}
	}
	
	public static void doOpenSetting(){
		// 跳转到系统的网络设置界面  
		Intent intent = new Intent(android.provider.Settings.ACTION_SETTINGS);
		context.startActivity(intent);
	}
    
    // 跳转到对应的评论界面
    public static void doOpenComment(){
		try {
			String mAddress = "market://details?id=" + getPackageName();
    		Intent marketIntent = new Intent("android.intent.action.VIEW");
    		marketIntent.setData(Uri.parse(mAddress ));
    		//不同的渠道需要设置
    		marketIntent.setPackage(PlatformFunction.getChannelPackage());
    		context.startActivity(marketIntent);
		} catch (Exception e) {
			e.printStackTrace();
		}
    }

    public static boolean openWeChat(){
		try{
			Intent intent = new Intent();
			ComponentName cmp = new ComponentName("com.tencent.mm","com.tencent.mm.ui.LauncherUI");
			intent.setAction(Intent.ACTION_MAIN);
			intent.addCategory(Intent.CATEGORY_LAUNCHER);
			intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			intent.setComponent(cmp);
			context.startActivity(intent);
			return true;
		}catch(Exception e){
			return false;
		}
	}
    
	//_gsm_signalStrength: gsm信号强度
	public void setSignalStrengthGsm(int _gsm_signalStrength){
		gsm_signalStrength = _gsm_signalStrength;
	}

	public static int getSignalStrengthGsm(){
		if (isMobileConnected()){
			gsm_signalStrength = 4;
		}
		else{
			gsm_signalStrength = 0;
		}
		return gsm_signalStrength;
	}
	//_wifi_signalStrength: wifi信号强度
	public static void setSignalStrengthWifi(int _wifi_signalStrength){
		wifi_signalStrength = _wifi_signalStrength;
	}

	public static int getSignalStrengthWifi(){
		int level = 0;
		// bugfix:
		// Error: The WIFI_SERVICE must be looked up on the Application context or memory will leak on devices < Android N.
		// Try changing context to context.getApplicationContext() [WifiManagerLeak]
		// level = 100 + ((WifiManager)context.getSystemService(Context.WIFI_SERVICE)).getConnectionInfo().getRssi();
		level = 100 + ((WifiManager)context.getApplicationContext().getSystemService(Context.WIFI_SERVICE)).getConnectionInfo().getRssi();
		level = (int)Math.max(level, 0);
		return level;
	}	
}
