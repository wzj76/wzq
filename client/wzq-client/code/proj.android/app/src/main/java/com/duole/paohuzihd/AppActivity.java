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
package com.duole.paohuzihd;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.annotation.SuppressLint;
import android.content.SharedPreferences;
import android.widget.ImageView;
import android.net.Uri;
import android.os.Handler;
import android.content.pm.PackageManager;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.telephony.PhoneStateListener;
import android.telephony.SignalStrength;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.view.View;
import android.view.KeyEvent;
import android.view.WindowManager;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import java.util.ArrayList;
import java.util.HashMap;
import com.duole.jniutil.CheckUpdate;
import com.duole.jniutil.PlatformFunction;
import com.duole.jniutil.SysUtil;
import com.duole.sdk.shareSdk.ShareSdkUtil;
import com.duole.sdk.umeng.UMengUtil;
import com.duole.update.AppDownloadUtil;
import com.duole.update.ProgressHandler;
import com.duole.sdk.wechat.WeChatSdkUtil;
import com.duole.sdk.x5.X5WebViewUtil;
import com.duole.sdk.launcher.DLLauncherUtil;
import android.support.v4.app.ActivityCompat;
import android.media.AudioManager;

public class AppActivity extends Cocos2dxActivity implements ActivityCompat.OnRequestPermissionsResultCallback, ProgressHandler {
	// 信号强度
	public static int gsm_signalStrength = 0;
	private TelephonyManager telManager;
	private SignalStateListener signalListener;	
	private static Cocos2dxActivity mThis = null;
	// 授权列表
	private HashMap<String, String> requiredPermissionCfg = new HashMap<String, String>();

    private static long appStartTime = 0;
    private static boolean firstInstall = false;

    private static long totalPauseTime = 0; //暂停总时间
    private static long pauseTimestamp = 0; // 切到后台的时间点
    private static boolean inPause = false;
    private static boolean isGameScreenChange = false;
    private static boolean isOpenShop = false;
    private static int nGameDir = 0; //游戏屏幕方向

    private ProgressDialog m_pProgress = null;
    private boolean m_bHideProgress = false;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
        //	记录app启动时间戳；
        appStartTime = System.currentTimeMillis();
		super.onCreate(savedInstanceState);
		// 启动异步加载
        launchApp();
        // 设置音量键控制媒体声音大小
		setVolumeControlStream(AudioManager.STREAM_MUSIC);
		// 屏幕常亮
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		// 动态权限申请
        PermissionsUtil.permissionsUtilInit(this);
		// 更新检测工具类初始化；
        CheckUpdate.init(this);
        DLLauncherUtil.init(this);
        // 信号检测
		signalListener  = new SignalStateListener();
		telManager  = (TelephonyManager)getSystemService(Context.TELEPHONY_SERVICE);
		telManager.listen(signalListener, PhoneStateListener.LISTEN_SIGNAL_STRENGTHS);

