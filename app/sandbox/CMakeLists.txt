SET(NIUBI_SETUP_TARGET_NAME sandbox)
include_directories(${PROJECT_SOURCE_DIR}/include)

SET(NIUBI_SETUP_HEADERS
)

SET(NIUBI_SETUP_SOURCES
    
    sandbox.cpp
    
)

NIUBI_SETUP_EXECUTABLE(ON)
target_link_libraries(${NIUBI_SETUP_TARGET_NAME} 
        eggRuntime eggEngine eggEditor
        )
