PROJECT(egg)
SET(PROJECT_NAME egg )

IF(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules/")
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.7)
include(niubiMacroUtils)

# IF(ANDROID)
    # include(apk/apk)
    # include(AndroidNdkModules)
    # android_ndk_import_module_ndk_helper()
    # android_ndk_import_module_native_app_glue()
# ENDIF(ANDROID)

SET(NIUBI_MAJOR_VERSION 0)
SET(NIUBI_MINOR_VERSION 1)
SET(NIUBI_PATCH_VERSION 0)
SET(NIUBI_VERSION ${OSGXI_MAJOR_VERSION}.${OSGXI_MINOR_VERSION}.${OSGXI_PATCH_VERSION})


OPTION(USE_QT5 "" OFF)
OPTION(USE_GTEST "" OFF)
OPTION(USE_OSG "" OFF)
OPTION(USE_OSGEARTH "" OFF)


FIND_PACKAGE(Doxygen)
FIND_PACKAGE(OpenGL)
# FIND_PACKAGE(PhysFS)
# FIND_PACKAGE(ZLIB)
# FIND_PACKAGE(DX9)
# FIND_PACKAGE(Mali)
#FIND_PACKAGE(NVGLES)
# FIND_PACKAGE(FLTK)  #1.3.0
# FIND_PACKAGE(maya)#2013 32bit en_US
# FIND_PACKAGE(maxsdk)
# FIND_PACKAGE(ffmpeg)
FIND_PACKAGE(fbx)
FIND_PACKAGE(imgui)
FIND_PACKAGE(glew)
FIND_PACKAGE(Eigen)
FIND_PACKAGE(rapidxml)
# FIND_PACKAGE(PhysX3)
FIND_PACKAGE(eigen3)

# if(NOT ANDROID)
if(USE_OSG)
FIND_PACKAGE(openThreads)
FIND_PACKAGE(osg)
FIND_PACKAGE(osgDB)
FIND_PACKAGE(osgUtil)
FIND_PACKAGE(osgGA)
FIND_PACKAGE(osgViewer)
FIND_PACKAGE(osgText)
FIND_PACKAGE(osgAnimation)
FIND_PACKAGE(osgShadow)
FIND_PACKAGE(osgTerrain)
FIND_PACKAGE(osgAnimation)

if(USE_OSGEARTH)
FIND_PACKAGE(osgEarth)
endif(USE_OSGEARTH)

# FIND_PACKAGE(osgBullet)
# FIND_PACKAGE(osgWorks 2.0.0 REQUIRED)
# endif(NOT ANDROID)
endif(USE_OSG)

# FIND_PACKAGE(rqsdk)
# FIND_PACKAGE(niubi)


# include( FindBulletHelper )

# if( osgWorks_FOUND )
    # include( ${osgWorks_USE_FILE} )
# endif()

# if( osgBullet_FOUND )
    # INCLUDE_DIRECTORIES( 
        # ${BULLET_INCLUDE_DIRS}
        # ${BULLET_EXTRAS_INCLUDE_DIR}
        # ${OSGBULLET_P5_INCLUDE_DIR}
        # ${OSGBULLET_INCLUDE_DIRS}
        # )
    # option( OSGBULLET_USE_DOUBLE_PRECISION "" OFF )
    # if( OSGBULLET_USE_DOUBLE_PRECISION )
        # add_definitions( -DBT_USE_DOUBLE_PRECISION )
    # endif()
# endif(osgBullet_FOUND)



SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# SET(NIUBI_DEBUG_POSTFIX "d" CACHE STRING "use postfix to link with osg with debug")

IF(WIN32)
    IF(MSVC)
        OPTION(WIN32_USE_MP "Set to ON to build OpenSceneGraph with the /MP option (Visual Studio 2005 and above)." OFF)
        MARK_AS_ADVANCED(WIN32_USE_MP)
        IF(WIN32_USE_MP)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        ENDIF(WIN32_USE_MP)

        # More MSVC specific compilation flags
        ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
        ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
    ENDIF(MSVC)
ENDIF(WIN32)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} CACHE STRING "You may add additional search paths here. Use ; to separate multiple paths.")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} CACHE STRING "You may add additional search paths here. Use ; to separate multiple paths.")


IF (NOT WIN32)
    SET(CMAKE_CXX_FLAGS "-W -Wall -Wno-unused")
ENDIF(NOT WIN32)


