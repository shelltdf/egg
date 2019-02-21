# physfs-2.0.0
# Find the native TUIO_CPP includes and library
# This module defines

#  PHYSFS_FOUND, If false, do not try to use OIS.

#  PHYSFS_INCLUDE_DIR, where to find OIS.h, etc.
#  PHYSFS_STATIC_LIBRARY , PHYSFS_STATIC_LIBRARY_DEBUG

# $PHYSFS_DIR is an environment variable that would
# correspond to the ./configure --prefix=$PHYSFS_DIR



FIND_PATH( PHYSFS_INCLUDE_DIR physfs.h
	$ENV{PHYSFS_DIR}/
	${CMAKE_SOURCE_DIR}/3rdparty/install/include
	) 
	  

	  
FIND_LIBRARY( PHYSFS_STATIC_LIBRARY 
    NAMES physfs-static 
    PATHS 
    $ENV{PHYSFS_DIR}/release/
	${CMAKE_SOURCE_DIR}/3rdparty/install/lib
    ) 
	
FIND_LIBRARY( PHYSFS_STATIC_LIBRARY_DEBUG 
    NAMES physfs-staticd
    PATHS 
    $ENV{PHYSFS_DIR}/debug/
	${CMAKE_SOURCE_DIR}/3rdparty/install/lib
    ) 
	
	
SET( PHYSFS_FOUND "NO" )
IF(PHYSFS_INCLUDE_DIR)
    SET( PHYSFS_FOUND "YES" )
ENDIF(PHYSFS_INCLUDE_DIR)
