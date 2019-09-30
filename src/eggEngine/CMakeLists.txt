SET(NIUBI_SETUP_TARGET_NAME eggEngine)
SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${NIUBI_SETUP_TARGET_NAME})
add_definitions(-DEGG_ENGINE_LIBRARY)

include_directories( ${imgui_INCLUDE_DIR} )

SET(NIUBI_ASSET_HEADERS

    ${HEADER_PATH}/Geometry.h
    ${HEADER_PATH}/Image.h
    ${HEADER_PATH}/Pass.h
    ${HEADER_PATH}/Material.h
    ${HEADER_PATH}/Program.h #不是asset 只能从xml读取
    ${HEADER_PATH}/ProgramDefine.h
    ${HEADER_PATH}/Shader.h
    ${HEADER_PATH}/Texture.h
    ${HEADER_PATH}/Animation.h
    ${HEADER_PATH}/Scene.h
)
SET(NIUBI_ASSET_SOURCES    
    Geometry.cpp
    Image.cpp
    Pass.cpp
    Material.cpp
    Program.cpp
    Shader.cpp
    Texture.cpp
    Animation.cpp
    Scene.cpp
)
SOURCE_GROUP("_ASSET" FILES ${NIUBI_ASSET_HEADERS})
SOURCE_GROUP("_ASSET" FILES ${NIUBI_ASSET_SOURCES})


SET(NIUBI_COMPONENT_HEADERS
    
    ${HEADER_PATH}/Components/ActorControler.h
    ${HEADER_PATH}/Components/MeshRender.h
    ${HEADER_PATH}/Components/SkeletonActorControler.h
    
    ${HEADER_PATH}/Components/Camera.h
    ${HEADER_PATH}/Components/IconMesh.h
    ${HEADER_PATH}/Components/Joint3D.h
    ${HEADER_PATH}/Components/Light.h
    ${HEADER_PATH}/Components/Transfrom3D.h
    
    ${HEADER_PATH}/Components/GUI.h
)
SET(NIUBI_COMPONENT_SOURCES    
    
    Components/ActorControler.cpp
    Components/MeshRender.cpp
    Components/SkeletonActorControler.cpp
    
    Components/Camera.cpp
    Components/IconMesh.cpp
    Components/Joint3D.cpp
    Components/Light.cpp
    Components/Transfrom3D.cpp
    
    Components/GUI.cpp
    
)
SOURCE_GROUP("_COMPONENT" FILES ${NIUBI_COMPONENT_HEADERS})
SOURCE_GROUP("_COMPONENT" FILES ${NIUBI_COMPONENT_SOURCES})

SET(NIUBI_VFS_HEADERS
    ${HEADER_PATH}/VirtualFileSystem.h
)

SET(NIUBI_VFS_SOURCES

    VirtualFileSystem.cpp
    VirtualFileSystem/NativeArchive.h
    VirtualFileSystem/NativeArchive.cpp
    VirtualFileSystem/ZipArchive.h
    VirtualFileSystem/ZipArchive.cpp
    # VirtualFileSystem/SZPArchive.cpp
)
IF(ANDROID)
    SET(NIUBI_VFS_SOURCES ${NIUBI_VFS_SOURCES} 
        VirtualFileSystem/AndroidAssetArchive.h
        VirtualFileSystem/AndroidAssetArchive.cpp
        )
ENDIF(ANDROID)
SOURCE_GROUP("_VFS" FILES ${NIUBI_VFS_HEADERS})
SOURCE_GROUP("_VFS" FILES ${NIUBI_VFS_SOURCES})


SET(NIUBI_RENDERER_HEADERS
    #窗口系统 window
    ${HEADER_PATH}/Window.h
    
    #渲染环境和渲染接口 renderer context
	${HEADER_PATH}/RenderState.h
    ${HEADER_PATH}/Renderer.h
    ${HEADER_PATH}/GraphicsContext.h
)
SET(NIUBI_RENDERER_SOURCES    

    Window.cpp
    RenderState.cpp
    Renderer.cpp
    GraphicsContext.cpp
)
SOURCE_GROUP("_RENDERER" FILES ${NIUBI_RENDERER_HEADERS})
SOURCE_GROUP("_RENDERER" FILES ${NIUBI_RENDERER_SOURCES})


