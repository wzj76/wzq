package com.duole.sdk.channel;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHelper;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.Intent;
import android.net.Uri;
import android.util.Log;

import com.duole.paohuzihd.ParameterConfig;
import com.duole.sdk.umeng.UMengUtil;

public class ChannelSdkUtil {
	private static Cocos2dxActivity thisActivity = null;
	public static final int VERIFIED_RESULT_DO_ANTI_ADDICTED = 0; 	// 防沉迷
	public static final int VERIFIED_RESULT_DO_LOGIN = 1; 			// 进游戏
	public static final int VERIFIED_RESULT_DO_EXIT = 2; 			// 切账号
	public static final int VERIFIED_RESULT_DO_FAIL_LOGIN = 3; 		// 未实名进游戏
	
	public static void init(Cocos2dxActivity activity) {
		thisActivity = activity;
	}
	
	public static void destroy() {
		thisActivity = null;
	}
	
	public static void onResume() {
		
	}
	
	public static void onPause() {
		
	}
	
	public static void doSdkLogin() {

	}

	public static void doGetVerifiedInfo() {

	}
	
	private static void doSdkGetTokenAndSsoid() {
		
	}
	
	public static void doSdkPay(
			final String order,
			final String attach,
			final int amount,
			final String productName,
			final String productDesc,
			final String callbackUrl) {
		
	}
	
	public static void doSdkQuit() {
		if (thisActivity == null) {
			return;
		}
		UMengUtil.onFinish();
	}
	
	public static native void onLoginComplete(String token, String ssoid);
	public static native void onPayComplete();
	public static native void onLoginError(String loginError);
	public static native void onVerifiedResult(int verifiedResult);
}
