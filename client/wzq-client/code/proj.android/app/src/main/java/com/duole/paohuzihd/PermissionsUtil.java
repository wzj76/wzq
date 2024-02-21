package com.duole.paohuzihd;

import java.util.ArrayList;
import java.util.HashMap;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;

import android.net.Uri;
import android.provider.Settings;
import android.support.v4.app.ActivityCompat;


public class PermissionsUtil {

	private static Cocos2dxActivity thisActivity = null;
	public static HashMap<String, String> requiredPermissionCfg = new HashMap<String, String>();
	public static ArrayList<String> requestPermissionList = new ArrayList<String>();
	public static ArrayList<String> allStoragePermissionList = new ArrayList<String>();
	private static Activity _activity = null;
	private static boolean _gotoSettings = false;
	private static int _requestPermissionType = -1; // 0 ：电话权限完成后走登录 1：电话权限 2：存储权限
	private static boolean showPermissTip = true;
	
	public static void permissionsUtilInit(final Activity activity){
		_activity = activity;
		thisActivity = (Cocos2dxActivity)activity;
		// 运行时权限配置
		requiredPermissionCfg.clear();
		requestPermissionList.clear();

		allStoragePermissionList.clear();
		allStoragePermissionList.add(Manifest.permission.WRITE_EXTERNAL_STORAGE);
	}

	// 判断存储是否未获取
	public static boolean checkStoragePermissions(){
		ArrayList<String> filterdRequestPermissionList = new ArrayList<String>();
		for (String permission : allStoragePermissionList) {
			int result = ActivityCompat.checkSelfPermission(_activity, permission);
			if (result != PackageManager.PERMISSION_GRANTED) {
				filterdRequestPermissionList.add(permission);
			}
		}
		if (filterdRequestPermissionList.size() > 0){
			return true;
		}else{
			return false;
		}
	}
	// 请求存储权限 checkStoragePermissions 之后调用
	public static void requestStoragePermissions() {
		_requestPermissionType = 2;
		ArrayList<String> filterdRequestPermissionList = new ArrayList<String>();
		for (String permission : allStoragePermissionList) {
			int result = ActivityCompat.checkSelfPermission(_activity, permission);
			if (result != PackageManager.PERMISSION_GRANTED) {
				filterdRequestPermissionList.add(permission);
			}
		}
		if (filterdRequestPermissionList.size() > 0) {
			if (showPermissTip) {
				thisActivity.runOnGLThread(new Runnable() {
					@Override
					public void run() {
						onShowPermissTip("需要【存储】权限用于记录游戏数据。请开启【存储】权限，以正常使用游戏功能。", 1);
					}
				});
				return;
			}
			String[] permissions = filterdRequestPermissionList.toArray(new String[filterdRequestPermissionList.size()]);
			ActivityCompat.requestPermissions(_activity, permissions, 0);
		}
	}

	public static void setPermissionsCanShowTip(final boolean show) {
		showPermissTip = show;
	}

	public static int getRequestReadType(){
		return _requestPermissionType;
	}

	public static void resetRequestReadType(){
		_requestPermissionType = -1;
	}

	//	从系统的授权设置界面返回；
	public static boolean isGotoSettings(){
		return _gotoSettings;
	}
	public static void resetGotoSettings(){
		_gotoSettings = false;
	}
	private static void configureRequiredPermission(final String permission, final String description) {
		boolean canAdd = true;
		for (int i = 0; i < requestPermissionList.size(); i++) {
            if (requestPermissionList.get(i).equals(permission)) {
            	canAdd = false;
 				break;
            }
        }
        if (!canAdd) {
        	return;
        }

		int result = ActivityCompat.checkSelfPermission(_activity, permission);
		if (result != PackageManager.PERMISSION_GRANTED) {
			requiredPermissionCfg.put(permission, description);
			requestPermissionList.add(permission);
		}
	}

	public static void showRequestPermissionRationale(final String permission,
													  final String message,
												final boolean gotoSettings) {
		AlertDialog.Builder builder = new AlertDialog.Builder(_activity, AlertDialog.THEME_DEVICE_DEFAULT_LIGHT);
		builder.setCancelable(false);
		builder.setTitle("权限申请");
		builder.setMessage(message);
		builder.setNegativeButton("取消", new DialogInterface.OnClickListener() {

			@Override
			public void onClick(DialogInterface dialog, int which) {
				// _activity.finish();
				// System.exit(0);
			}
		});
		builder.setPositiveButton("去设置", new DialogInterface.OnClickListener() {

			@Override
			public void onClick(DialogInterface dialog, int which) {
				if (gotoSettings) {
					launchSettings();
				} else {
					ActivityCompat.requestPermissions(_activity, new String[] { permission }, 0);
				}
			}
		});
		builder.show();
	}

	private static void launchSettings() {
		Intent intent = new Intent();
		intent.setAction(Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
		intent.setData(Uri.fromParts("package", _activity.getPackageName(), null));
		_activity.startActivity(intent);
		_gotoSettings = true;
	}

	// 获取保存的横竖屏
    public static native void onShowPermissTip(String des, int Type);
}