# dx9
# Find the native libusb-win32 includes and library
# This module defines

#  DX9_FOUND, If false, do not try to use CEGUI.

#  DX9_INCLUDE_DIR, where to find CEGUI.h, etc.

#  DX9_LIBRARY 

# $DXSDK_DIR is an environment variable that would
# correspond to the ./configure --prefix=$DXSDK_DIR



    FIND_PATH( DX9_INCLUDE_DIR d3d9.h
		$ENV{DXSDK_DIR}/Include
		) 

		
    FIND_LIBRARY( DX9_LIBRARY        
	    NAMES d3d9      
	    PATHS $ENV{DXSDK_DIR}/Lib/x86 ) 




SET( DX9_FOUND "NO" )
IF(DX9_INCLUDE_DIR)
    SET( DX9_FOUND "YES" )
ENDIF(DX9_INCLUDE_DIR)

