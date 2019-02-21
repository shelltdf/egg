SET(NIUBI_SETUP_TARGET_NAME test_eigen)
include_directories(${PROJECT_SOURCE_DIR}/include)

SET(NIUBI_SETUP_HEADERS
)

SET(NIUBI_SETUP_SOURCES
    
    test_eigen.cpp
    
)

NIUBI_SETUP_EXECUTABLE(ON)
# target_link_libraries(${NIUBI_SETUP_TARGET_NAME} 
        # eggRuntime eggEngine eggApplication
        # )
