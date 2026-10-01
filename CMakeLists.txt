cmake_minimum_required(VERSION 3.16)

project(FCamera VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.10)

# Content of downloaded design from Figma to Qt or similar
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/importedcontent/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/importedcontent)
endif()

qt_add_executable(appFCamera
    main.cpp
    src/video/FFmpegVideoDecoder.cpp
    src/video/FFmpegVideoDecoder.h
    src/video/QMLImageProvider.cpp
    src/video/QMLImageProvider.h
)

qt_add_qml_module(appFCamera
    URI FCamera
    QML_FILES
        Main.qml
)

# FFmpeg
set(FFMPEG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../ffmpeg-9.0.2-full_build-shared")

target_include_directories(appFCamera PRIVATE
    "${FFMPEG_ROOT}/include"
)

target_link_directories(appFCamera PRIVATE
    "${FFMPEG_ROOT}/lib"
)

target_link_libraries(appFCamera PRIVATE
    avcodec
    avformat
    avutil
    swscale
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appFCamera PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appFCamera
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appFCamera
    PRIVATE Qt6::Quick
)

install(TARGETS appFCamera
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
