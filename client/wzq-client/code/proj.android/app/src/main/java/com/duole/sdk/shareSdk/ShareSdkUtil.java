/****************************************************************************
sharesdk SDK相关
author：daxiang
time：2022.8.29
****************************************************************************/
package com.duole.sdk.shareSdk;

import org.cocos2dx.lib.Cocos2dxHelper.Cocos2dxHelperListener;
import org.cocos2dx.lib.Cocos2dxJavascriptJavaBridge;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.util.Log;
import android.text.TextUtils;

import com.duole.jniutil.PlatformFunction;

import java.util.HashMap;

import cn.sharesdk.framework.Platform;
import cn.sharesdk.framework.PlatformDb;
import cn.sharesdk.framework.Platform.ShareParams;
import cn.sharesdk.framework.PlatformActionListener;
import cn.sharesdk.framework.ShareSDK;
import cn.sharesdk.tencent.qq.QQ;
import cn.sharesdk.tencent.qzone.QZone;
import cn.sharesdk.wechat.friends.Wechat;
import cn.sharesdk.wechat.moments.WechatMoments;

import com.duole.paohuzihd.PermissionsUtil;
import com.duole.paohuzihd.R;
import com.mob.MobSDK;

public class ShareSdkUtil {
	private static Cocos2dxActivity thisActivity = null;
	public static Cocos2dxHelperListener sCocos2dxHelperListener;

	// ShareSDK分享
	// 平台类型
	private static final int SOCIAL_PLATFORM_WECHATSESSION = 1;
	private static final int SOCIAL_PLATFORM_WECHATTIMELINE = 2;
	private static final int SOCIAL_PLATFORM_QQ = 3;
	private static final int SOCIAL_PLATFORM_QZONE = 4;
	private static final int SOCIAL_PLATFORM_WEACHATGAME = 5;

	private static String _shareType = "-1";	// 分享点，如 大厅、结算等
	private static PlatformActionListener ssdkShareListener = new PlatformActionListener() {
		
		@Override
		public void onError(Platform platform, int action, Throwable t) {
			t.printStackTrace();
			if(t!=null){
				Log.i("ghome","throw:"+t.getMessage());
			}
			onShareResult(false, _shareType);
			Log.i("ghome","throw========:");
		}
		
		@Override
		public void onComplete(final Platform platform, int action, HashMap<String, Object> res) {
			Log.i("ghome","platform"+platform);
			onShareResult(true, _shareType);
		}
		
		@Override
		public void onCancel(Platform platform, int action) {
			onShareResult(false, _shareType);
			Log.i("ghome","throw======rrrrr==:");
		}
	};

	public static void init(Cocos2dxActivity activity) {
		thisActivity = activity;

		onResume();

		sCocos2dxHelperListener = activity;

		// ShareSDK分享
		MobSDK.init(thisActivity);
		MobSDK.submitPolicyGrantResult(true, null);
		// 微信好友
		HashMap<String, Object> info0 = new HashMap<String, Object>();
		info0.put("Id", String.valueOf(SOCIAL_PLATFORM_WECHATSESSION));
		info0.put("SortId", String.valueOf(SOCIAL_PLATFORM_WECHATSESSION));
		info0.put("AppId", PlatformFunction.getWechatAppId());
		info0.put("AppSecret", PlatformFunction.getWechatAppSecret());
		ShareSDK.setPlatformDevInfo(Wechat.NAME, info0);
		// 微信朋友圈
		HashMap<String, Object> info1 = new HashMap<String, Object>();
		info1.put("Id", String.valueOf(SOCIAL_PLATFORM_WECHATTIMELINE));
		info1.put("SortId", String.valueOf(SOCIAL_PLATFORM_WECHATTIMELINE));
		info1.put("AppId", PlatformFunction.getWechatAppId());
		info1.put("AppSecret", PlatformFunction.getWechatAppSecret());
		ShareSDK.setPlatformDevInfo(WechatMoments.NAME, info1);
		// QQ好友
		HashMap<String, Object> info2 = new HashMap<String, Object>();
		info2.put("Id", String.valueOf(SOCIAL_PLATFORM_QQ));
		info2.put("SortId", String.valueOf(SOCIAL_PLATFORM_QQ));
		info2.put("AppId", PlatformFunction.getQQAppId());
		info2.put("AppKey", PlatformFunction.getQQAppSecret());
		ShareSDK.setPlatformDevInfo(QQ.NAME, info2);
		// QQ空间
		HashMap<String, Object> info3 = new HashMap<String, Object>();
		info3.put("Id", String.valueOf(SOCIAL_PLATFORM_QZONE));
		info3.put("SortId", String.valueOf(SOCIAL_PLATFORM_QZONE));
		info3.put("AppId", PlatformFunction.getQQAppId());
		info3.put("AppKey", PlatformFunction.getQQAppSecret());
		ShareSDK.setPlatformDevInfo(QZone.NAME, info3);

	}

