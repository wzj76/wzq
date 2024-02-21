package com.duole.update;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Timer;
import java.util.TimerTask;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;
import org.json.JSONObject;

import android.annotation.SuppressLint;
import android.app.DownloadManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.support.v4.content.FileProvider;
import android.util.Log;

import com.duole.jniutil.PlatformFunction;

public class AppDownloadUtil {
	private static Cocos2dxActivity thisActivity = null;
	private static ProgressHandler m_pProgressHandler = null;
	// DownloaderManager下载任务唯一标识
	private static long m_nDownloadRefId = -1;
	private static String m_sFilename = "";
	private static String m_sFileDir = "";
	private static boolean m_bFileReady = false;
	private static boolean m_bInProgress = false;
	private static boolean m_bInBack = false;
	private static String m_sTitle = "";
	// 静默下载
	private static boolean m_bSilent = true;
	// 计时器
	private static Timer m_pTimer = null;
	private static TimerTask m_pTask = null;
	// DownloadManger包名：检查是否可用
	private static final String DOWNLOAD_MANAGER_PACKAGE_NAME = "com.android.providers.downloads";
	private static final String FILE_EXT_APK = ".apk";
	private static final String DOWNLOAD_RECORD = "download_record.ref";
	/**
	 * 初始化
	 * @param activity Cocos2dxActivity实例
	 */
	public static void init(Cocos2dxActivity activity, ProgressHandler handler) {
		thisActivity = activity;
		m_pProgressHandler = handler;
	}
	/**
	 * 销毁
	 */
	public static void destroy() {
		thisActivity = null;
		m_pProgressHandler = null;
	}
	/**
	 * 是否允许下载
	 * @return 是否允许下载
	 */
	@SuppressLint("InlinedApi") public static Boolean isDownloadEnabled() {
		// 检查是否允许读写
		Boolean bRet = Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED);
		// 检查DownloadManager是否启用
		if (bRet) {
			m_sFileDir = thisActivity.getExternalFilesDir(Environment.DIRECTORY_DOWNLOADS).getAbsolutePath() + "/";
			int nState = thisActivity.getPackageManager().getApplicationEnabledSetting(DOWNLOAD_MANAGER_PACKAGE_NAME);
			// 
			bRet = nState != PackageManager.COMPONENT_ENABLED_STATE_DISABLED;
			if (bRet) {
				int nSdkInt = Build.VERSION.SDK_INT;
				if (nSdkInt >= 14) {
					bRet = nState != PackageManager.COMPONENT_ENABLED_STATE_DISABLED_USER;
				}
				if (bRet && nSdkInt >= 18) {
					bRet = nState != PackageManager.COMPONENT_ENABLED_STATE_DISABLED_UNTIL_USED;
				}
			}
		} else  {
			m_sFileDir = "";
		}
		return bRet;
	}
	/**
	 * Apk是否已经存在
	 * @param sFilename 文件名称
	 * @return
	 */
	public static Boolean isApkExisted(final String sFilename) {
		String sFilePath = getDownloadFilePath(sFilename, FILE_EXT_APK);
		File file = new File(sFilePath);
		Log.d("AppDownloadUtil", "isApkExisted >>> fullpath=" + sFilePath);
		Log.d("AppDownloadUtil", "isApkExisted >>> exists=" + file.exists());
		return file.exists();
	}
	/**
	 * 下载Apk
	 * @param sApkUrl Apk地址
	 * @param bSilent 是否静默
	 */
	public static void download(final String sApkUrl, final String sNotifyTitle, final String sNotifyDesc, final String sDialogTitle, final boolean bSilent) {
		if (!isDownloadEnabled() || !isNetworkReady(bSilent)) {
			clearDownload();
			if (!bSilent) {
				PlatformFunction.openURL(sApkUrl);
			}
			return;
		}
		m_sTitle = sDialogTitle;
		// 下载完成
		String sFilename = getMD5(sApkUrl);
		// 新的下载
		if (!sFilename.equals(m_sFilename)) {
			clearDownload();
			// 查询下载记录
			String sOldUrl = "";
			long nOldRef = -1;
			String sJson = getRecordInfo();
			if (sJson != null && sJson.length() > 0) {
				JSONObject obJson = getJsonFromString(sJson);
				try {
					if (obJson.has("url")) {
						sOldUrl = obJson.getString("url");
					}
					if (obJson.has("ref")) {
						nOldRef = obJson.getLong("ref");
					}
				} catch (JSONException e) {
					Log.d("AppDownloadUtil", "JSONException");
					e.printStackTrace();
				}
			}
			if (sApkUrl.equals(sOldUrl) && nOldRef != -1) {
				// 已经存在的下载
				m_bSilent = bSilent;
				m_nDownloadRefId = nOldRef;
				m_sFilename = sFilename;
				if (!m_bSilent) {
					updateProgress(sDialogTitle, m_nDownloadRefId);
				}
				return;
			}
			// 更新包有变动
			if (sOldUrl != null && !sApkUrl.equals(sOldUrl) && nOldRef != -1) {
				clearTimer();
			}
			if (sOldUrl != null && sOldUrl.length() > 0) {
				String sOldFilename = getMD5(sOldUrl);
				removeLastFiles(sOldFilename, nOldRef);
			}
			// 下载更新包
			downloadApk(sApkUrl, sFilename, sNotifyTitle, sNotifyDesc, sDialogTitle, bSilent);
			return;
		}
		m_bSilent = bSilent;
		m_bInBack = false;
		// 静默加载
		startProgress();
	}

	public static void inBack() {
		m_bInBack = true;
		hideProgress();
	}

	public static void hideProgress() {
		m_bInProgress = false;
	}
	
	public static void showProgress() {
		m_bInProgress = true;
	}
	
	private static void startProgress() {
		if (!m_bInBack && !m_bInProgress) {
			m_pProgressHandler.progressBegin(m_sTitle, m_bSilent);
		}
	}

	private static void updateProgress(final String sTitle, final long nRefId) {
		clearTimer();
		m_pTimer = new Timer();
		m_pTask  = new TimerTask() {
			@Override
			public void run() {				
				DownloadManager.Query dmQuery = new DownloadManager.Query();
				dmQuery.setFilterById(nRefId);
				Cursor cursor = ((DownloadManager)thisActivity.getSystemService(Context.DOWNLOAD_SERVICE)).query(dmQuery);
				if (cursor != null && cursor.moveToFirst()) {
					int nIdxStatus = cursor.getColumnIndex(DownloadManager.COLUMN_STATUS);
					if (nIdxStatus != -1 && cursor.getInt(nIdxStatus) == DownloadManager.STATUS_SUCCESSFUL) {
						clearTimer();
						cursor.close();
						downloadComplete(m_sFilename);
						return;
					}
					int nIdxDownloaded = cursor.getColumnIndex(DownloadManager.COLUMN_BYTES_DOWNLOADED_SO_FAR);
					int nIdxTotal = cursor.getColumnIndex(DownloadManager.COLUMN_TOTAL_SIZE_BYTES);
					if (nIdxDownloaded >= 0 && nIdxTotal > 0) {
						long nBytesDone = cursor.getLong(nIdxDownloaded);
						long nBytesAll = cursor.getLong(nIdxTotal);
						cursor.close();
						final int nProgress = (int)(nBytesDone * 100 / nBytesAll);
						startProgress();
						Log.d("AppDownloadUtil", "进度="  + nBytesDone + "/" + nBytesAll);
						Log.d("AppDownloadUtil", "进度="  + nProgress + "%");
						m_pProgressHandler.updateProgress(sTitle, nProgress);
					} else {
						cursor.close();
					}
				}
			}
			
		};
		m_pTimer.schedule(m_pTask, 0, 500);
	}

	/**
	 * 获得MD5字符串
	 * @param sText
	 * @return MD5字符串或原字符串
	 */
	public static String getMD5(final String sText) {
		String sRet = sText;
		try {
			MessageDigest digest = java.security.MessageDigest.getInstance("MD5");
			digest.update(sText.getBytes());
			byte[] msgDigests = digest.digest();
			StringBuffer hexStr = new StringBuffer();
			for (int i = 0; i < msgDigests.length; i ++)  {
				hexStr.append(Integer.toHexString(0xFF & msgDigests[i]));
				sRet = hexStr.toString();
			}
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		}
		return sRet;
	}
	/**
	 * 下载Apk
	 * @param sUrl 下载地址
	 * @param sFilename 文件名（不含扩展）
	 * @param sNotifyTitle 通知栏标题
	 * @param sNotifyDesc 通知栏描述
	 * @param bSilent 是否静默下载
	 */
	private static void downloadApk(final String sUrl, final String sFilename, final String sNotifyTitle, final String sNotifyDesc, final String sDialogTitle, final Boolean bSilent) {
		clearDownload();
		m_sFilename = sFilename;
		m_bSilent = bSilent;
		// 获取DownloadManager对象
		DownloadManager downloadManager = (DownloadManager)thisActivity.getSystemService(Context.DOWNLOAD_SERVICE);  
		// 创建DownloadManager请求
		DownloadManager.Request request = new DownloadManager.Request(Uri.parse(sUrl));
		// 设置允许下载的网络环境
		if (bSilent) {			
			request.setAllowedNetworkTypes(DownloadManager.Request.NETWORK_WIFI);
			// 设置不允许漫游状态执行操作
			request.setAllowedOverRoaming(false);
		}
		// 设置下载中可见
		request.setVisibleInDownloadsUi(false);
		request.setTitle(sNotifyTitle);
		request.setDescription(sNotifyDesc);
		String sFileFullname = sFilename + FILE_EXT_APK;
		Log.d("AppDownloadUtil", "downloadApk >>> url=" + sUrl);
		Log.d("AppDownloadUtil", "downloadApk >>> fullname=" + sFileFullname);
		request.setDestinationInExternalFilesDir(thisActivity, Environment.DIRECTORY_DOWNLOADS, sFileFullname);
		m_nDownloadRefId = downloadManager.enqueue(request);
		saveToRecordFile(sUrl, m_nDownloadRefId);
		Log.d("AppDownloadUtil", "downloadApk >>> refid=" + m_nDownloadRefId);
		if (!bSilent) {
			updateProgress(sDialogTitle, m_nDownloadRefId);
		}
	}
	/**
	 * 网络是否就绪
	 * @param bSilent 静默下载
	 * @return 是否就绪
	 */
	private static Boolean isNetworkReady(final Boolean bSilent) {
		Boolean bRet = false;
		ConnectivityManager pConnMgr = (ConnectivityManager) thisActivity.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo wifi = pConnMgr.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
		bRet = wifi.isAvailable() && wifi.isConnected();
		if (bSilent) {
			return bRet;
		}
		if (!bRet) {
			NetworkInfo mobile = pConnMgr.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
			bRet = mobile.isAvailable() && mobile.isConnected();
		}
		return bRet;
	}
	/** 清理下载 */
	private static void clearDownload() {
		m_sFilename = "";
		m_nDownloadRefId = -1;
		m_bFileReady = false;
		m_bInProgress = false;
	}
	/**
	 * 下载完成
	 * @param sFilename 文件名字
	 */
	private static void downloadComplete(final String sFilename) {
		if (m_bFileReady) {
			return;
		}
		m_bFileReady = true;
		m_bInProgress = false;
		m_bInBack = false;
		m_pProgressHandler.progressFinish();
		Log.d("AppDownloadUtil", "downloadComplete >>> filename=" + sFilename);
		Log.d("AppDownloadUtil", "downloadComplete >>> fileready=" + m_bFileReady);
		String sPathApk = getDownloadFilePath(sFilename, FILE_EXT_APK);
		File fileApk = new File(sPathApk);
		clearDownload();
		if (fileApk.exists()) {
			afterDownload(sFilename);
		}
	}
	/**
	 * 下载完成后
	 * @param sFilename 文件名
	 */
	private static void afterDownload(final String sFilename) {
		Log.d("AppDownloadUtil", "afterDownload >>> silent=" + m_bSilent);
		if (!m_bSilent) {
			installApk(sFilename);
		}
		clearDownload();
	}
	/**
	 * 安装Apk
	 * @param sFilename 文件名
	 */
	private static void installApk(final String sFilename) {
		String sFilePath = getDownloadFilePath(sFilename, FILE_EXT_APK);
		File file = new File (sFilePath);
		Log.d("AppDownloadUtil", "installApk >>> file exsits=" + file.exists());
		if (!file.exists()) {
			Log.e("AppDownloadUtil", "安装文件丢失，请稍后重试");
			return;
		}
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
			installApkN(file);
		} else {
			installApk(file);
		}
	}
	/**
	 * Android 1.x-6.x
	 * @param file
	 */
	private static void installApk(final File file) {
		Intent intent = new Intent(Intent.ACTION_VIEW);
		intent.setDataAndType(Uri.fromFile(file), "application/vnd.android.package-archive");
		intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK); // without this flag android returned a intent error!
		thisActivity.startActivity(intent);
	}
	/**
	 * Android 7.x-
	 * @param file
	 */
	private static void installApkN(final File file) {
		Uri apkUri = FileProvider.getUriForFile(thisActivity, thisActivity.getPackageName() + ".provider", file);
		Intent intent = new Intent(Intent.ACTION_VIEW);
		intent.setDataAndType(apkUri, "application/vnd.android.package-archive");
		intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK); // without this flag android returned a intent error!
		thisActivity.startActivity(intent);
	}
	/**
	 * 获得下载文件路径
	 * @param sFilename 文件名
	 * @param sFileExt	扩展名
	 * @return 文件完整路径
	 */
	private static String getDownloadFilePath(final String sFilename, final String sFileExt) {
		return m_sFileDir + sFilename + sFileExt;
	}
	/**
	 * 获取记录文件的Json字符串
	 * @return Json字符串
	 */
	private static String getRecordInfo() {
		String sRet = "";
		String sFilePath = m_sFileDir + DOWNLOAD_RECORD;
		File file = new File(sFilePath);
		if (file.exists()) {
			try {
				FileInputStream ins = new FileInputStream(file);
				byte[] bytes = new byte[ins.available()];
				ins.read(bytes);
				ins.close();
				sRet = new String(bytes);
			} catch (FileNotFoundException e) {
				Log.e("AppDownloadUtil", "FileNotFoundException");
				e.printStackTrace();
			} catch (IOException e) {
				Log.e("AppDownloadUtil", "IOException");
			}
		}
		return sRet;
	}
	/**
	 * 根据字符串获取Json对象
	 * @param sJson Json字符串
	 * @return Json对象
	 */
	private static JSONObject getJsonFromString(final String sJson) {
		JSONObject pRet = new JSONObject();
		try {
			pRet = new JSONObject(sJson);
		} catch(JSONException e) {
			Log.e("AppDownloadUtil", "JSONException");
			e.printStackTrace();
		}
		return pRet;
	}
	/**
	 * 保存记录文件
	 * @param sUrl 下载地址
	 * @param nRefId 下载的ReferenceId
	 */
	private static void saveToRecordFile(final String sUrl, final long nRefId) {
		String sFilePath = m_sFileDir + DOWNLOAD_RECORD;
		File file = new File(sFilePath);
		try {
			FileOutputStream fos = new FileOutputStream(file);
			JSONObject json = new JSONObject();
			json.put("url", sUrl);
			json.put("ref", nRefId);
			String sOutput = json.toString();
			fos.write(sOutput.getBytes());
			fos.close();
		} catch (FileNotFoundException e) {
			Log.e("AppDownloadUtil", "FileNotFoundException");
			e.printStackTrace();
		} catch (JSONException e) {
			Log.e("AppDownloadUtil", "JSONException");
			e.printStackTrace();
		} catch (IOException e) {
			Log.e("AppDownloadUtil", "IOException");
			e.printStackTrace();
		}
	}
	/**
	 * 清理定时器和定时任务
	 */
	private static void clearTimer() {
		if (m_pTimer != null) {
			m_pTimer.cancel();
			m_pTimer.purge();
			m_pTimer = null;
		}
		if (m_pTask != null) {
			m_pTask.cancel();
			m_pTask = null;
		}
	}
	/**
	 * 删除旧下载
	 * @param sFilename 文件名（无扩展名）
	 * @param nRefId 对应的下载ReferenceId
	 */
	private static void removeLastFiles(final String sFilename, final long nRefId) {
		if (nRefId != -1) {
			DownloadManager downloadManager = (DownloadManager)thisActivity.getSystemService(Context.DOWNLOAD_SERVICE); 
			downloadManager.remove(nRefId);
		}
		String sFilePath = getDownloadFilePath(sFilename, FILE_EXT_APK);
		File file = new File(sFilePath);
		if (file.exists()) {
			file.delete();
		}
	}
}
