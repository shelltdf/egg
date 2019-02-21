SET(NIUBI_SETUP_TARGET_NAME eggNet)
SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${NIUBI_SETUP_TARGET_NAME})
include_directories(${PROJECT_SOURCE_DIR}/include/)
# add_definitions(-DEGG_NET_LIBRARY)


SET(NIUBI_SETUP_HEADERS

    ${HEADER_PATH}/Export.h
    
    ${HEADER_PATH}/Downloader.h
    ${HEADER_PATH}/DownloadTask.h
    ${HEADER_PATH}/NetworkManager.h
    ${HEADER_PATH}/Processor.h
    
)

SET(NIUBI_SETUP_SOURCES

    Downloader.cpp
    DownloadTask.cpp
    NetworkManager.cpp
    Processor.cpp
)

NIUBI_SETUP_LIBRARY(ON)
NIUBI_SETUP_INSTALL()
NIUBI_SETUP_INSTALL_INCLUDE()
NIUBI_SETUP_DOCUMENT()
