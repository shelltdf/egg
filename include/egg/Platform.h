#ifndef _EGG_PLATFORM_H_
#define _EGG_PLATFORM_H_

#include <egg/Export.h>

#include <egg/Singleton.h>

#include <string>
#include <vector>

namespace egg
{
    class EGG_RUNTIME_EXPORT Platform
    {
    public:
        Platform();
        ~Platform();

        //manager(Platform)
        DECLARE_SINGLETON(Platform)

        //目录操作全部移动到system dir里

        //创建多级文件路径
        int createDirectory(const char* path);

        //路径是否存在
        //  返回值 0不存在 1文件 2目录
        int isExist(const char* path);

        //查询文件长度
        unsigned int Length(const char* path);

        //删除文件按或者目录
        void removeFile(const char* path, bool force_delete_non_enpty_dir = false);

        //列举目录中的路径
        //  dir 列表起算位置
        //  path dir内的路径
        //  ergodic 递归所有子目录
        std::vector<std::string> listFile(const char* dir, const char* path, bool ergodic = false);


        //这些全部移动到system dir对象里
        std::string getApplicationDir();
        std::string getApplicationUrl();
        std::string getAppStorageDir();
        std::string getAppStorageUrl();
        std::string getCacheDir();
        std::string getCacheUrl();
        std::string getDesktopDir();
        std::string getDesktopUrl();
        std::string getDocumentsDir();
        std::string getDocumentsUrl();

    private:

    };

} //namespace egg


#endif // !_EGG_PLATFORM_H_
