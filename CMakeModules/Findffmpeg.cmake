# ffmpeg 1.2.3
# Find the native libusb-win32 includes and library
# This module defines

#  DX9_FOUND, If false, do not try to use CEGUI.

#  DX9_INCLUDE_DIR, where to find CEGUI.h, etc.

#  DX9_LIBRARY 

# $FFMPEG_DIR is an environment variable that would
# correspond to the ./configure --prefix=$FFMPEG_DIR



FIND_PATH( FFMPEG_INCLUDE_DIR libavformat/avformat.h
    ${CMAKE_SOURCE_DIR}/extlib/include
	$ENV{FFMPEG_DIR}
	) 

IF( MSVC OR MINGW )    
    SET( FF_LIBRARY_DIR ${FFMPEG_INCLUDE_DIR}/../bin )
ENDIF( MSVC OR MINGW )

IF( UNIX )    
    SET( FF_LIBRARY_DIR ${FFMPEG_INCLUDE_DIR}/../lib )
ENDIF( UNIX )

FIND_LIBRARY( FFMPEG_avformat_LIBRARY	NAMES avformat	PATHS   ${FF_LIBRARY_DIR}) 
FIND_LIBRARY( FFMPEG_avcodec_LIBRARY	NAMES avcodec	PATHS   ${FF_LIBRARY_DIR}) 
FIND_LIBRARY( FFMPEG_avdevice_LIBRARY	NAMES avdevice	PATHS   ${FF_LIBRARY_DIR}) 
FIND_LIBRARY( FFMPEG_avfilter_LIBRARY	NAMES avfilter	PATHS   ${FF_LIBRARY_DIR}) 
FIND_LIBRARY( FFMPEG_avutil_LIBRARY		NAMES avutil	PATHS   ${FF_LIBRARY_DIR}) 
FIND_LIBRARY( FFMPEG_swresample_LIBRARY	NAMES swresample PATHS  ${FF_LIBRARY_DIR}) 
FIND_LIBRARY( FFMPEG_swscale_LIBRARY	NAMES swscale	PATHS   ${FF_LIBRARY_DIR}) 


SET( FFMPEG_LIBRARY	
	${FFMPEG_avformat_LIBRARY} ${FFMPEG_avcodec_LIBRARY} ${FFMPEG_avdevice_LIBRARY}
	${FFMPEG_avfilter_LIBRARY} ${FFMPEG_avutil_LIBRARY} ${FFMPEG_swresample_LIBRARY}
	${FFMPEG_swscale_LIBRARY} ) 


SET( FFMPEG_FOUND "NO" )
IF(FFMPEG_INCLUDE_DIR)
	SET( FFMPEG_FOUND "YES" )
ENDIF(FFMPEG_INCLUDE_DIR)

