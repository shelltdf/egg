SET(TEST_NAME test_eggruntime)
include_directories(${PROJECT_SOURCE_DIR}/include)

SET(TEST_SOURCES
    
	test_datatype.h
    test_smart_pointer.h
    test_path.h
    test_serializer.h
	test_module.h
	test_node.h
    test_listender.h

    main.cpp
    
)

NIUBI_SETUP_TEST(ON)
target_link_libraries(${TEST_NAME} 
        egg
        )
NIUBI_SETUP_INSTALL()

