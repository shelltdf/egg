SET(NIUBI_SETUP_TARGET_NAME player)
include_directories(${PROJECT_SOURCE_DIR}/include)

SET(NIUBI_SETUP_HEADERS
)

SET(NIUBI_SETUP_SOURCES
    
    main.cpp
    
)

NIUBI_SETUP_EXECUTABLE(ON)
target_link_libraries(${NIUBI_SETUP_TARGET_NAME} 
        eggRuntime eggEngine eggApplication

		Window_win32
		Renderer_glcore
		GraphicsContext_wgl
        )
