SET(NIUBI_SETUP_TARGET_NAME egg)
SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${NIUBI_SETUP_TARGET_NAME})
include_directories(${PROJECT_SOURCE_DIR}/include/)
add_definitions(-DEGG_RUNTIME_LIBRARY)

#设计模式
set( Design_Pattern_HEADERS
    ${HEADER_PATH}/Singleton.h
    ${HEADER_PATH}/Factory.h
)
set( Design_Pattern_SOURCE
    Factory.cpp
)
SOURCE_GROUP( "_Design_Pattern" FILES ${Design_Pattern_HEADERS} )
SOURCE_GROUP( "_Design_Pattern" FILES ${Design_Pattern_SOURCE} )

#基本工具
set( Util_HEADERS
    ${HEADER_PATH}/Log.h
    ${HEADER_PATH}/SystemArg.h
    ${HEADER_PATH}/Timer.h
    ${HEADER_PATH}/StringUtil.h
    ${HEADER_PATH}/ThreadPool.h
    ${HEADER_PATH}/Path.h
	${HEADER_PATH}/Math.h
	${HEADER_PATH}/FileDirUtil.h
	${HEADER_PATH}/Codes.h
)
set( Util_SOURCE
    Timer.cpp
    StringUtil.cpp
    Path.cpp
	FileDirUtil.cpp
    Codes.cpp
)
SOURCE_GROUP( "_Util" FILES ${Util_HEADERS} )
SOURCE_GROUP( "_Util" FILES ${Util_SOURCE} )

#内存管理
set( Memory_HEADERS
    ${HEADER_PATH}/ref_ptr.h
    ${HEADER_PATH}/observer_ptr.h
    ${HEADER_PATH}/Referenced.h
)
SOURCE_GROUP( "_Memory" FILES ${Memory_HEADERS} )

#序列化
set( Serializer_HEADERS
    ${HEADER_PATH}/Serializer.h
)
SOURCE_GROUP( "_Serializer" FILES ${Serializer_HEADERS} )

#object基础类
set( Object_HEADERS
    ${HEADER_PATH}/Object.h
)
set( Object_SOURCE
    Object.cpp
)
SOURCE_GROUP( "_Object" FILES ${Object_HEADERS} )
SOURCE_GROUP( "_Object" FILES ${Object_SOURCE} )

#block基础类
set( Block_HEADERS
    ${HEADER_PATH}/Block.h
    ${HEADER_PATH}/BlockScript.h
)
set( Block_SOURCE
    Block.cpp
    BlockScript.cpp
)
SOURCE_GROUP( "_Block" FILES ${Block_HEADERS} )
SOURCE_GROUP( "_Block" FILES ${Block_SOURCE} )

#Event基础类
set( Event_HEADERS
    ${HEADER_PATH}/Event.h
    ${HEADER_PATH}/EventQueue.h
)
set( Event_SOURCE
    Event.cpp
    EventQueue.cpp
)
SOURCE_GROUP( "_Event" FILES ${Event_HEADERS} )
SOURCE_GROUP( "_Event" FILES ${Event_SOURCE} )


#node支持
set( Node_HEADERS
    ${HEADER_PATH}/Node.h
    ${HEADER_PATH}/NodePath.h
    ${HEADER_PATH}/NodeVisitor.h
)
set( Node_SOURCE
    Node.cpp
)
SOURCE_GROUP( "_Node" FILES ${Node_HEADERS} )
SOURCE_GROUP( "_Node" FILES ${Node_SOURCE} )

#反射
set( Reflection_HEADERS
    ${HEADER_PATH}/Reflection/ObjectHandle.h
    ${HEADER_PATH}/Reflection/ValueHandle.h
    ${HEADER_PATH}/Reflection/FunctionHandle.h
)
set( Reflection_SOURCE    
    Reflection/ObjectHandle.cpp
    Reflection/ValueHandle.cpp
    Reflection/FunctionHandle.cpp
)
SOURCE_GROUP( "_Reflection" FILES ${Reflection_HEADERS} )
SOURCE_GROUP( "_Reflection" FILES ${Reflection_SOURCE} )

#解释执行命令行
set( Command_HEADERS
    ${HEADER_PATH}/Command/Command.h
    ${HEADER_PATH}/Command/Console.h
    ${HEADER_PATH}/Command/Context.h
    ${HEADER_PATH}/Command/ResultStack.h
    ${HEADER_PATH}/Command/History.h
    ${HEADER_PATH}/Command/Parser.h
    ${HEADER_PATH}/Command/cmd/CMDsystem.h
    ${HEADER_PATH}/Command/cmd/CMDobject.h
    ${HEADER_PATH}/Command/cmd/CMDattribute.h
)
set( Command_SOURCE 
    Command/Command.cpp
    Command/Console.cpp
    Command/Context.cpp
    Command/ResultStack.cpp
    Command/History.cpp
    Command/Parser.cpp
    Command/cmd/CMDsystem.cpp
    Command/cmd/CMDobject.cpp
    Command/cmd/CMDattribute.cpp
)
SOURCE_GROUP( "_Command" FILES ${Command_HEADERS} )
SOURCE_GROUP( "_Command" FILES ${Command_SOURCE} )



SET(NIUBI_SETUP_HEADERS

    ${Design_Pattern_HEADERS}
    ${Util_HEADERS}
    ${Memory_HEADERS}
    ${Serializer_HEADERS}
    ${Object_HEADERS}
    ${Block_HEADERS}
    ${Event_HEADERS}
    ${Node_HEADERS}

    
    
    #宏定义
    ${HEADER_PATH}/Export.h
    ${HEADER_PATH}/DataType.h
    
    #对象继承类
    ${HEADER_PATH}/Runtime.h
    ${HEADER_PATH}/Module.h
    ${HEADER_PATH}/Plugins.h
    ${HEADER_PATH}/Platform.h
    
    #其他功能
    ${HEADER_PATH}/Coord.h
    ${HEADER_PATH}/Media.h
)

SET(NIUBI_SETUP_SOURCES
    
    ${Design_Pattern_SOURCE}
    ${Util_SOURCE}
    # ${Memory_SOURCE}
    # ${Serializer_SOURCE} 
    ${Object_SOURCE}
    ${Event_SOURCE}
    ${Block_SOURCE}
    ${Node_SOURCE}
    ${Reflection_SOURCE}
    ${Command_SOURCE}
    
    ${Reflection_HEADERS}
    ${Command_HEADERS}
    
    #对象继承类
    Runtime.cpp
    Module.cpp
    Plugins.cpp
    Platform.cpp
    
    #其他功能
    Coord.cpp
    Media.cpp
)

NIUBI_SETUP_LIBRARY(ON)
NIUBI_SETUP_INSTALL()
NIUBI_SETUP_INSTALL_INCLUDE()
NIUBI_SETUP_DOCUMENT()

INSTALL(
		FILES       ${Reflection_HEADERS}
		DESTINATION ${INSTALL_INCDIR}/${NIUBI_SETUP_TARGET_NAME}/Reflection
	)    
    
INSTALL(
		FILES       ${Command_HEADERS}
		DESTINATION ${INSTALL_INCDIR}/${NIUBI_SETUP_TARGET_NAME}/Command
	)    
    
    