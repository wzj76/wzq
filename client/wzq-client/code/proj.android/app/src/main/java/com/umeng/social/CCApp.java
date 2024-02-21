package com.umeng.social;

import android.app.Application;

import com.duole.sdk.umeng.UMengUtil;


public class CCApp extends Application {

    @Override
    public void onCreate() {
        super.onCreate();
        UMengUtil.preInit(this);
    }
}
