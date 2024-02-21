#ifndef	__BASEDEFINE_H__
#define __BASEDEFINE_H__


#ifndef WIDEN2
#define WIDEN2(x) L ## x
#endif
#ifndef WIDEN
#define WIDEN(x) WIDEN2(x)
#endif
#ifndef __WFILE__
#define __WFILE__ WIDEN(__FILE__)
#endif

//----------------------------------------------------------------------------------------------------------------
//	字段长度定义，注意一般都以0结尾，因此实际可用长度是缓冲区-1
//----------------------------------------------------------------------------------------------------------------
#ifndef BUFLEN_HOST
#define BUFLEN_HOST					(49+1)		//服务器地址
#endif
#define	BUFLEN_IP					(15+1)		//IP地址长度，xxx.xxx.xxx.xxx
#define	BUFLEN_URL					(255+1)		//URL地址

#define	BUFLEN_USERNAME				(79+1)		//用户名长度
#define BUFLEN_NICKNAME				(79+1)		//中文名称长度
#define	BUFLEN_PASSWORD				(15+1)		//密码长度
#define BUFLEN_MOBILENO				(19+1)		//手机号码长度
#define	BUFLEN_REALNAME				(19+1)		//姓名长度
#define	BUFLEN_PHONE					(19+1)		//电话号码
#define	MIN_USERNAME					2			//用户名最短长度
#define	MIN_PASSWORD					3			//密码最短长度

#define	BUFLEN_SERVICENAME			20			//服务名称长度

#define	BUFLEN_GUID					64			//GUID长度
#define	BUFLEN_ROLENAME				80			//角色长度
#define	BUFLEN_TIME					20			//时间长度



//----------------------------------------------------------------------------------------------------------------
//	标准错误代码
//----------------------------------------------------------------------------------------------------------------
#define BIT_FAIL						0			//失败
#define BIT_SUCCESS					1			//成功
#define BIT_SOCKETFAIL				-1			//socket操作失败，可能为连接、发送、接收
#define BIT_CONNECTFAIL				-2			//服务器连接失败
#define BIT_SENDFAIL					-3			//数据发送失败
#define BIT_RECVFAIL					-4			//数据接收失败
#define BIT_OUTOFMEMORY				-5			//内存不足，指服务器端
#define	BIT_BUSY						-6			//服务忙
#define	BIT_TIMEOUT					-7			//超时
#define	BIT_FILENOTFOUND				-8			//文件未找到
#define	BIT_CREATEFILEERROR			-9			//文件创建失败
#define	BIT_OPENFILEERROR				-10			//文件打开失败
#define	BIT_READFILEERROR				-11			//文件读错误
#define	BIT_WRITEFILEERROR			-12			//文件写错误
#define	BIT_NOTFOUND					-13			//由于指定的内容没有找到引发的错误
#define	BIT_INVALIDPARAM				-14			//参数错误
#define	BIT_DONOTHING					-15			//不需要进行操作
#define	BIT_NOENOUGHMONEY				-16			//财富值不够
#define	BIT_NOTSUPPORT				-17			//不支持
#define	BIT_NOENOUGHCOIN				-18			//币不够

#define	BIT_USER						-2000			//用户自定义的返回错误信息从这个编号开始


#endif // __BASEDEFINE_H__
