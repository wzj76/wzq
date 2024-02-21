package cn.udesk.activity;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;

import udesk.core.LocalManageUtil;

public class UdeskBaseActivity extends AppCompatActivity {


    @Override
    protected void attachBaseContext(Context newBase) {
        super.attachBaseContext(LocalManageUtil.setLocal(newBase));
    }
}
