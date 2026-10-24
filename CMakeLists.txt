cmake_minimum_required(VERSION 3.31)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
project(wol)

# Set options based on OS
# Linux
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_options(-Wall -Os -faddrsig -flto -fdata-sections -ffunction-sections -std=c++11)
    add_link_options(-fuse-ld=lld -faddrsig -flto -fdata-sections -ffunction-sections -Wl,--icf=safe -Wl,--gc-sections)
endif()
# macOS
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_compile_options(-Wall -Os -faddrsig -flto -fdata-sections -ffunction-sections -std=c++11)
    add_link_options(-faddrsig -flto -fdata-sections -ffunction-sections)
endif()
# Windows
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_options(-Wall -Os -faddrsig -flto -fdata-sections -ffunction-sections -finput-charset=UTF-8 -std=c++11)
    add_link_options(-fuse-ld=lld -faddrsig -flto -fdata-sections -ffunction-sections -lws2_32 -Wl,--icf=safe -Wl,--gc-sections)
endif()

add_executable(${PROJECT_NAME} "src/wol.cpp")
install(TARGETS ${PROJECT_NAME})
