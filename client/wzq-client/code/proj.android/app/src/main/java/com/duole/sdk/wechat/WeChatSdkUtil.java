package com.duole.sdk.wechat;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Intent;
import android.net.Uri;
import android.util.Log;
import android.widget.Toast;

import com.duole.jniutil.PlatformFunction;
import com.duole.paohuzihd.ParameterConfig;
import com.tencent.mm.opensdk.constants.ConstantsAPI;
import com.tencent.mm.opensdk.modelbase.BaseReq;
import com.tencent.mm.opensdk.modelbase.BaseResp;
import com.tencent.mm.opensdk.modelpay.PayReq;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;

public class WeChatSdkUtil {
	private static Cocos2dxActivity thisActivity = null;
	private static IWXAPI api;
	
	public static void init(Cocos2dxActivity activity) {
		thisActivity = activity;
		
		api = WXAPIFactory.createWXAPI(thisActivity, null);
		api.registerApp(PlatformFunction.getWechatAppId());
	}
	
	public static void destroy() {
        thisActivity = null;
	}
	
	public static void doSdkPay(
			final String partnerId,
			final String prepayId,
			final String nonceStr,
			final String timestamp,
			final String pack,						
			final String sign) {
		Log.i("ghome", "do sdk pay wechat");
		Log.i("ghome", "partnerId=" + partnerId + " prepayId" + prepayId + " nonceStr=" + nonceStr + " timestamp=" + timestamp + " pack=" + pack + " sign=" + sign);
		Runnable payRunnable = new Runnable() {
			@Override
			public void run() {
				PayReq req = new PayReq();
				req.appId = PlatformFunction.getWechatAppId();
				req.partnerId = partnerId;
				req.prepayId = prepayId;
				req.nonceStr = nonceStr;
				req.timeStamp = timestamp;
				req.packageValue = pack;
				req.sign = sign;				
				api.sendReq(req);
			}
		};
		// 必须异步调用
		Thread payThread = new Thread(payRunnable);
		payThread.start();
	}
	
	public static void onReq(BaseReq req) {
		
	}
	
	public static void onResp(BaseResp resp) {
		if (resp.getType() == ConstantsAPI.COMMAND_PAY_BY_WX) {
			if (resp.errCode == 0)
			{
				Toast.makeText(thisActivity, "微信支付成功", Toast.LENGTH_LONG).show();
				Log.i("ghome", "wechat pay success");
				thisActivity.runOnGLThread(new Runnable() {
					
					@Override
					public void run() {
						onPayComplete();
					}
				});
			}
		}
	}
	
	public static void doAliPay(String url)
	{
		Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
		thisActivity.startActivity(intent);
	}

	public static native void onPayComplete();
}
