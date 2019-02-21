
FIND_PATH(RQSDK_INCLUDE_DIR rqsdk.h
    PATHS
    ${CMAKE_SOURCE_DIR}../install/include/
    $ENV{PATH}
    /usr/include/
    /usr/local/include/
)

FIND_LIBRARY(RQSDK_RedQueenStarter_LIBRARY 
			NAMES RedQueenStarter
			PATHS ${RQSDK_INCLUDE_DIR}/../lib/)

SET(RQSDK_FOUND "NO")
IF(RQSDK_INCLUDE_DIR AND RQSDK_RedQueenStarter_LIBRARY)
    SET(RQSDK_FOUND "YES")
ENDIF(RQSDK_INCLUDE_DIR AND RQSDK_RedQueenStarter_LIBRARY)
