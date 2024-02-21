# resolved warning: ABIs [arm64-v8a,armeabi-v7a,armeabi] set by 'android.injected.build.abi' gradle flag contained 'ARME
# c++_static -> c++_shared
# APP_STL := c++_static
APP_STL := c++_shared

APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char -Wno-extern-c-compat
APP_LDFLAGS := -latomic

APP_ABI := armeabi-v7a
APP_SHORT_COMMANDS := true


ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif
