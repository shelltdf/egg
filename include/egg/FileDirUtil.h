
#ifndef NIUBI_BASE_FILE_DIR_UTIL_H
#define NIUBI_BASE_FILE_DIR_UTIL_H

#include <egg/Export.h>

//#include <direct.h>  
#include <string>
#include <vector>

#include <iostream>  
#include <stdlib.h>  
#include <stdio.h>  
//#include <string.h>  
#ifdef linux  
#include <unistd.h>  
#include <dirent.h>  
#endif  
#ifdef WIN32  
#include <direct.h>  
#include <io.h>  
#endif  

#if 0
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
using namespace boost::filesystem;
#endif

#include <egg/Path.h>
#include <egg/Codes.h>


//查询当前目录
EGG_RUNTIME_EXPORT std::string getCurrentDirectory();

//创建多级菜单 必须是相对目录 ./ ../开头
EGG_RUNTIME_EXPORT int mkdir_r(const std::string& path);

//递归目录生成文件列表 UTF-8
EGG_RUNTIME_EXPORT std::vector<std::string> getFilesInDir(std::string cate_dir);


//文件是否存在
EGG_RUNTIME_EXPORT bool isFileExists(const std::string& filename);

//目录是否存在
EGG_RUNTIME_EXPORT bool isDirExists(const std::string& dir);

#endif //NIUBI_BASE_FILE_DIR_UTIL_H
