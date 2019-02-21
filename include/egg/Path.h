#ifndef NIUBI_BASE_PATH_H
#define NIUBI_BASE_PATH_H

#include <egg/Export.h>
//#include <nbBase/Log.h>

//#include <list>
#include <string>
#include <vector>
#include <deque>


/*
* 这里仅仅是一个路径处理对象
* 以后要把这里面和系统有关的功能全部移动到 system级别里处理
*/

namespace egg
{
    
    //!文件路径对象
	class EGG_RUNTIME_EXPORT Path
	{
	public:
		//Path(const char* path = "");
        Path(const std::string& path = "");
		~Path();

        //for std::map key
        bool operator < (const Path & cmp) const
        {
            if(mIsAbs != cmp.mIsAbs)
            {
                return mIsAbs < cmp.mIsAbs;
            }
            return getPathMel() < cmp.getPathMel();
        } 

        inline Path& operator = (const Path& r)
        {
            mPathList = r.mPathList;
            mIsAbs = r.mIsAbs;
            return *this;
        }
        inline Path& operator = (const char* path )
        {
            set(path);
            return *this;
        }
        inline Path& operator = (const std::string& str)
        {
            set( str.c_str() );
            return *this;
        }

		//是否是绝对路径
        bool isAbsolutePath()const{ return mIsAbs; }
        void setAbsolutePath(bool b){ mIsAbs = b; }

        //是否为无效
        //    如果不是绝对路径 而且路径为空 就认为是无效
        bool isNull(){return mPathList.empty() && (!mIsAbs); }

        //路径长度
        unsigned int size(){return mPathList.size(); }


		//设置路径
		//	参数1 路径名称
		//	      绝对路径必须用 / | c: 开始其他都被认作是先对路径
		//        同时支持混合路径输入
		void set( const char* path );

        //追加子路径
        //  参数1 必须是相对路径
		void push(const char* path);
        void push(const Path& path);

        //上升路径
        //  参数1 生成的层数
        //        绝对路径最多上升到盘符 相对路径最多上升到当前
		void pop(unsigned int level = 1);

        std::string getFront();
        void popFront();

        //获取末端名称
        std::string getLastName() const;

        //获取末端文件名按照.分割的数组
        std::vector< std::string > splitLastName() const;


		//获取制定格式的路径
		std::string getPathWindows(bool only_dir = false) const;//输出windows路径
		std::string getPathUnix(bool only_dir = false) const;//输出unix路径
        std::string getPathMel(bool only_dir = false,bool as_abs = false) const;//输出mel路径

        //输出当前系统路径
        std::string getNativePath(bool only_dir = false) const;


        //计算相对路径
        //    form + return = to
        static Path relative(const char* from_abs_path, const char* to_abs_path, bool from_is_file );
        static Path relative(Path from_abs_path, Path to_abs_path ,bool from_is_file ); //form路径是不是一个文件


	private:

		//存储结构
		std::deque< std::string > mPathList;

		//是否是绝对路径
		bool mIsAbs;
	};

}//namespace nbBase

#endif//NIUBI_BASE_PATH_H