# Here we apparantly do some funky stuff with making the bin/ and lib/
# folders which is probably needed to work around a very old CMake bug?

#SET(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin/${CMAKE_SYSTEM_NAME})
SET(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin)
MAKE_DIRECTORY(${OUTPUT_BINDIR})
IF(MSVC AND NOT MSVC_IDE)
    MAKE_DIRECTORY(${OUTPUT_BINDIR}/${OSG_PLUGINS})
ENDIF(MSVC AND NOT MSVC_IDE)

#SET(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib/${CMAKE_SYSTEM_NAME})
SET(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib)
MAKE_DIRECTORY(${OUTPUT_LIBDIR})
IF(NOT MSVC OR MSVC_IDE)
    MAKE_DIRECTORY(${OUTPUT_LIBDIR}/${OSG_PLUGINS})
ENDIF(NOT MSVC OR MSVC_IDE)

# On CMake 2.4.x use EXECUTABLE_OUTPUT_PATH and LIBRARY_OUTPUT_PATH and later
# we work around the DLL placement by use of the PREFIX target property hack
#
# On CMake 2.6.x use the newly minted CMAKE_LIBRARY_OUTPUT_DIRECTORY,
# CMAKE_ARCHIVE_OUTPUT_DIRECTORY & CMAKE_RUNTIME_OUTPUT_DIRECTORY

IF(CMAKE_MAJOR_VERSION EQUAL 2 AND CMAKE_MINOR_VERSION LESS 5)
    SET(EXECUTABLE_OUTPUT_PATH ${OUTPUT_BINDIR})
    SET(LIBRARY_OUTPUT_PATH    ${OUTPUT_LIBDIR})
ELSE()
    # If CMake >= 2.6.0
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_LIBDIR})
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BINDIR})
    IF(WIN32)
        SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_BINDIR})
    ELSE(WIN32)
        SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_LIBDIR})
    ENDIF(WIN32)
ENDIF()


# set postfix
SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
# SET(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "add a postfix, usually empty on windows")
# SET(CMAKE_MINSIZEREL_POSTFIX "s" CACHE STRING "add a postfix, usually empty on windows")

INCLUDE_DIRECTORIES(include ${CMAKE_INCLUDE_PATH})
LINK_DIRECTORIES(${CMAKE_LIBRARY_PATH})


#默认只建立主工程
OPTION(BUILD_EGG_ONLY "只建造核心" ON)
#OPTION(BUILD_TESTER "建造测试程序" ON)
#OPTION(BUILD_PLUGINS "建造插件" ON)
OPTION(BUILD_DOCUMENTATION "Build  documentation" OFF)
OPTION(BUILD_SAMPLES "建造测试程序" OFF)
OPTION(BUILD_STATIC_LIBRAY "建造测试程序" OFF)
OPTION(BUILD_WINDOWS_SYSTEM "建造窗口程序" OFF)

set( SELECT_PLATFORM "1" CACHE STRING "1 Windows\n2 Linux X11\n3 FlasCC\n4 emscripten\n5 ndk\n" )
set( SELECT_GRAPHICS_INTERFACE "1" CACHE STRING "1 OpenGL 2.1\n2 D3D 9\n3 OpenGL ES 1.x\n4 OpenGL ES 2.x\n" )




#系统信息 
#编译环境信息 
#编译器信息

IF( CYGWIN )
	MESSAGE( "system: windows")
	MESSAGE( "environment: cygwin")
	MESSAGE( "compiler: gcc")
ENDIF( CYGWIN )

IF( MSYS AND MINGW )
	MESSAGE( "system: windows")
	MESSAGE( "environment: msys")
	MESSAGE( "compiler: mingw-gcc")
ENDIF( MSYS AND MINGW )

IF( WIN32 AND NOT CYGWIN AND NOT MINGW )
	MESSAGE( "system: windows")
	IF(MSVC_IDE)
		MESSAGE( "environment: MSVC_IDE")
		MESSAGE( "compiler: MSVC")
	ELSE(MSVC)
		MESSAGE( "environment: NMAKE")
		MESSAGE( "compiler: MSVC")
	ENDIF(MSVC_IDE)
ENDIF( WIN32 AND NOT CYGWIN AND NOT MINGW)


IF( APPLE )
	MESSAGE( "system: Mac OS")
	IF(XCODE_VERSION)
		MESSAGE( "environment: XCODE")
	ELSE(XCODE_VERSION)
		MESSAGE( "environment: Unix make")
	ENDIF(XCODE_VERSION)
    MESSAGE( "compiler: GCC")
