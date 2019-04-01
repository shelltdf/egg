
FIND_PATH(rapidxml_INCLUDE_DIR rapidxml.hpp
    PATHS
    ${CMAKE_SOURCE_DIR}../install/include/
    $ENV{PATH}
    /usr/include/
    /usr/local/include/
)

# FIND_LIBRARY(rapidxml_LIBRARY 
			# NAMES rapidxml
			# PATHS ${rapidxml}/../lib/)

SET(rapidxml_FOUND "NO")
IF(rapidxml_INCLUDE_DIR)
    SET(rapidxml_FOUND "YES")
ENDIF(rapidxml_INCLUDE_DIR)
