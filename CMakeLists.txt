cmake_minimum_required(VERSION 3.10)
project(AutoHeuristic LANGUAGES C CXX)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

source_group("Scripts" FILES ${PROJECT_SOURCE_DIR}/scripts/hello-world.pl)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    third_party/imgui
    third_party/imgui/backends
    third_party/implot
)

set (SOURCES
    main.cpp
    src/file_utils.cpp
    src/mask_utils.cpp
    src/gui.cpp
    src/gui_platform/gui_platform.cpp
    src/gui_histogram/gui_histogram.cpp
)

file(GLOB IMGUI_SOURCES
    "third_party/imgui/*.cpp"
    "third_party/imgui/backends/*.cpp"
)

file(GLOB IMPLOT_SOURCES
    "third_party/implot/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES} ${IMPLOT_SOURCES}) 

target_link_libraries(${PROJECT_NAME} PRIVATE d3d11 d3dcompiler dxguid dwmapi)
