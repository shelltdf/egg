
FIND_PATH(imgui_INCLUDE_DIR imgui.h
    PATHS
    ${CMAKE_SOURCE_DIR}../install/include/
    $ENV{PATH}
    /usr/include/
    /usr/local/include/
)

FIND_LIBRARY(imgui_LIBRARY 
			NAMES imgui
			PATHS ${imgui_INCLUDE_DIR}/../lib/)
            
FIND_LIBRARY(imgui_LIBRARY_DEBUG
			NAMES imguid
			PATHS ${imgui_INCLUDE_DIR}/../lib/)
            
SET(imgui_FOUND "NO")
IF(imgui_INCLUDE_DIR)
    SET(imgui_FOUND "YES")
ENDIF(imgui_INCLUDE_DIR)
