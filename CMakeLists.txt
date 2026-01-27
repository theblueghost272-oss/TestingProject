cmake_minimum_required(VERSION 3.16)
project(AIGameDevSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# MinGW64 path
set(MINGW_PATH "D:/msys64/mingw64")

# GLFW
set(GLFW_INCLUDE_DIR "${MINGW_PATH}/include")
set(GLFW_LIBRARY "${MINGW_PATH}/lib/libglfw3.a")

# CURL
set(CURL_INCLUDE_DIR "${MINGW_PATH}/include")
set(CURL_LIBRARY
    "${MINGW_PATH}/lib/libcurl.a"
    "${MINGW_PATH}/lib/libcurl.dll.a"
)

# ImGui
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/imgui")
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_executable(AIGameDevSimulator
    GameProject/Main.cpp
    ${IMGUI_SOURCES}
)

target_include_directories(AIGameDevSimulator PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${GLFW_INCLUDE_DIR}
    ${CURL_INCLUDE_DIR}
)

target_link_libraries(AIGameDevSimulator PRIVATE
    opengl32
    ${GLFW_LIBRARY}
    ${CURL_LIBRARY}
    ws2_32
)

set_target_properties(AIGameDevSimulator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)