        m_pProgress = new ProgressDialog(this);
        m_pProgress.setCancelable(false);
        m_pProgress.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
        m_pProgress.setMax(100);
        m_pProgress.setButton(DialogInterface.BUTTON_POSITIVE, "后台下载", new DialogInterface.OnClickListener() {

            @Override
            public void onClick(DialogInterface dialog, int which) {
                m_bHideProgress = true;
                AppDownloadUtil.inBack();
                m_pProgress.dismiss();
            }
        });
	}

	private void launchApp() {
        mThis = this;
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
        //
		PlatformFunction.init(this);
        SysUtil.getInstance().init(this);
        AppDownloadUtil.init(this, this);

        Log.d("", "事件上报启动游戏");
        @SuppressLint("WrongConstant") SharedPreferences preferences = getContext().getSharedPreferences("androidLocalData", MODE_PRIVATE);
        firstInstall = preferences.getBoolean("firstInstall", true);
        // 存本地
        SharedPreferences.Editor editor = preferences.edit();
        editor.putBoolean("firstInstall", false);
        editor.commit();

        // 上报安卓sdk初始化开始
        String deviceId = SysUtil.getMobileID();
        SysUtil.reportPageEvent("安卓sdk初始化开始", "", "", "", deviceId, "");
        UMengUtil.init(this);

        // sharesdk 初始化
        ShareSdkUtil.init(this);
        //初始化微信支付
		WeChatSdkUtil.init(this);
		// X5初始化
        X5WebViewUtil.init(this, mFrameLayout);
        Uri data = getIntent().getData();
		if (data != null)
		{
            String sPath= data.getPath();  
    		String sData= data.getQuery();
    		PlatformFunction.onSaveSchemesResult(sPath, sData);
        }
        // 上报安卓sdk初始化结束
        SysUtil.reportPageEvent("安卓sdk初始化结束", "", "", "", deviceId, "");
    }
	/*
    * 获取app启动过程耗时：毫秒，用于控制logo页面显示时间
    * */
    public static int getTimeMillisAppStarted() {
        long delta = System.currentTimeMillis() - appStartTime - totalPauseTime;
        return (int)delta;
    }
    /*
    *  是否是首次安装
    * */
    public static String getFirstInstall() {
        return firstInstall ? "true" : "false";
    }
	@Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        final int nRequestReadType = PermissionsUtil.getRequestReadType();
        for (int i = 0; i < permissions.length; i++) {
            String permission = permissions[i];
            int result = grantResults[i];
            // 清理已允许权限
            if (result == PackageManager.PERMISSION_GRANTED) {
                PermissionsUtil.requestPermissionList.remove(permission);
            }
        }

        final ArrayList<String> showRationalePermissionList = new ArrayList<String>();
        for (String permission : PermissionsUtil.requestPermissionList) {
            // 记录没有彻底禁止的权限
            if (ActivityCompat.shouldShowRequestPermissionRationale(AppActivity.this, permission)) {
                showRationalePermissionList.add(permission);
            }
        }

        final ArrayList<String> showRationaleStoragePermissionList = new ArrayList<String>();
        for (String permission : PermissionsUtil.allStoragePermissionList) {
            // 记录没有彻底禁止的权限
            if (ActivityCompat.shouldShowRequestPermissionRationale(this, permission)) {
                showRationaleStoragePermissionList.add(permission);
            }
        }

        if (showRationalePermissionList.size() > 0  && (nRequestReadType == 0 || nRequestReadType == 1)) {
            // 优先提示没有彻底禁止的权限，不需要前往设置
            mThis.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    String permission = showRationalePermissionList.get(0);
                    PermissionsUtil.showRequestPermissionRationale(permission, PermissionsUtil.requiredPermissionCfg.get(permission), true);
                }
            });
        } else if (showRationaleStoragePermissionList.size() > 0 && nRequestReadType == 2) {
            // 优先提示没有彻底禁止的权限，不需要前往设置
            mThis.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    String permission = showRationaleStoragePermissionList.get(0);
                    PermissionsUtil.showRequestPermissionRationale(permission, "需要【存储】权限用于记录游戏数据。请开启【存储】权限，以正常使用游戏功能。", true);
                }
            });
        } else if (PermissionsUtil.requestPermissionList.size() > 0 && (nRequestReadType == 0 || nRequestReadType == 1))  {
            // 次要提示彻底禁止的权限，需要前往设置
            mThis.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    String permission = PermissionsUtil.requestPermissionList.get(0);
                    PermissionsUtil.showRequestPermissionRationale(permission, PermissionsUtil.requiredPermissionCfg.get(permission), true);
                }
            });
        } else if (PermissionsUtil.allStoragePermissionList.size() > 0 && PermissionsUtil.checkStoragePermissions() && nRequestReadType == 2)  {
            // 次要提示彻底禁止的权限，需要前往设置
            mThis.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    String permission = PermissionsUtil.allStoragePermissionList.get(0);
                    PermissionsUtil.showRequestPermissionRationale(permission, "需要【存储】权限用于记录游戏数据。请开启【存储】权限，以正常使用游戏功能。", true);
                }
            });
        }
        PermissionsUtil.resetRequestReadType();
    }
	@Override
	protected void onDestroy() {
		super.onDestroy();
		
		SysUtil.getInstance().destroy();
		PlatformFunction.destroy();
        AppDownloadUtil.destroy();
        UMengUtil.destroy();
        ShareSdkUtil.destroy();
	}
	
	@Override
	protected void onPause() {
		super.onPause();
        ShareSdkUtil.onPause();
        inPause = true;
        pauseTimestamp = System.currentTimeMillis();
	}
	
	@Override
	protected void onResume() {
		super.onResume();
        ShareSdkUtil.onResume();
        if(PermissionsUtil.isGotoSettings()){
            PermissionsUtil.resetGotoSettings();
        }
        if (inPause){
            long resumeTime = System.currentTimeMillis() - pauseTimestamp;
			if (resumeTime > 0) {
				totalPauseTime += resumeTime;
			}
        }
        inPause = false;
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		PlatformFunction.onActivityResult(requestCode, resultCode, data);
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		Log.d("", "onKeyDown");
		return false;
	}
	
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		Log.d("", "onKeyUp");
		return false;
	}

    @Override
    protected void onNewIntent(Intent intent) {
		Uri data = intent.getData();
		if (data != null)
		{
            String sPath= data.getPath();  
    		String sData= data.getQuery();
    		PlatformFunction.onSaveSchemesResult(sPath, sData);
        }     
    }
    	
	private class SignalStateListener extends PhoneStateListener{
		public void onSignalStrengthsChanged(SignalStrength signalStrength) {
			super.onSignalStrengthsChanged(signalStrength);
			final int strength = signalStrength.getGsmSignalStrength();
			int level = (int)Math.pow(strength, 1.0 / 3) * 30;
			SysUtil.getInstance().setSignalStrengthGsm(level);
		}
	}

	// 设置设备横、竖屏
	public static void changeOrientation(int type) {
        isGameScreenChange = false;
        if (nGameDir == 1) {
        	doScreenChange(nGameDir);
        }
        else {
        	if (mThis.getRequestedOrientation() != type) {
    			mThis.setRequestedOrientation(type);	
    		}
        }
	}
	
	// 商城是横屏
	public static void changeOrientationToShop(int type) {
        isGameScreenChange = false;
    	if (mThis.getRequestedOrientation() != type) {
			mThis.setRequestedOrientation(type);
            isOpenShop = true;  	
		}
        else {
            mThis.runOnGLThread(new Runnable() {
                @Override
                public void run() {                     
                    onX5OpenShop();
                }
            });
        }
	}

    public static void doScreenChange(int type) {
        isGameScreenChange = true;
        if (mThis.getRequestedOrientation() != type) {
            mThis.setRequestedOrientation(type);      
        }
    }

    public static int getScreenDir() {
        return nGameDir;
    }

    @Override  
    public void onConfigurationChanged(Configuration newConfig) {  
        // 重写onConfigurationChanged事件，捕捉当设置之后的值  
        // 须设置configChanges属性才能捕捉onConfigurationChanged 
        if (isGameScreenChange == true) {
            if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE || newConfig.orientation == Configuration.ORIENTATION_PORTRAIT) {
                mThis.runOnGLThread(new Runnable() {
                    @Override
                    public void run() {                     
                        boolean bIsPortrait = true;
                        nGameDir = 1;
                        if(mThis.getRequestedOrientation() == 0)
                        {
                            //横屏    
                            bIsPortrait = false;
                            nGameDir = 0;
                        }
                        Log.d("", "onConfigurationChanged");
                        onScreenChangeEnd(bIsPortrait);
                    }
                });
            }
            if (newConfig.keyboardHidden == Configuration.KEYBOARDHIDDEN_NO) {  }  
        }
        else {
            mThis.runOnGLThread(new Runnable() {
                @Override
                public void run() {                     
                    onPromptChange();
                    if (isOpenShop) {
                        isOpenShop = false;
                        onX5OpenShop();
                    }
                }
            });
        }
        isGameScreenChange = false;
        super.onConfigurationChanged(newConfig);  
    }
    // 横竖屏切换
    public static native void onScreenChangeEnd(boolean bIsPortrait);
    // 通知提示变化
    public static native void onPromptChange();
    public static native void onX5OpenShop();
    // 获取保存的横竖屏
    public static native int getSceneDirectionKey();

    @Override
    public void updateProgress(final String sTitle, final int nProgress) {
        if (m_bHideProgress) {
            return;
        }
        mThis.runOnUiThread(new Runnable() {

            @Override
            public void run() {
                if (m_bHideProgress) {
                    return;
                }
                m_pProgress.setMessage(sTitle);
                m_pProgress.show();
                m_pProgress.setProgress(nProgress);
                if (nProgress == 100) {
                    m_pProgress.dismiss();
                }
            }
        });
    }

    @Override
    public void progressBegin(final String sTitle, boolean bSilent) {
        m_bHideProgress = bSilent;
        mThis.runOnUiThread(new Runnable() {

            @Override
            public void run() {
                m_pProgress.setMessage(sTitle);
                if (m_bHideProgress) {
                    m_pProgress.hide();
                    AppDownloadUtil.hideProgress();
                } else {
                    m_pProgress.setProgress(0);
                    m_pProgress.show();
                    AppDownloadUtil.showProgress();
                }
            }
        });
    }

    @Override
    public void progressFinish() {
        m_bHideProgress = true;
        m_pProgress.dismiss();
    }
}
