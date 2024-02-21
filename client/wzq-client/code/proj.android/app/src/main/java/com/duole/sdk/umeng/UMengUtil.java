/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.duole.sdk.umeng;


import android.app.Application;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import com.duole.common.PlatformUtil;
import com.duole.jniutil.PlatformFunction;
import com.duole.jniutil.SysUtil;
import com.duole.paohuzihd.ParameterConfig;
import com.duole.paohuzihd.PermissionsUtil;
import com.duole.paohuzihd.R;
import com.umeng.analytics.MobclickAgent;
import com.umeng.commonsdk.UMConfigure;

import org.cocos2dx.lib.Cocos2dxActivity;


public class UMengUtil {
	private static Cocos2dxActivity thisActivity = null;
	private static int _taskId = 0;

	public static void preInit(Application context){
		System.out.println("UMengUtil.getChannel() = " + UMengUtil.getChannel(context));
		UMConfigure.preInit(context, ParameterConfig.UMENG_APP_KEY, UMengUtil.getChannel(context));
	}

	public static String getChannel(Application context) {
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

	public static void init(Cocos2dxActivity activity) {
		thisActivity = activity;

		initUMengInfo();
	}

	public static void initUMengInfo()
	{
		UMConfigure.submitPolicyGrantResult(thisActivity.getApplicationContext(), true);
		String channel = PlatformUtil.getChannelId();
		UMConfigure.init(thisActivity, PlatformFunction.getUmengAppKey(), channel, UMConfigure.DEVICE_TYPE_PHONE,null);
		MobclickAgent.setPageCollectionMode(MobclickAgent.PageMode.AUTO);
	}

	public static void onPause() {

	}

	public static void onResume() {

	}

	public static void onFinish() {
		MobclickAgent.onKillProcess(thisActivity);
	}

	public static void destroy() {

	}

}
