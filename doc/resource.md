# 内存资源管理



## 设计思路

把加载到内存的对象分组管理，也就是建立 ResourceGroup 分组。每个被加载的Resource都必须存放在一个组内。不然就被自动释放。

对于动态加载的数据，比如临时打开一个模型，或者地球数据分页加载，都可以建立一个临时组来管理。

ResourceManager 包含一个默认组 default



## 主要类型

ResourceManager

Resource

ResourceGroup 



