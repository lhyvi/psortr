cmake_minimum_required(VERSION 3.21)

project(psortr VERSION 1.0.0 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} src/main.c)


include(FetchContent)
FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG master)
FetchContent_MakeAvailable(raylib)
target_include_directories(${PROJECT_NAME} PRIVATE ${raylib_SOURCE_DIR}/src/extras)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_compile_definitions(raylib PRIVATE
    SUPPORT_FILEFORMAT_BMP=1
    SUPPORT_FILEFORMAT_PNG=1
    SUPPORT_FILEFORMAT_TGA=1
    SUPPORT_FILEFORMAT_JPG=1
    SUPPORT_FILEFORMAT_GIF=1
    SUPPORT_FILEFORMAT_QOI=1
    SUPPORT_FILEFORMAT_PSD=1
    SUPPORT_FILEFORMAT_HDR=1
    SUPPORT_FILEFORMAT_PIC=1
    SUPPORT_FILEFORMAT_PNM=1
    SUPPORT_FILEFORMAT_DDS=1
    SUPPORT_FILEFORMAT_PKM=1
    SUPPORT_FILEFORMAT_KTX=1
    SUPPORT_FILEFORMAT_PVR=1
    SUPPORT_FILEFORMAT_ASTC=1
)
