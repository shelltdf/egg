SET(TEST_NAME test_eggengine)
include_directories(${PROJECT_SOURCE_DIR}/include)

SET(TEST_SOURCES
    
	test_vfs.h
	test_asset.h

    main.cpp
    
)

NIUBI_SETUP_TEST(ON)
target_link_libraries(${TEST_NAME} 
        eggRuntime eggEngine
        )
NIUBI_SETUP_INSTALL()

