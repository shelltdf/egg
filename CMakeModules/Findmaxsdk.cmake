# - Find OIS library
# Find the native OIS includes and library
# This module defines

#  MAXSDK_FOUND, If false, do not try to use OIS.

#  MAXSDK_INCLUDE_DIR, where to find OIS.h, etc.
#  MAXSDK_LIBRARY_ALL

# $ADSK_3DSMAX_x32_2012 is an environment variable that would
# correspond to the ./configure --prefix=$ADSK_3DSMAX_x32_2012



FIND_PATH( MAXSDK_INCLUDE_DIR max.h 
    $ENV{ADSK_3DSMAX_x32_2012}/maxsdk/include
    DOC "The directory where OIS.h resides" 
    ) 
	
    
    
SET( MAXSDK_LIBRARY_ALL )
SET( MAXSDK_LIBRARY_NAMES
    acap amodeler assetmanagement atscore
    biped bmm
    composite core crowd custdlg 
    DataBridge
    edmodel expr
    flt 
    geom geomimp gfx GraphicsDriver gup
    igame imageviewers
    manipsys maxnet maxscrpt maxutil menuman menus mesh mnmath
    paramblk2 particle ParticleFlow physique poly PolyPaint 
    renderutil rtmax  
    springsys
    tessint 
    viewfile
    zlibdll
)
      
      
FOREACH( LIBRARY_NAME ${MAXSDK_LIBRARY_NAMES})
    FIND_LIBRARY( MAXSDK_${LIBRARY_NAME}_LIBRARY 
        NAMES ${LIBRARY_NAME} 
        PATHS 
        ${MAXSDK_INCLUDE_DIR}/../lib 
    ) 
    list(APPEND MAXSDK_LIBRARY_ALL  ${MAXSDK_${LIBRARY_NAME}_LIBRARY} )
ENDFOREACH()
    
    

	  



SET( MAXSDK_FOUND "NO" )
IF(MAXSDK_INCLUDE_DIR)
    SET( MAXSDK_FOUND "YES" )
ENDIF(MAXSDK_INCLUDE_DIR)

