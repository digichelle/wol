cmake_minimum_required(VERSION 3.31)
project(wol)

# Set options based on OS
# macOS
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "OS: macOS")
    add_compile_options(-Wall -Os -faddrsig -flto -fdata-sections -ffunction-sections -std=c++20)
    add_link_options(-faddrsig -flto -fdata-sections -ffunction-sections)
# Windows
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "OS: Windows")
    add_compile_options(-Wall -Os -faddrsig -flto -fdata-sections -ffunction-sections -finput-charset=UTF-8 -std=c++20)
    add_link_options(-fuse-ld=lld -faddrsig -flto -fdata-sections -ffunction-sections -lws2_32 -Wl,--icf=safe -Wl,--gc-sections)
# UNIX/UNIX-like operating systems (BSD, Linux, et al; not macOS)
else()
    message(STATUS "OS: UNIX/UNIX-like OS (BSD, Linux, et al; not macOS)")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-Wall -Os -faddrsig -flto -fdata-sections -ffunction-sections -std=c++20)
        add_link_options(-fuse-ld=lld -faddrsig -flto -fdata-sections -ffunction-sections -Wl,--icf=safe -Wl,--gc-sections)
    endif()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-Wall -Os -flto -fdata-sections -ffunction-sections -std=c++20)
        add_link_options(-fuse-ld=mold -flto -fdata-sections -ffunction-sections)
    endif() 
endif()

add_executable(${PROJECT_NAME} "src/wol.cpp")
install(TARGETS ${PROJECT_NAME})