SET(NIUBI_RENDERING_HEADERS

    ${HEADER_PATH}/Composer.h
    ${HEADER_PATH}/RenderNode.h
    ${HEADER_PATH}/RenderPass.h
    ${HEADER_PATH}/RenderWindow.h
    ${HEADER_PATH}/RenderVisitor.h
    ${HEADER_PATH}/RenderUpdater.h
    ${HEADER_PATH}/RenderCullMode.h
)
SET(NIUBI_RENDERING_SOURCES
    
    Composer.cpp
    RenderNode.cpp
    RenderPass.cpp
    RenderWindow.cpp
    RenderVisitor.cpp
    RenderUpdater.cpp
    RenderCullMode.cpp
)
SOURCE_GROUP( "_RENDERING" FILES ${NIUBI_RENDERING_HEADERS})
SOURCE_GROUP( "_RENDERING" FILES ${NIUBI_RENDERING_SOURCES})

SET(NIUBI_3rdParty_SOURCES

    ${HEADER_PATH}/Matrix.h
    ${HEADER_PATH}/XML.h
    ${HEADER_PATH}/ImageIO.h
    
    ${HEADER_PATH}/IMGUI.h
    # ${PROJECT_SOURCE_DIR}/3rdParty/imgui-master/imgui.cpp
    # ${PROJECT_SOURCE_DIR}/3rdParty/imgui-master/imgui_draw.cpp
    # ${PROJECT_SOURCE_DIR}/3rdParty/imgui-master/imgui_demo.cpp
)
SOURCE_GROUP(
    "_3rdParty"
    FILES ${NIUBI_3rdParty_SOURCES}
)

SET(NIUBI_SETUP_HEADERS

    ${NIUBI_ASSET_HEADERS}
    ${NIUBI_RENDERER_HEADERS}
    ${NIUBI_RENDERING_HEADERS}
    
    ${NIUBI_VFS_HEADERS}
    ${NIUBI_COMPONENT_HEADERS}
    
    #引擎对象
    ${HEADER_PATH}/DefaultSetting.h
    ${HEADER_PATH}/Engine.h
    
    #简单数据定义
    ${HEADER_PATH}/DataDefine.h
    
    #宏定义
    ${HEADER_PATH}/Export.h
    
    #第三方引用
    ${NIUBI_3rdParty_SOURCES}
    
    #资源管理 resource
    ${HEADER_PATH}/Resource.h
    ${HEADER_PATH}/ResourceManager.h
    
    #场景图 scene
    ${HEADER_PATH}/SceneNode.h
	${HEADER_PATH}/Component.h
    
    #渲染关系图 composer
    # ${HEADER_PATH}/View.h
    
    #实体层 entity
    # ${HEADER_PATH}/Entity.h
    
    ${HEADER_PATH}/ImportExport.h
    ${HEADER_PATH}/SaveLoad.h
    
    ${HEADER_PATH}/EntityManager.h
    ${HEADER_PATH}/Level.h
    
    ${HEADER_PATH}/InputVisitor.h
)


SET(NIUBI_SETUP_SOURCES

    #虚拟文件系统 VFS
    ${NIUBI_VFS_SOURCES}
    
    # components
    ${NIUBI_COMPONENT_SOURCES}
    
    # assets
    ${NIUBI_ASSET_SOURCES}
    
    # renderer 
    ${NIUBI_RENDERER_SOURCES}
    
    #
    ${NIUBI_RENDERING_SOURCES}


    Resource.cpp
    ResourceManager.cpp
    
    SceneNode.cpp
	Component.cpp

    # View.cpp
    
    DefaultSetting.cpp
    Engine.cpp
    
    ImportExport.cpp
    SaveLoad.cpp
    
    EntityManager.cpp
    Level.cpp
    
    InputVisitor.cpp
)




NIUBI_SETUP_LIBRARY(ON)
target_link_libraries(${NIUBI_SETUP_TARGET_NAME} 
        egg
		#${imgui_LIBRARY} 
		optimized ${imgui_LIBRARY}  debug ${imgui_LIBRARY_DEBUG}
        )
NIUBI_SETUP_INSTALL()
NIUBI_SETUP_INSTALL_INCLUDE()
NIUBI_SETUP_DOCUMENT()


INSTALL(
		FILES       ${NIUBI_COMPONENT_HEADERS}
		DESTINATION ${INSTALL_INCDIR}/${NIUBI_SETUP_TARGET_NAME}/Components
	)
INSTALL(
		FILES       ${NIUBI_VFS_HEADERS}
		DESTINATION ${INSTALL_INCDIR}/${NIUBI_SETUP_TARGET_NAME}/Components
	)    
    
    
