 
# libwebsockets
## 编译
```shell
# 3.0.1版本
sh build_lws_3.0.1.sh
# 3.1.0版本
sh build_lws_3.1.0.sh
```

## 问题记录
- error: 'MSG_NOSIGNAL' macro redefined 

**3.1.0**
lib/plat/unix/private.h
**3.0.1**
lib/core/private.h
**官方修改方案：**
```c++
/*
 * Mac OSX as well as iOS do not define the MSG_NOSIGNAL flag,
 * but happily have something equivalent in the SO_NOSIGPIPE flag.
 */
#ifdef __APPLE__
/* iOS SDK 12+ seems to define it, undef it for compatibility both ways */
#undef MSG_NOSIGNAL
#define MSG_NOSIGNAL SO_NOSIGPIPE
#endif
```