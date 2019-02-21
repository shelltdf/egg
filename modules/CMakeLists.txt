
# ADD_SUBDIRECTORY(Platform_win32) # vc mingw cygwin

if(WIN32)
ADD_SUBDIRECTORY(Window_win32) # windows api
ADD_SUBDIRECTORY(GraphicsContext_wgl) # windows wgl
ADD_SUBDIRECTORY(Renderer_glcore) # opengl core profile
endif(WIN32)

if(EMSCRIPTEN)

endif(EMSCRIPTEN)


ADD_SUBDIRECTORY(Application_test) # test app

if(USE_QT5)
ADD_SUBDIRECTORY(Editor_qt) # test app
endif(USE_QT5)
