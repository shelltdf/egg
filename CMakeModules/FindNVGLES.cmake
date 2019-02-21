# ARM\Mali Developer Tools\OpenGL ES Emulator v1.3.0
# Find the Mali includes and library
# This module defines

#  MALI_FOUND, If false, do not try to use CEGUI.

#  MALI_INCLUDE_DIR, where to find CEGUI.h, etc.
#  MALI_LIBRARY_DIR, where to find CEGUI.h, etc.

#  MALI_EGL_LIBRARY 
#  MALI_GLES_1_LIBRARY 
#  MALI_GLES_2_LIBRARY 

# $NV_WINGL_X86_PLAT is an environment variable that would
# correspond to the ./configure --prefix=$NV_WINGL_X86_PLAT


	message(STATUS "NV_WINGL_X86_PLAT")
	message(STATUS $ENV{NV_WINGL_X86_PLAT})
		

    FIND_PATH( MALI_INCLUDE_DIR EGL/egl.h
		$ENV{NV_WINGL_X86_PLAT}/Include/
		) 
		
	FIND_PATH( MALI_LIBRARY_DIR libGLESv1_CM.lib
		$ENV{NV_WINGL_X86_PLAT}/lib/release/
		) 

		
	# FIND_LIBRARY( MALI_EGL_LIBRARY        
		# NAMES libEGL      
		# PATHS $ENV{NV_WINGL_X86_PLAT}/Lib/ ) 

	FIND_LIBRARY( MALI_GLES_1_LIBRARY        
		NAMES libGLESv1_CM      
		PATHS $ENV{NV_WINGL_X86_PLAT}/Lib/release/ ) 

	FIND_LIBRARY( MALI_GLES_2_LIBRARY        
		NAMES libGLES20      
		PATHS $ENV{NV_WINGL_X86_PLAT}/Lib/release/ ) 


		



SET( MALI_FOUND "NO" )
IF(MALI_INCLUDE_DIR)
    SET( MALI_FOUND "YES" )
ENDIF(MALI_INCLUDE_DIR)

