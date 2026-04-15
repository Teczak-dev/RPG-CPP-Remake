cmake_minimum_required(VERSION 3.10)
project("RPG CPP Remake" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

set(PROJECT_TARGET rpg_cpp_remake)

set(APP_ICON_MACOS "${CMAKE_SOURCE_DIR}/assets/icons/app.icns" CACHE FILEPATH "Path to macOS .icns app icon")
set(APP_ICON_WINDOWS "${CMAKE_SOURCE_DIR}/assets/icons/app.ico" CACHE FILEPATH "Path to Windows .ico app icon")
set(APP_BUNDLE_IDENTIFIER "com.rpgcpp.remake" CACHE STRING "macOS bundle identifier")

if(APPLE)
    add_executable(${PROJECT_TARGET} MACOSX_BUNDLE src/main.cpp)
elseif(WIN32)
    add_executable(${PROJECT_TARGET} WIN32 src/main.cpp)
else()
    add_executable(${PROJECT_TARGET} src/main.cpp)
endif()

if(APPLE)
    if(EXISTS "${APP_ICON_MACOS}")
        get_filename_component(MACOS_ICON_FILE "${APP_ICON_MACOS}" NAME)
        target_sources(${PROJECT_TARGET} PRIVATE "${APP_ICON_MACOS}")
        set_source_files_properties("${APP_ICON_MACOS}" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
        set_target_properties(${PROJECT_TARGET} PROPERTIES MACOSX_BUNDLE_ICON_FILE "${MACOS_ICON_FILE}")
    endif()

    set_target_properties(${PROJECT_TARGET} PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "RPG CPP Remake"
        MACOSX_BUNDLE_GUI_IDENTIFIER "${APP_BUNDLE_IDENTIFIER}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
        MACOSX_BUNDLE_BUNDLE_VERSION "1"
    )
endif()

if(WIN32 AND EXISTS "${APP_ICON_WINDOWS}")
    string(REPLACE "\\" "/" APP_ICON_WINDOWS_RC "${APP_ICON_WINDOWS}")
    configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/windows_app_icon.rc.in"
        "${CMAKE_BINARY_DIR}/windows_app_icon.rc"
        @ONLY
    )
    target_sources(${PROJECT_TARGET} PRIVATE "${CMAKE_BINARY_DIR}/windows_app_icon.rc")
endif()

if(APPLE)
    target_link_libraries(${PROJECT_TARGET} raylib "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
else()
    target_link_libraries(${PROJECT_TARGET} raylib)
endif()
