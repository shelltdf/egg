
#include <egg/FileDirUtil.h>

#include <windows.h>

#include <egg/StringUtil.h>

using namespace egg;

std::string getCurrentDirectory()
{
    int size = GetCurrentDirectory(0, 0);

    std::string str;
    str.resize(size);

    size = GetCurrentDirectory(size, &str[0]);

    return str;
}

int mkdir_r(const std::string& path)
{
    std::string p;

    std::vector<std::string> sl = split( path, "/", true);
    for (size_t i = 0; i < sl.size(); i++)
    {
        p += sl[i];
        if ((sl[i] == "." || sl[i] == ".."))
        {
        }
        else
        {
            printf("mkdir:%s\n",p.c_str());
            mkdir(p.c_str());
        }
        p += "/";
    }

    return 0;
}


std::vector<std::string> getFilesInDir(std::string cate_dir)
{
    std::vector<std::string> files;//存放文件名  

#if 0
    path current_dir(cate_dir); //
    //boost::regex pattern("a.*"); // list all files starting with a
    for (recursive_directory_iterator iter(current_dir), end;
        iter != end;
        ++iter)
    {
        std::string name = iter->path().filename().string();
        //if (regex_match(name, pattern))
        {
            //std::cout << iter->path() << "\n";
            std::wstring ws = iter->path().c_str();
            //files.push_back(osgCore::w2m(ws, CP_UTF8));
            files.push_back(osgCore::w2m(ws, CP_ACP));
        }
    }


#else

#ifdef WIN32

    egg::Path p(cate_dir);
    cate_dir = p.getPathWindows();
    cate_dir += "\\*";

    _finddata_t file;
	intptr_t lf;

	//搜索第一个文件失败  
	if ((lf = _findfirst(cate_dir.c_str(), &file)) == -1)
	{
		std::cout << cate_dir << " not found!!!" << std::endl;
		return files;
	}

	//遍历全部文件
	do
	{
		//输出文件名  
		//cout<<file.name<<endl;  
		if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
			continue;
		files.push_back(file.name);

	} while (_findnext(lf, &file) == 0);

	//关闭
	_findclose(lf);

#endif  

#ifdef linux  
    cate_dir += "/";

    DIR *dir;
    struct dirent *ptr;
    char base[1000];

    if ((dir = opendir(cate_dir.c_str())) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr = readdir(dir)) != NULL)
    {
        if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parrent dir  
            continue;
        else if (ptr->d_type == 8)    ///file  
                                      //printf("d_name:%s/%s\n",basePath,ptr->d_name);  
            files.push_back(ptr->d_name);
        else if (ptr->d_type == 10)    ///link file  
                                       //printf("d_name:%s/%s\n",basePath,ptr->d_name);  
            continue;
        else if (ptr->d_type == 4)    ///dir  
        {
            files.push_back(ptr->d_name);
            /*
            memset(base,'\0',sizeof(base));
            strcpy(base,basePath);
            strcat(base,"/");
            strcat(base,ptr->d_nSame);
            readFileList(base);
            */
        }
    }
    closedir(dir);
#endif  

    //排序，按从小到大排序  
    //std::sort(files.begin(), files.end());

#endif  
    return files;
}


bool isFileExists(const std::string& filename)
{
    if ((_access(filename.c_str(), 0)) != -1)
    {
        return true;
    }
    return false;
}

bool isDirExists(const std::string& dir)
{
#ifdef WIN32
    struct _stat fileStat;
    if ((_stat(dir.c_str(), &fileStat) == 0) && (fileStat.st_mode & _S_IFDIR))
    {
        return true;
    }
#endif

#ifdef linux  
    struct stat fileStat;
    if ((stat(dir.c_str(), &fileStat) == 0) && S_ISDIR(fileStat.st_mode))
    {
        isExist = true;
    }
#endif

    return false;
}

