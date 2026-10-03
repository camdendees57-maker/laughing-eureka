cmake_minimum_required(VERSION 3.22.1)
project(vrimgui LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(vrimgui SHARED
    src/vrimgui.cpp
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    backends/imgui_impl_android.cpp
    backends/imgui_impl_opengl3.cpp
)

target_include_directories(vrimgui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

target_compile_definitions(vrimgui PRIVATE
    IMGUI_IMPL_OPENGL_ES3
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS
)

find_library(log-lib log)
find_library(android-lib android)
find_library(gles-lib GLESv3)

target_link_libraries(vrimgui
    ${log-lib}
    ${android-lib}
    ${gles-lib}
)
