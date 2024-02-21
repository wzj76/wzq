package com.duole.jniutil;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.util.HashMap;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxLuaJavaBridge;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.Intent;
import android.net.Uri;
import android.util.Log;
import android.widget.Toast;

import com.duole.update.AppDownloadUtil;

import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLSocketFactory;

public class CheckUpdate {
	//检查更新参数
	public static final String DUOLE_GAME_ID = "8"; 	
	public static final String CHECK_URL = "https://update.duole.com/__enc/apk/check";
	
	public static final String REQ_PARAM_GAMEID = "gameid";
	public static final String REQ_PARAM_CHANNEL = "channel";
	public static final String REQ_PARAM_VERSIONCODE = "version_code";
	public static final String REQ_PARAM_PACKAGENAME = "package_name";
	public static final String REQ_PARAM_APKSIGNATURE = "apk_signature";
	
	public static final String RESP_PARAM_CODE = "code";
	public static final String RESP_PARAM_VERSIONNAME = "version_name";
	public static final String RESP_PARAM_UPDATELOG = "update_log";
	public static final String RESP_PARAM_APKURL = "apk_url";
	public static final String RESP_PARAM_APKMD5 = "apk_md5";
	public static final String RESP_PARAM_APKSIZE = "apk_size";
	
	private static String _loadApdUrl = "";
	//
	private static Cocos2dxActivity thisActivity = null;	
	
	public static void init(Cocos2dxActivity activity) {
		thisActivity = activity;
	}
	

		
	//检查更新
	public static void doManualCheckUpdate(){
		Log.i("ghome", "------doManualCheckUpdate");
		new Thread(new Runnable() {
			
			@Override
			public void run() {
				try {
					HashMap<String, String> map = new HashMap<String, String>();
					map.put(REQ_PARAM_GAMEID, DUOLE_GAME_ID);
					map.put(REQ_PARAM_CHANNEL, SysUtil.getChannel());
					map.put(REQ_PARAM_VERSIONCODE, SysUtil.getVersionCode());
					map.put(REQ_PARAM_PACKAGENAME, SysUtil.getPackageName());
					map.put(REQ_PARAM_APKSIGNATURE, SysUtil.getApkSignatureMd5());
					String params = SysUtil.generateHttpRequestParams(map);
					
					Log.i("ghome", "---channel=" + SysUtil.getChannel() + "     packagename=" + SysUtil.getPackageName() + "    md5=" + SysUtil.getApkSignatureMd5());
					// 新建一个URL对象
					URL url = new URL(CHECK_URL);
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
					conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
					// 开始连接
					conn.connect();
		            // 发送请求参数
		            DataOutputStream os = new DataOutputStream(conn.getOutputStream());
		            os.write(params.getBytes());
					os.flush();
					os.close();
					Log.i("ghome", "-------getResponseCode = " + conn.getResponseCode());
					if (conn.getResponseCode() == 200) {
						onCheckUpdateResult(true, convertStreamToString(conn.getInputStream()));
					} else {
						onCheckUpdateResult(false, "");
					}
					conn.disconnect();
				} catch (Exception e) {
					e.printStackTrace();
					onCheckUpdateResult(false, "");
				}
			}
		}).start();	
	}
	
	//手动更新
	public static void doManualUpdate(){
		Log.i("ghome", "------checkVersionUpdate=" + _loadApdUrl);
		// Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(_loadApdUrl));
		// thisActivity.startActivity(intent);
		AppDownloadUtil.download(_loadApdUrl, "多乐跑胡子", "正在下载新版本", "多乐跑胡子更新", false);
	}
	
	private static String convertStreamToString(InputStream is) {
		try {
			ByteArrayOutputStream os = new ByteArrayOutputStream();
			byte[] buffer = new byte[1024];
			int len = 0;
			while ((len = is.read(buffer)) != -1) {
				os.write(buffer, 0, len);
			}
			os.close();
			String sRsp = new String(os.toByteArray());
			return SysUtil.Rot47Manager(sRsp);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return "";
	}
	
	private static void onCheckUpdateResult(boolean succeed, String resp) {
		if (succeed) {
			try {
				JSONObject json = new JSONObject(resp);
				int code = json.getInt(RESP_PARAM_CODE);
				Log.i("ghome", "-----code=" + code);
				if (code == 0) {
					final String updateLog = json.getString(RESP_PARAM_UPDATELOG);
					final String versionName = json.getString(RESP_PARAM_VERSIONNAME);
					final String url = json.getString(RESP_PARAM_APKURL);
					checkVersionUpdate(versionName, updateLog, url);
					return;
				}
			} catch (JSONException e) {
				e.printStackTrace();
			}
		}
	}
	
	private static void checkVersionUpdate(final String versionName, final String updateLog, final String url){
		String oldVersion = SysUtil.getInstance().getVersionName();
		Log.i("ghome", "------oldVersion=" + oldVersion + "   newVersionName=" + versionName);
		if(!oldVersion.equals(versionName))
		{
			Log.i("ghome", "-----1");
			_loadApdUrl = url;
			thisActivity.runOnGLThread(new Runnable() {				
				@Override
				public void run() {		
					Log.i("ghome", "---------- do onCheckVersionUpdateResult");
					onCheckVersionUpdateResult(versionName, updateLog);
				}
			});
		}				
	}
	
	
	//检查更新
	private static native void onCheckVersionUpdateResult(String resultText, String updateLog);
	
}