	public static void onPause() {

	}

	public static void onResume() {

	}

	public static void onFinish() {
	}

	public static void destroy() {
		thisActivity = null;
	}

	public static void share(final String redirectUrl, final int platform, final String text, final String title, final String thumbUrl, final String shareType) {
		if (TextUtils.isEmpty(text)) {
			return;
		}
		_shareType = shareType;
		Log.i("ghome", "ShareSdkUtil share redirectUrl = " + redirectUrl + " platform = " + platform + " title = " + title);
		// 标题
		String formattedTitle = TextUtils.isEmpty(title) ? thisActivity.getString(R.string.app_name) : title;
		// 图片url
		String formattedThumbUrlUrl = TextUtils.isEmpty(thumbUrl) ? "https://assets.duole.com/images/uploads/202209/883f192418b9f4057201732db7e0f49b.jpg" : thumbUrl ;
		// 跳转链接
		String formattedRedirectUrl = redirectUrl;
		// 设置分享参数
		ShareParams params = new ShareParams();
		params.setText(text);
		params.setTitle(formattedTitle);
		params.setUrl(formattedRedirectUrl);
		params.setImageUrl(formattedThumbUrlUrl);
		params.setShareType(Platform.SHARE_WEBPAGE);
		// 分享平台
		String media = "";
		switch (platform) {
			case SOCIAL_PLATFORM_WECHATTIMELINE:
				// 微信朋友圈分享消息只显示标题，以内容替换标题的方式特殊处理
				params.setTitle(text);
				media = WechatMoments.NAME;
				break;
			case SOCIAL_PLATFORM_QQ:
				params.setTitleUrl(formattedRedirectUrl);
				media = QQ.NAME;
				break;
			case SOCIAL_PLATFORM_QZONE:
				params.setTitleUrl(formattedRedirectUrl);
				media = QZone.NAME;
				break;
			case SOCIAL_PLATFORM_WECHATSESSION:
			default:
				media = Wechat.NAME;
				break;
		}
		// 分享
		Platform ssdkPlatform = ShareSDK.getPlatform(media);
		ssdkPlatform.setPlatformActionListener(ssdkShareListener);
		ssdkPlatform.share(params);
	}

	public static void shareImage(final String absoluteImagePath, final int platform, final String shareType) {
		if (TextUtils.isEmpty(absoluteImagePath)) {
			return;
		}
		_shareType = shareType;
		// 设置分享参数
		ShareParams params = new ShareParams();
		params.setImagePath(absoluteImagePath);
		params.setShareType(Platform.SHARE_IMAGE);
		// 平台
		String media = "";
		switch (platform) {
			case SOCIAL_PLATFORM_WECHATTIMELINE:
				media = WechatMoments.NAME;
				break;
			case SOCIAL_PLATFORM_QQ:
				if (PermissionsUtil.checkStoragePermissions()) {
					PermissionsUtil.requestStoragePermissions();
					return;
				}
				media = QQ.NAME;
				break;
			case SOCIAL_PLATFORM_QZONE:
				if (PermissionsUtil.checkStoragePermissions()) {
					PermissionsUtil.requestStoragePermissions();
					return;
				}
				media = QZone.NAME;
				break;
			case SOCIAL_PLATFORM_WECHATSESSION:
			default:
				media = Wechat.NAME;
				break;
		}
		Log.i("ghome", "分享 media=" + media);
		Platform ssdkPlatform = ShareSDK.getPlatform(media);
		ssdkPlatform.setPlatformActionListener(ssdkShareListener);
		ssdkPlatform.share(params);
	}

