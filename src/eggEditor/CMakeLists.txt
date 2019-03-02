SET(NIUBI_SETUP_TARGET_NAME eggEditor)
SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${NIUBI_SETUP_TARGET_NAME})
add_definitions(-DEGG_EDITOR_LIBRARY)

SET(NIUBI_SETUP_HEADERS

    #宏定义
    ${HEADER_PATH}/Export.h

    
    #编辑器对象
    ${HEADER_PATH}/Editor.h
    
    ${HEADER_PATH}/View.h
    ${HEADER_PATH}/Controller.h
    ${HEADER_PATH}/Panel.h
    ${HEADER_PATH}/Selection.h
)

SET(NIUBI_SETUP_SOURCES

    Editor.cpp
    
    View.cpp
    Controller.cpp
    Panel.cpp
    Selection.cpp
)

NIUBI_SETUP_LIBRARY(ON)
target_link_libraries(${NIUBI_SETUP_TARGET_NAME} 
        egg eggEngine
        )
NIUBI_SETUP_INSTALL()
NIUBI_SETUP_INSTALL_INCLUDE()
NIUBI_SETUP_DOCUMENT()
