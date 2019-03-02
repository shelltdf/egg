SET(NIUBI_SETUP_TARGET_NAME Application_test)
#add_definitions(-DEGG_RUNTIME_LIBRARY)

SET(NIUBI_SETUP_HEADERS

)
SET(NIUBI_SETUP_SOURCES
    
    Application_test.cpp
)

NIUBI_SETUP_PLUGINS(ON)
target_link_libraries(${NIUBI_SETUP_TARGET_NAME} 
        egg eggEngine eggApplication
        )
NIUBI_SETUP_INSTALL()
NIUBI_SETUP_INSTALL_INCLUDE()
NIUBI_SETUP_DOCUMENT()