	public static void shareMini(final String path, final String text, final String thumbUrl, final String title, String shareType) {
		_shareType = shareType;
		Log.i("ghome", "ShareSdkUtil shareMini text = " + text + " title = " + title + " thumbUrl = " + thumbUrl);
		// 设置分享参数
		ShareParams params = new ShareParams();
		params.setText(text);
		params.setTitle(title);
		params.setImageUrl(thumbUrl);
		params.setUrl("http://open.qq.com");
		params.setShareType(Platform.SHARE_WXMINIPROGRAM);
		params.setWxUserName(PlatformFunction.getQQAppId());
		params.setWxPath(path);
		params.setWxMiniProgramType(0);  //正式版
		params.setWxWithShareTicket(true);

		// 平台
		String media = Wechat.NAME;

		Platform ssdkPlatform = ShareSDK.getPlatform(media);
		ssdkPlatform.setPlatformActionListener(ssdkShareListener);
		ssdkPlatform.share(params);
	}

	public static void doWeChatAuth() {
		Log.i("ghome","微信登录");
		ShareSDK.setActivity(thisActivity);
		Platform ssdkPlatform = ShareSDK.getPlatform(Wechat.NAME);
		ssdkPlatform.setPlatformActionListener(new PlatformActionListener() {
			@Override
			public void onComplete(Platform platform, int i, HashMap<String, Object> hashMap) {
				PlatformDb db = platform.getDb();
				final String openid = db.get("openid");
				final String accessToken = db.get("token");
				final String refreshToken = db.get("refresh_token");
				Log.i("ghome","doWeChatAuth onComplete openid = " + openid + " accessToken = " + accessToken + " refreshToken = " + refreshToken);
				thisActivity.runOnGLThread(new Runnable() {
					@Override
					public void run() {
						try {
							if (!TextUtils.isEmpty(openid) && !TextUtils.isEmpty(accessToken) && !TextUtils.isEmpty(refreshToken)) {
								onAuthFinish(accessToken, refreshToken, openid, false);
							} else {
								onAuthFinish("", "", "", true);
							}
						}catch (Exception e){

						}
					}
				});
			}
			
			@Override
			public void onError(Platform platform, int i, Throwable throwable) {
				Log.i("ghome","doWeChatAuth onError platform = " + platform + " i = " + i);
				throwable.printStackTrace();
			}
			
			@Override
			public void onCancel(Platform platform, int i) {
				Log.i("ghome","doWeChatAuth onCancel platform ="+ platform + " i = " + i);
				
			}
		});
		ssdkPlatform.authorize();
	}

		//分享结果
	private static void onShareResult(final boolean success, final String shareType)
	{
		Log.i("ghome", "ShareSdkUtil onShareResult success = " + success + " shareType = " + shareType);
		thisActivity.runOnGLThread(new Runnable() {
			@Override
			public void run() {
				PlatformFunction.onShareResult(success, Integer.parseInt(shareType));
			}
		});
	}

	//授权结果
	private static void onAuthFinish(final String accessToken, final String refreshToken, final String openId, final boolean error)
	{
		Log.i("ghome", "ShareSdkUtil onAuthFinish accessToken = " + accessToken + " refreshToken = " + refreshToken + " openId = " + openId + " error = " + error);
		PlatformFunction.onAuthFinish(accessToken, refreshToken, openId, error);
	}
}
