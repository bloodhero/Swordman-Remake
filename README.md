# 天地劫-神魔至尊传重制
重制经典的战旗游戏神魔至尊传，在保留原玩法的基础上进行改进。

# 开始
推荐使用 Visual Studio 2019，本游戏主要发布在windos平台。

使用 git 克隆本仓库：

`git clone git@github.com:bloodhero/Swordman-Remake.git`

执行 scripts/Win-GenProjects.bat 生成工程文件

使用 vs2019 打开 Swordman-Remake.sln 即可。

# 开发规范
依赖的库：
- entt 管理实体
- ffmpeg 音视频解码
- json 资源序列化
- nuklear GUI
- openal-soft 音频播放
- SDL2
- stb_image 图片解码
- spdlog 日志记录

命名规范：
- 类型名使用大驼峰命名法，类型包括Classes, Structures, Unions, Typedefs, Templates
- 数据成员使用带前缀的大驼峰命名法
- 成员函数使用小驼峰命名法
- 常量、枚举、宏 使用大写命名法
- 局部变量和命名空间使用蛇形命名法

成员函数分类：
- CREARTORS 管理对象的构建与销毁
- ACCESSORS const成员函数
- MANIPULATORS 非const成员函数

# 计划
1. 先复刻原游戏的玩法。
2. 借鉴合金装备和平行者的养成玩法（主基地养成）和放置玩法（派遣雇佣兵做任务）。
