SET(NIUBI_SETUP_TARGET_NAME fbxConvtor)

SET(NIUBI_SETUP_HEADERS
    # ext/bitmap.h
    # ext/tinymatrix.h
    # ext/tinymesh.h
    # ext/tinyGUI.h
    # ext/tinyfont.h
    # ext/TuioInput.h
    
    Utils.h
    Export.h
)

SET(NIUBI_SETUP_SOURCES
    # ${_oscpack} 
    # ${_TUIO}
    # ext/tinyfont.cpp
    # ext/TuioInput.cpp
    
    main.cpp
    
    Utils.cpp
    Export.cpp
    
    ExportMesh.cpp
    ExportProgramAndShader.cpp
    ExportTextureAndImage.cpp
    ExportMaterial.cpp
    ExportSkeleton.cpp
    ExportAnimation.cpp
    ExportCameraAndLight.cpp
    ExportLine.cpp
)

INCLUDE_DIRECTORIES(AFTER ${PROJECT_SOURCE_DIR}/include/ )
INCLUDE_DIRECTORIES(AFTER ${PROJECT_SOURCE_DIR})
INCLUDE_DIRECTORIES(AFTER ${_3RDPARTY_INCLUDE_DIR})
# INCLUDE_DIRECTORIES(AFTER ${PROJECT_SOURCE_DIR}/extlib/TUIO11_CPP/TUIO)
# INCLUDE_DIRECTORIES(AFTER ${PROJECT_SOURCE_DIR}/extlib/TUIO11_CPP/oscpack)

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
IF(MALI_FOUND)
    LINK_DIRECTORIES( ${MALI_LIBRARY_DIR})
ENDIF()

IF(FBX_FOUND)
    INCLUDE_DIRECTORIES(AFTER ${FBX_INCLUDE_DIR})
ENDIF(FBX_FOUND)

SET(EXT_LIB)
IF( FBX_FOUND )
    SET(EXT_LIB ${EXT_LIB} debug ${FBX_LIBRARY_DEBUG} optimized ${FBX_LIBRARY})
ENDIF( FBX_FOUND )


SET(NDK_LIB)
IF( ANDROID )
    SET(NDK_LIB ${NDK_LIB} android log native_app_glue EGL GLESv1_CM )
    SET(NDK_LIB ${NDK_LIB} Launcher_ndk GraphicsContext_egl_ndk Renderer_gles_1_x)
ENDIF( ANDROID )


NIUBI_SETUP_EXECUTABLE(ON)
TARGET_LINK_LIBRARIES( ${NIUBI_SETUP_TARGET_NAME} 
		eggRuntime eggEngine
		${NDK_LIB} ${EXT_LIB})
NIUBI_SETUP_INSTALL()
NIUBI_SETUP_DOCUMENT()

