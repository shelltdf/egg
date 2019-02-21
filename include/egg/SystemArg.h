#ifndef EGG_SYSTEM_ARG_H
#define EGG_SYSTEM_ARG_H


#if __ANDROID__
#include <jni.h>
#include <errno.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#endif //__ANDROID__

namespace egg
{

	struct niubi_entry_arg
	{

#if __ANDROID__

		niubi_entry_arg():state(0),native_window(0),isinit(0){}
		android_app* state;
		void* native_window;
		bool isinit;

		//unsigned int display;
		//unsigned int context;
		//unsigned int surface;
		unsigned int width;
		unsigned int height;


#else //windows linux ..... 

		niubi_entry_arg():argv(0){}
		int argc;
		char** argv;

#endif

	};

}//namespace egg


#endif//EGG_SYSTEM_ARG_H
