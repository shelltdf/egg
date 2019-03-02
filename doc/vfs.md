# 虚拟文件系统 vfs

## 设计思路

为了适应不同运行环境和打包的需求。还有系统设备上强制规定文件管理方式的需求。

也提供了文件覆盖模式的可能性。

VFS只是提供文件的操作功能，并不提供文件内存的管理。



## 主要类型

* FileSystem 系统对象

* Archive 本地包对象

* Entry 文件或者目录对象



通过继承 Archive  Entry 来扩展具体的支持方式：

* Native 本地目录
* ZIP zip文件
* SZP sqlite+QuickLZ
* Android 本地的assetmanager支持
* IOS支持  跟Native同样的实现 只是目录位置不同



## 主要功能点

FileSystem 可以添加多个 Archive 。后续添加的Archive 中的文件路径如果和前面的一样，加载文件的时候就回被覆盖。（这个功能是上层实现的，本层内完全暴露全部Archive 的内容）

