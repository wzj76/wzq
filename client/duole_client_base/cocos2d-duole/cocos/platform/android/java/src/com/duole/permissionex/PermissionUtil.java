package com.duole.permissionex;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.provider.Settings;
import android.support.v4.app.ActivityCompat;
// import org.cocos2dx.lib.Cocos2dxActivity;
import java.util.ArrayList;

public class PermissionUtil {

    private static Activity thisActivity = null;
    // // 必要权限列表
    // private static ArrayList<AppPermissionObject> requiredPermissionObjects = null;

    // 自定义权限列表
    private static ArrayList<PermissionObject> customPermissionObjects = null;
    // 自定义回调
    private static PermissionCallback customPermissionCallback = null;

    public static void init(Activity activity) {
        thisActivity = activity;
        // // 初始化权限列表
        // requiredPermissionObjects = new ArrayList<AppPermissionObject>();
    }

    public static void requestCustomPermission(PermissionObject object, PermissionCallback callback) {
        if (object == null || callback == null) {
            return;
        }
        ArrayList<PermissionObject> objects = new ArrayList<PermissionObject>();
        objects.add(object);
        requestCustomPermissions(objects, callback);
    }

    public static void requestCustomPermissions(ArrayList<PermissionObject> objects, PermissionCallback callback) {
        if (objects == null || objects.isEmpty() || callback == null) {
            return;
        }
        customPermissionObjects = objects;
        customPermissionCallback = callback;
        thisActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (!requestPermissions(customPermissionObjects, PermissionConsts.REQUEST_CODE_CUSTOM)) {
                    customPermissionCallback.onPermissionGranted(true);
                }
            }
        });
    }

    public static void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        if (requestCode == PermissionConsts.REQUEST_CODE_CUSTOM) {
            if (processRequestPermissionsResult(customPermissionObjects, requestCode, permissions, grantResults)) {
                customPermissionCallback.onPermissionGranted(true);
            }
            return;
        }
    }

    public static void showPermissionTip(final PermissionObject object, final PermissionCallback callback) {
        if (object == null || callback == null) {
            return;
        }
        if (ActivityCompat.checkSelfPermission(thisActivity, object.name) == PackageManager.PERMISSION_GRANTED) {
            callback.onPermissionGranted(true);
            return;
        }

        AlertDialog.Builder builder = new AlertDialog.Builder(thisActivity, AlertDialog.THEME_DEVICE_DEFAULT_LIGHT);
        builder.setCancelable(false);
        builder.setTitle("权限申请");
        builder.setMessage(object.description);
        builder.setNegativeButton("以后再说", new DialogInterface.OnClickListener() {

            @Override
            public void onClick(DialogInterface dialog, int which) {
                callback.onPermissionGranted(false);
            }
        });
        builder.setPositiveButton("确认", new DialogInterface.OnClickListener() {

            @Override
            public void onClick(DialogInterface dialog, int which) {
                requestCustomPermission(object, callback);
            }
        });
        builder.show();
    }

    private static boolean processRequestPermissionsResult(ArrayList<PermissionObject> objects, int requestCode, String[] permissions, int[] grantResults) {
        for (int i = 0; i < permissions.length; i++) {
            String permission = permissions[i];
            int result = grantResults[i];
            // 更新已允许权限
            for (PermissionObject object : objects) {
                if (object.name.equals(permission)) {
                    object.granted = result == PackageManager.PERMISSION_GRANTED;
                }
            }
        }

        ArrayList<PermissionObject> showRationalePermissions = new ArrayList<PermissionObject>();
        for (PermissionObject object : objects) {
            // 记录没有彻底禁止的权限
            if (object.granted || !object.required) {
                continue;
            }
            if (ActivityCompat.shouldShowRequestPermissionRationale(thisActivity, object.name)) {
                showRationalePermissions.add(object);
            }
        }
        if (!showRationalePermissions.isEmpty()) {
            // 优先提示没有彻底禁止的权限，不需要前往设置
            showPermissionAlert(showRationalePermissions.get(0), requestCode, false);
            return false;
        }

        ArrayList<PermissionObject> deniedPermissions = new ArrayList<PermissionObject>();
        for (PermissionObject object : objects) {
            // 记录彻底禁止的权限
            if (object.granted || !object.required) {
                continue;
            }
            deniedPermissions.add(object);
        }
        if (!deniedPermissions.isEmpty()) {
            // 次要提示彻底禁止的权限，需要前往设置
            showPermissionAlert(deniedPermissions.get(0), requestCode, true);
            return false;
        }

        return true;
    }

    private static void configurePermission(ArrayList<PermissionObject> objects, String name, String description, boolean required) {
        PermissionObject object = PermissionObject.build(name, description, required);
        if (ActivityCompat.checkSelfPermission(thisActivity, name) == PackageManager.PERMISSION_GRANTED) {
            object.granted = true;
        }
        objects.add(object);
    }

    private static boolean requestPermissions(ArrayList<PermissionObject> objects, int code) {
        ArrayList<String> permissions = new ArrayList<String>();
        for (PermissionObject object : objects) {
            if (ActivityCompat.checkSelfPermission(thisActivity, object.name) == PackageManager.PERMISSION_GRANTED) {
                object.granted = true;
            } else {
                object.granted = false;
                permissions.add(object.name);
            }
        }
        if (permissions.isEmpty()) {
            return false;
        }
        ActivityCompat.requestPermissions(thisActivity, permissions.toArray(new String[permissions.size()]), code);
        return true;
    }

    private static void showPermissionAlert(final PermissionObject object, final int code, final boolean toSettings) {
        AlertDialog.Builder builder = new AlertDialog.Builder(thisActivity, AlertDialog.THEME_DEVICE_DEFAULT_LIGHT);
        builder.setCancelable(false);
        builder.setTitle("权限申请");
        builder.setMessage(object.description);
        builder.setNegativeButton("取消", new DialogInterface.OnClickListener() {

            @Override
            public void onClick(DialogInterface dialog, int which) {
            }
        });
        builder.setPositiveButton("去设置", new DialogInterface.OnClickListener() {

            @Override
            public void onClick(DialogInterface dialog, int which) {
                if (toSettings) {
                    PermissionUtil.launchSettings();
                } else {
                    ActivityCompat.requestPermissions(thisActivity, new String[] { object.name }, code);
                }
            }
        });
        builder.show();
    }

    private static void launchSettings() {
        Intent intent = new Intent();
        intent.setAction(Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
        intent.setData(Uri.fromParts("package", thisActivity.getPackageName(), null));
        thisActivity.startActivity(intent);
    }
}