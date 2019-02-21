SET(NIUBI_SETUP_TARGET_NAME test_app)
include_directories(${PROJECT_SOURCE_DIR}/include)

SET(NIUBI_SETUP_HEADERS

	osgViewer.h
	osgEarth.h

)

SET(NIUBI_SETUP_SOURCES
    
    test_app.cpp
    
)

INCLUDE_DIRECTORIES(AFTER ${OSG_INCLUDE_DIR})

SET(EXT_LIBRARY )
IF(USE_OSG)
add_definitions(-DEGG_USE_OSG)
SET(EXT_LIBRARY ${EXT_LIBRARY}
    debug ${OPENTHREADS_LIBRARY_DEBUG} optimized ${OPENTHREADS_LIBRARY}
    debug ${OSG_LIBRARY_DEBUG} optimized ${OSG_LIBRARY}
    debug ${OSGDB_LIBRARY_DEBUG} optimized ${OSGDB_LIBRARY}
    debug ${OSGUTIL_LIBRARY_DEBUG} optimized ${OSGUTIL_LIBRARY}
    debug ${OSGGA_LIBRARY_DEBUG} optimized ${OSGGA_LIBRARY}
    debug ${OSGVIEWER_LIBRARY_DEBUG} optimized ${OSGVIEWER_LIBRARY}
)
ENDIF(USE_OSG)
IF(USE_OSGEARTH)
add_definitions(-DEGG_USE_OSGEARTH)
SET(EXT_LIBRARY ${EXT_LIBRARY}
    optimized ${OSGEARTH_LIBRARY}               debug ${OSGEARTH_LIBRARY_DEBUG}
    optimized ${OSGEARTHFEATURES_LIBRARY}       debug ${OSGEARTHFEATURES_LIBRARY_DEBUG}
    optimized ${OSGEARTHUTIL_LIBRARY}           debug ${OSGEARTHUTIL_LIBRARY_DEBUG}
    # optimized ${OSGEARTHQT_LIBRARY}             debug ${OSGEARTHQT_LIBRARY_DEBUG}
    optimized ${OSGEARTHSYMBOLOGY_LIBRARY}      debug ${OSGEARTHSYMBOLOGY_LIBRARY_DEBUG}
    optimized ${OSGEARTHANNOTATION_LIBRARY}     debug ${OSGEARTHANNOTATION_LIBRARY_DEBUG}
)
ENDIF(USE_OSGEARTH)


NIUBI_SETUP_EXECUTABLE(ON)
target_link_libraries(${NIUBI_SETUP_TARGET_NAME} 
        eggRuntime eggEngine eggApplication
        ${EXT_LIBRARY}

		Window_win32
		Renderer_glcore
		GraphicsContext_wgl
        )
