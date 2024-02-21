package com.duole.paohuzihd.wxapi;

import android.app.Activity;
import android.os.Bundle;
import android.content.Intent;

import com.duole.jniutil.PlatformFunction;
import com.duole.sdk.wechat.WeChatSdkUtil;
import com.tencent.mm.opensdk.modelbase.BaseReq;
import com.tencent.mm.opensdk.modelbase.BaseResp;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;

public class WXPayEntryActivity extends Activity implements IWXAPIEventHandler {
    private IWXAPI api;
    private static String APP_ID = PlatformFunction.getWechatAppId();

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        api = WXAPIFactory.createWXAPI(this, APP_ID);
        api.handleIntent(getIntent(), this);
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        setIntent(intent);
        api.handleIntent(intent, this);
    }

    @Override
    public void onReq(BaseReq req) {
        WeChatSdkUtil.onReq(req);
        finish();
    }

    @Override
    public void onResp(BaseResp resp) {
        WeChatSdkUtil.onResp(resp);
        finish();
    }
}
