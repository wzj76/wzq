package com.duole.paohuzihd.wxapi;

import android.os.Bundle;
import android.util.Log;

import com.duole.jniutil.PlatformFunction;
import cn.sharesdk.wechat.utils.WechatHandlerActivity;

public class WXEntryActivity extends WechatHandlerActivity {

	@Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);    
        Log.i("ghome", "获取微信appid=" + PlatformFunction.getWechatAppId());
    }
}