ENDIF( APPLE )

IF( UNIX AND NOT EMSCRIPTEN AND NOT ANDROID)
    MESSAGE( "system: UNIX")
    MESSAGE( "environment: Unix make")
    MESSAGE( "compiler: GCC")
    
    SET(SELECT_PLATFORM 2)
ENDIF(UNIX AND NOT EMSCRIPTEN AND NOT ANDROID)

IF( EMSCRIPTEN )
    MESSAGE( "system: EMC")
    MESSAGE( "environment: EMC")
    MESSAGE( "compiler: EMC")
    
    SET(SELECT_GRAPHICS_INTERFACE 3)
    SET(SELECT_PLATFORM 4)
ENDIF( EMSCRIPTEN )
    
IF(ANDROID)
    MESSAGE( "system: NDK")
    MESSAGE( "environment: NDK")
    MESSAGE( "compiler: NDK")
    
    SET(SELECT_GRAPHICS_INTERFACE 4) #3 gles1 #4 gles2
    SET(SELECT_PLATFORM 5)
ENDIF(ANDROID)

#开启c++11支持
if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions( -std=c++11 )
    set(CMAKE_CXX_FLAGS -std=c++11 ${CMAKE_CXX_FLAGS})
endif()


add_definitions( -DNOMINMAX )


