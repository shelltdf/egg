# ARM\Mali Developer Tools\OpenGL ES Emulator v1.3.0
# Find the Mali includes and library
# This module defines

#  MALI_FOUND, If false, do not try to use CEGUI.

#  MALI_INCLUDE_DIR, where to find CEGUI.h, etc.
#  MALI_LIBRARY_DIR, where to find CEGUI.h, etc.

#  MALI_EGL_LIBRARY 
#  MALI_GLES_1_LIBRARY 
#  MALI_GLES_2_LIBRARY 

# $MALI_DIR is an environment variable that would
# correspond to the ./configure --prefix=$MALI_DIR


if( CMAKE_SIZEOF_VOID_P EQUAL 8 )

FIND_PATH( MALI_INCLUDE_DIR EGL/egl.h
    $ENV{MALI_DIR}/Include/
    # "${CMAKE_SOURCE_DIR}/etc/OpenGL ES Emulator v1.3.0/Include/"
    "${CMAKE_SOURCE_DIR}/etc/Mali_OpenGL_ES_Emulator-v3.0.g47c3b-Windows-64bit/Include/"
    ) 
    
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )

FIND_PATH( MALI_INCLUDE_DIR EGL/egl.h
    $ENV{MALI_DIR}/Include/
    # "${CMAKE_SOURCE_DIR}/etc/OpenGL ES Emulator v1.3.0/Include/"
    "${CMAKE_SOURCE_DIR}/etc/Mali_OpenGL_ES_Emulator-v3.0.g47c3b-Windows-32bit/Include/"
    ) 
    
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  

    
FIND_PATH( MALI_LIBRARY_DIR libEGL.lib
    ${MALI_INCLUDE_DIR}/../lib/
    ${MALI_INCLUDE_DIR}/../
    ) 

FIND_LIBRARY( MALI_EGL_LIBRARY        
    NAMES libEGL      
    PATHS ${MALI_LIBRARY_DIR} ) 

FIND_LIBRARY( MALI_GLES_1_LIBRARY        
    NAMES libGLESv1_CM      
    PATHS ${MALI_LIBRARY_DIR} ) 

FIND_LIBRARY( MALI_GLES_2_LIBRARY        
    NAMES libGLESv2      
    PATHS ${MALI_LIBRARY_DIR} ) 
    
FIND_LIBRARY( MALI_EMULATOR_LIBRARY        
    NAMES libMaliEmulator      
    PATHS ${MALI_LIBRARY_DIR} ) 

SET( MALI_FOUND "NO" )
IF(MALI_INCLUDE_DIR)
    SET( MALI_FOUND "YES" )
ENDIF(MALI_INCLUDE_DIR)

