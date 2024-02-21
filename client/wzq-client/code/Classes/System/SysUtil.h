#ifndef _INC_SYSUTIL_H_
#define _INC_SYSUTIL_H_

#include <string>

//定义网络类型
enum NETWORK_TYPE
{
	NETWORK_TYPE_MOBILE = 1,
	NETWORK_TYPE_WIFI = 2,
	NETWORK_TYPE_BLUETOOTH = 7,
	NETWORK_TYPE_ETHERNET = 9
};

class CSysUtil
{
public:
    static bool GetMobileID(std::string& strMobileID);
	//取得电话号码
	static bool GetPhoneNumber(std::string& strPhoneNumber);
	//取得IMSI，可通过IMSI区分运营商
	//46000/46002:中国移动 46001:中国联通 46003:中国电信
	static bool GetIMSI(std::string& strIMSI);
	static bool GetCacheDir(std::string& strCacheDir);
	static bool GetExternalCacheDir(std::string& strCacheDir);
	static bool GetFilesDir(std::string& strCacheDir);
	static bool GetExternalStorageDirectory(std::string& strDir);
	static bool GetPackageName(std::string& strName);
    static bool GetVersionName(std::string& strVersion);
    static bool GetApplicationMetaData(const char* pszMetaName, std::string& strMetaVal);
    static bool GetBuildModel(std::string& strModel);
    static bool GetBuildManufacturer(std::string& strManufact);
    static bool GoToURL(const char* pszURL, std::string& strVale);
	static bool GetWriteablePath(std::string& strPath);
    static bool AppendDataToFile(std::string strFileName, const void* pBuf, int nSize);
    
public:
	//判断网络是否有效
	static bool IsNetworkConnected();
	//判断WIFI网络是否可用
	static bool IsWifiConnected();
	//判断MOBILE网络是否可用
	static bool IsMobileConnected();
	//获取当前网络连接的类型信，类型定义见enum NETWORK_TYPE
	static int GetConnectedType();
    //获取当前网络连接的强度
    static int GetSignalStrength();
    
public:
	//crash report
	static void CrashHandlerSetup();
	static bool s_bCrashSetupCompleted;
};

#endif//_INC_SYSUTIL_H_
