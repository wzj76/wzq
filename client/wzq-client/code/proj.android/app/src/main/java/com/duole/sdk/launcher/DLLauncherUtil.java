package com.duole.sdk.launcher;
import com.duole.launcher.LauncherUtils;
import org.cocos2dx.lib.Cocos2dxActivity;
import android.content.Intent;

// 壳，任何SDK都要在同意隐私政策之后再初始化
public class DLLauncherUtil {
	public static Cocos2dxActivity thisActivity = null;
	public static void init (Cocos2dxActivity activity) {
		thisActivity = activity;
	}

	// 设置是否同意 0未同意 1已同意
	public static void setPrivacyCheckBox(int nPlag) {
		LauncherUtils.setPrivacyCheckBox(thisActivity, nPlag);
	}

	// 显示用户服务协议
	public static void showPrivacyAgreement() {
		LauncherUtils.showPrivacyAgreement(thisActivity);
	}

	// 显示隐私保护指引
	public static void showPrivacyGuide() {
		LauncherUtils.showPrivacyGuide(thisActivity);
	}

	// 显示未成年隐私保护指引
	public static void showPrivacyGuideChildren() {
		LauncherUtils.showPrivacyGuideChildren(thisActivity);
	}

	// 获取启动游戏Intent传值 （用在启动传值 如vivo）
	public static Intent getLauncherIntent() {
		return LauncherUtils.getLauncherIntent();
	}

	// 显示ShadeImage（遮住家在游戏期间黑屏）
	public static void showShadeImageView() {
		LauncherUtils.showShadeImageView(thisActivity);
	}

	// 移除ShadeImage
	public static void removeShadeImageView() {
		LauncherUtils.removeShadeImageView();
	}
}