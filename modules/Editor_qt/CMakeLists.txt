SET(NIUBI_SETUP_TARGET_NAME Editor_qt)
#add_definitions(-DEGG_RUNTIME_LIBRARY)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies (QtGui and QtCore).
# find_package(Qt5Widgets REQUIRED)
find_package(Qt5 COMPONENTS 
    Widgets
    Multimedia
    # Declarative 
    LinguistTools #for ts
    )

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Multimedia_INCLUDE_DIRS}
    )

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(
    ${Qt5Widgets_DEFINITIONS}
    ${Qt5Multimedia_DEFINITIONS}
    )
    
qt5_wrap_ui(ui_FILES 
    # ui/appwindows.ui
    )  
    
qt5_add_resources(QRCS 
    # res/style.qrc
    # res/qtpropertybrowser.qrc
    )
    
qt5_create_translation(qm_files 
    ${ui_FILES} ${QRCS} 
    # translations/myobject_en.ts 
    # translations/myobject_cn_zh.ts
    )
    
SET(NIUBI_SETUP_HEADERS

)
SET(NIUBI_SETUP_SOURCES
    
    ${ui_FILES}
    ${QRCS}
    ${qm_files}
    
    Editor_qt.cpp
)

NIUBI_SETUP_PLUGINS(ON)
target_link_libraries(${NIUBI_SETUP_TARGET_NAME} 
        eggRuntime eggEngine eggEditor
        ${Qt5Widgets_LIBRARIES} 
        )
NIUBI_SETUP_INSTALL()
NIUBI_SETUP_INSTALL_INCLUDE()
NIUBI_SETUP_DOCUMENT()
