#ifndef EGG_LOG_H
#define EGG_LOG_H


#if __ANDROID__

#include <jni.h>
#include <errno.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "niubi_ndk", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "niubi_ndk", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,"niubi_ndk", __VA_ARGS__))

#elif (_WIN32 && (!__MINGW32__)) //windows

#include <stdio.h>
#include <stdarg.h>
#include <string>
#define LOGI( str , ...) (printf( (std::string("INFO:")		+ str +"\n").c_str()	, __VA_ARGS__ ) )
#define LOGW( str , ...) (printf( (std::string("WARNING:")	+ str +"\n").c_str()	, __VA_ARGS__ ) )
#define LOGE( str , ...) (printf( (std::string("ERROR:")	+ str +"\n").c_str()	, __VA_ARGS__ ) )

#else //linux ..... 

#include <stdio.h>
#include <stdarg.h>
#include <string>
#define LOGI( str , agrs...) (printf( (std::string("INFO:")	+ str +"\n").c_str()	, ##agrs ) )
#define LOGW( str , agrs...) (printf( (std::string("WARNING:")	+ str +"\n").c_str()	, ##agrs ) )
#define LOGE( str , agrs...) (printf( (std::string("ERROR:")	+ str +"\n").c_str()	, ##agrs ) )

#endif //NIUBI_USE_ANDROID


#endif//EGG_LOG_H