#第三方库路径
SET( _3RDPARTY_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/3rdParty/include)
# SET( _3RDPARTY_INCLUDE_DIR ${_3RDPARTY_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/3rdParty/FastLZ_trunk/)
SET( _3RDPARTY_INCLUDE_DIR ${_3RDPARTY_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/3rdParty/rapidxml-1.13/)
# SET( _3RDPARTY_INCLUDE_DIR ${_3RDPARTY_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/3rdParty/smart-pointers-1.0/)
# SET( _3RDPARTY_INCLUDE_DIR ${_3RDPARTY_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/3rdParty/vmmlib_trunk/include/)
SET( _3RDPARTY_INCLUDE_DIR ${_3RDPARTY_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/3rdParty/vmmlib-master/)
SET( _3RDPARTY_INCLUDE_DIR ${_3RDPARTY_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/3rdParty/glew-1.9.0/include/)
# SET( _3RDPARTY_INCLUDE_DIR ${_3RDPARTY_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/3rdParty/cereal-master/include/)
SET( _3RDPARTY_INCLUDE_DIR ${_3RDPARTY_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/3rdParty/stb-master/)
SET( _3RDPARTY_INCLUDE_DIR ${_3RDPARTY_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/3rdParty/include/eigen3/)
# SET( _3RDPARTY_INCLUDE_DIR ${_3RDPARTY_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/3rdParty/eigen-eigen-5a0156e40feb/)
# SET( _3RDPARTY_INCLUDE_DIR ${_3RDPARTY_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/3rdParty/eigen-eigen-b3f3d4950030/)
SET( _3RDPARTY_INCLUDE_DIR ${_3RDPARTY_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/3rdParty/imgui-master/)
#MESSAGE( WARING ${_3RDPARTY_INCLUDE_DIR})      #工程目录

# include_directories(${_3RDPARTY_INCLUDE_DIR})

add_definitions(-DCMAKE_DEBUG_POSTFIX="'${CMAKE_DEBUG_POSTFIX}'")

if(EMSCRIPTEN)
add_definitions(-DEMSCRIPTEN)
endif(EMSCRIPTEN)

include_directories(${EIGEN_INCLUDE_DIR} )
include_directories(${rapidxml_INCLUDE_DIR} )

#核心
ADD_SUBDIRECTORY(src)

if( NOT BUILD_EGG_ONLY)
ADD_SUBDIRECTORY(modules)
ADD_SUBDIRECTORY(app)
endif( NOT BUILD_EGG_ONLY)

if(USE_GTEST)
ADD_SUBDIRECTORY(test)
endif(USE_GTEST)


# ADD_SUBDIRECTORY(src/eggRuntime)
# ADD_SUBDIRECTORY(src/eggEngine)
# ADD_SUBDIRECTORY(src/eggEditor)
# ADD_SUBDIRECTORY(src/eggApplication)
 
# ADD_SUBDIRECTORY(src/nbBase)
# ADD_SUBDIRECTORY(src/nbFS)
# ADD_SUBDIRECTORY(src/nbPlugins)
# ADD_SUBDIRECTORY(src/nbSG)
# ADD_SUBDIRECTORY(src/nbNet)
IF(OSG_FOUND)
    # ADD_SUBDIRECTORY(src/osgMesh)
    # ADD_SUBDIRECTORY(src/osgBEH)
    # ADD_SUBDIRECTORY(src/osgCore)
    # ADD_SUBDIRECTORY(src/osgEFT)
    # ADD_SUBDIRECTORY(src/osgPTC)
ENDIF(OSG_FOUND)
    
# windows 
# IF( MSVC OR MINGW )
	# ADD_SUBDIRECTORY(src/Plugins/Launcher_win32)
# ENDIF()

#liunx cygwin
IF( MSVC OR MINGW )
ENDIF()

#x11
# IF( UNIX AND NOT EMSCRIPTEN AND NOT ANDROID)
    # ADD_SUBDIRECTORY(src/plugins/Launcher_x11)
# ENDIF(UNIX AND NOT EMSCRIPTEN AND NOT ANDROID)


# IF( EMSCRIPTEN )
    # ADD_SUBDIRECTORY(src/plugins/Launcher_emc)
    # ADD_SUBDIRECTORY(src/Plugins/GraphicsContext_egl_emc)
    # ADD_SUBDIRECTORY(src/Plugins/Renderer_gles_1_x)
# ENDIF( EMSCRIPTEN )


# opengl 
# IF( OPENGL_FOUND AND (SELECT_GRAPHICS_INTERFACE EQUAL 1))
    # INCLUDE_DIRECTORIES(AFTER ${OPENGL_INCLUDE_DIR})
	# ADD_SUBDIRECTORY(src/plugins/Renderer_gl_2_0)
	# IF( MSVC OR MINGW )
		# ADD_SUBDIRECTORY(src/Plugins/GraphicsContext_wgl)
	# ENDIF()
    # IF( UNIX )
        # ADD_SUBDIRECTORY(src/plugins/GraphicsContext_glx)
    # ENDIF(UNIX)
# ENDIF()

# dx9
# IF( DX9_FOUND AND MSVC AND (SELECT_GRAPHICS_INTERFACE EQUAL 2))
	# ADD_SUBDIRECTORY(src/Plugins/GraphicsContext_dx9)
	# ADD_SUBDIRECTORY(src/Plugins/Renderer_dx_9)
# ENDIF()

# gles (moniqi ANDROID IOS)
# IF( (MALI_FOUND AND MSVC) OR ANDROID OR IOS )

    # IF( SELECT_GRAPHICS_INTERFACE EQUAL 3)
        # ADD_SUBDIRECTORY(src/Plugins/Renderer_gles_1_x)
    # ENDIF()

    # IF( SELECT_GRAPHICS_INTERFACE EQUAL 4)
        # ADD_SUBDIRECTORY(src/Plugins/Renderer_gles_2_x)
    # ENDIF()
	
    # IF(ANDROID)
        # IF( SELECT_GRAPHICS_INTERFACE EQUAL 3)
            # ADD_SUBDIRECTORY(src/Plugins/GraphicsContext_egl_ndk)
        # ENDIF()
        # IF( SELECT_GRAPHICS_INTERFACE EQUAL 4)
            # ADD_SUBDIRECTORY(src/Plugins/GraphicsContext_egl_ndk)
        # ENDIF()
        # ADD_SUBDIRECTORY(src/Plugins/Launcher_ndk)
    # ENDIF(ANDROID)
    
	# IF( MALI_FOUND AND MSVC )
		# ADD_SUBDIRECTORY(src/Plugins/GraphicsContext_egl_mali) #支持GLES 1 2 3
	# ENDIF()
	
# ENDIF()



#建立范例程序
IF ( BUILD_SAMPLES )

    #渲染测试
    # ADD_SUBDIRECTORY(example/test_render_a)
    # ADD_SUBDIRECTORY(example/test_render_00)
    # ADD_SUBDIRECTORY(example/test_render_01)
    
    #视频播放测试 和 TCP测试
    # ADD_SUBDIRECTORY(example/test_ftp_server)
    # ADD_SUBDIRECTORY(example/test_tcp_server)
    # ADD_SUBDIRECTORY(example/test_ffmpeg_player)
    # ADD_SUBDIRECTORY(example/project_net_palyer)
    
    #场景测试
    # ADD_SUBDIRECTORY(example/test_scene)
    # ADD_SUBDIRECTORY(example/test_scene_02)
    
    #zmq测试
    # ADD_SUBDIRECTORY(example/test_zmq_server)
    # ADD_SUBDIRECTORY(example/test_zmq_client)
    # ADD_SUBDIRECTORY(example/test_zhttpd)
    # ADD_SUBDIRECTORY(example/test_zhttp)
    
    #udp测试
    # ADD_SUBDIRECTORY(example/test_udp_all)#测试 单播 组播 广播
    # ADD_SUBDIRECTORY(example/heartbeat_server)
    # ADD_SUBDIRECTORY(example/heartbeat_client)
    
    #ns
    # ADD_SUBDIRECTORY(example/test_ns_tcp_udp)
    # ADD_SUBDIRECTORY(example/http_file_server)
    
if(NOT ANDROID)    
    # ADD_SUBDIRECTORY(example/test_path_and_filesystem) #文件系统测试
    # ADD_SUBDIRECTORY(example/test_nbsg)                #GL简单接口测试
    # ADD_SUBDIRECTORY(example/test_nbsg2)                #场景图测试
    # ADD_SUBDIRECTORY(example/test_nbsg3)                #场景图测试 把VEIW作为根节点测试
    # ADD_SUBDIRECTORY(example/test_vbo)                  #测试VBO功能
    # ADD_SUBDIRECTORY(example/test_shader_tree)          #shader虚拟语法树生成
    # ADD_SUBDIRECTORY(example/test_dom)                  #测试DOM的读写操作
    # IF(FBX_FOUND)
        # ADD_SUBDIRECTORY(example/test_fbx2dom)              #测试FBX 转换到 DOM
    # ENDIF(FBX_FOUND)    
    IF(OSG_FOUND)
        # ADD_SUBDIRECTORY(example/osgmovie)            #大屏幕程序
        # ADD_SUBDIRECTORY(example/osgpad)              #PAD控制端
        # ADD_SUBDIRECTORY(example/osggait)            #步态生成
        # ADD_SUBDIRECTORY(example/osggait2)           #步态生成 基于简单的抬脚逻辑
        # ADD_SUBDIRECTORY(example/osgik)              #ik测试
        
        # ADD_SUBDIRECTORY(example/osgeft)              #测试pssm
        # ADD_SUBDIRECTORY(example/osgeft_ssao)         #测试ssao
        # ADD_SUBDIRECTORY(example/osgeft_city)         #完整城市效果
        
        # ADD_SUBDIRECTORY(example/osgeffect)              #老版本移植
        # ADD_SUBDIRECTORY(example/osgFrustrum)              #测试视锥体
        
        # ADD_SUBDIRECTORY(example/geosplit)              #切分geometry对象
        # ADD_SUBDIRECTORY(example/buildplod)             #建立plod的八叉树
        # ADD_SUBDIRECTORY(example/bimviewer)             #观察器
        
        # ADD_SUBDIRECTORY(example/detect_box)             #测试detect_box函数
        
        # ADD_SUBDIRECTORY(example/osgocclusionquery)             #测试osgocclusionquery
        # ADD_SUBDIRECTORY(example/test_matrix)             #测试矩阵计算效率
        
        # ADD_SUBDIRECTORY(example/osgCameraCull)           #测试摄像机更新过程
        
        
        # ADD_SUBDIRECTORY(example/osgeft_forward)           #前向软渲染BOX
        # ADD_SUBDIRECTORY(example/osgeft_forward_hard)      #前向硬件渲染BOX
        
        # ADD_SUBDIRECTORY(example/osgeft_bluesky)      #测试蓝天模拟器的水波纹
        # ADD_SUBDIRECTORY(example/osgeft_bluesky_cfg)  #测试蓝天模拟器的cfg解析
        # ADD_SUBDIRECTORY(example/osgeft_bluesky_osg)  #测试蓝天模拟器的cfg解析
        # ADD_SUBDIRECTORY(example/osgeft_texinfo)      #输出文件中的贴图信息
        
        
        # ADD_SUBDIRECTORY(src/osgPlugins/physfs)  #physfs插件
        # ADD_SUBDIRECTORY(example/test_physfs)    #physfs插件测试
        
        # ADD_SUBDIRECTORY(src/osgPlugins/fbx)  #fbx插件
        
        # ADD_SUBDIRECTORY(example/test_fbx2osg)    #新的FBX读插件
        # ADD_SUBDIRECTORY(example/test_mesh)    #测试VBO共享
        
        # ADD_SUBDIRECTORY(example/dir_viewer)    #目录观察器
        
        # ADD_SUBDIRECTORY(example/osgPhysxCar)    #基于physx的车辆模拟
        # ADD_SUBDIRECTORY(example/osgPhysxCar2)    #基于physx的车辆模拟
        
        
        # ADD_SUBDIRECTORY(example/test_mem)
        
        # ADD_SUBDIRECTORY(example/osgRevitManipulator) #播放revit的操作器
        # ADD_SUBDIRECTORY(example/ziptool) #zip压缩工具
        # ADD_SUBDIRECTORY(example/zipinfo) #zip对象统计
        
        # ADD_SUBDIRECTORY(example/skybox) #skybox
        
        # ADD_SUBDIRECTORY(example/test_mark) #测试mark标记对象

        # ADD_SUBDIRECTORY(src/osgPlugins/pcds)  #pcds插件
        
        # ADD_SUBDIRECTORY(example/test_revite_export) #测试revit输出的xml
        
    ENDIF(OSG_FOUND)
    
    
    
endif(NOT ANDROID)    

    # ADD_SUBDIRECTORY(example/nbViewer) #nv模型观察器

if(NOT ANDROID)
    
    # ADD_SUBDIRECTORY(example/test_bsp2d) #测试2d bsp算法实现
    
    #基于 mg fossa ns的测试 (在win8.1上 vs2012 32 64测试 各种bug)
    # ADD_SUBDIRECTORY(example/mg_client) #基于mg的udp客户端
    # ADD_SUBDIRECTORY(example/mg_server) #基于mg的udp服务器
    
    #原始的UDP和TCP支持
    # ADD_SUBDIRECTORY(example/test_udp_all)
    # ADD_SUBDIRECTORY(example/test_tcp_server)
    # ADD_SUBDIRECTORY(example/test_tcp_client)    
    
    #基于原始TCP接口的测试
    # ADD_SUBDIRECTORY(example/nw_client)
    # ADD_SUBDIRECTORY(example/nw_server)
    
    #启动器
    # ADD_SUBDIRECTORY(example/launcher) #单机版测试
    #ADD_SUBDIRECTORY(example/launcher2) #网络版测试

	# ADD_SUBDIRECTORY(example/beanbag_game) #丢沙包游戏
    
endif(NOT ANDROID)
    
ENDIF( BUILD_SAMPLES )

# MESSAGE( WARING ${CMAKE_BINARY_DIR})      #工程目录
# MESSAGE( WARING ${CMAKE_SOURCE_DIR})
# MESSAGE( WARING ${PROJECT_BINARY_DIR})    #工程目录
# MESSAGE( WARING ${PROJECT_SOURCE_DIR})
# MESSAGE( WARING ${CMAKE_CURRENT_BINARY_DIR}) #CmakeList.txt的目录
# MESSAGE( WARING ${CMAKE_CURRENT_SOURCE_DIR})
# MESSAGE( WARING ${CMAKE_FILES_DIRECTORY}) #CMakeFiles的相对目录

IF(BUILD_DOCUMENTATION)

    IF(DOT)
        SET(DOXYGEN_HAVE_DOT YES)
    ELSE(DOT)
        SET(DOXYGEN_HAVE_DOT NO)
    ENDIF(DOT)
    
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN NO)
    
    set(DOXYGEN_INLINE_SOURCES YES)
    set(DOXYGEN_SOURCE_BROWSER YES)
    
    # set(DOXYGEN_INPUT_ENCODING GBK)
    set(DOXYGEN_DOXYFILE_ENCODING GBK)

    doxygen_add_docs(
        doxygen
        # ${PROJECT_SOURCE_DIR}
        include
        src
        COMMENT "Generate man pages"
    )

    # CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/doc/Doxyfiles/doxyfile.cmake
        # ${PROJECT_BINARY_DIR}/doc/doxyfile
    # )
    # ADD_CUSTOM_TARGET(DoxygenDoc ${DOXYGEN}
        # ${PROJECT_BINARY_DIR}/doc/doxyfile
    # )
	
ENDIF(BUILD_DOCUMENTATION)

# CONFIGURE_FILE(
  # "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/cmake_uninstall.cmake.in"
  # "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  # IMMEDIATE @ONLY)
# ADD_CUSTOM_TARGET(uninstall
  # "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
