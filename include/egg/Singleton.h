
#ifndef _EGG_SINGLETON_H_
#define _EGG_SINGLETON_H_

//#include <eggRuntime/Object.h>
#include <map>


/*
SINGLETON 主要用于核心对象
*/
#define DECLARE_SINGLETON(T) \
static T* mInstance;\
static T* getInstance(bool create = false);\
static void delInstance();

#define IMPLEMENT_SINGLETON(T) \
T* T::mInstance = 0;\
T* T::getInstance(bool create){if((mInstance==0) && create){mInstance = new T();} return mInstance;}\
void T::delInstance(){mInstance=0;}


/*
SINGLETON2 主要用于单例插件基础类
*/
#define DECLARE_SINGLETON2(T) \
static T* mInstance;\
static T* getInstance();\
static void setInstance(T* t);

#define IMPLEMENT_SINGLETON2(T) \
T* T::mInstance = 0;\
T* T::getInstance(){return mInstance;}\
void T::setInstance(T* t){mInstance=t;}


#endif //_EGG_SINGLETON_H_
