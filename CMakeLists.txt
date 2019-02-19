# CMakeLists.txt --- CMake project settings
#    ex) cmake -G "Visual Studio 9 2008"
#    ex) cmake -DCMAKE_BUILD_TYPE=Release -G "MSYS Makefiles"
##############################################################################

# CMake minimum version
cmake_minimum_required(VERSION 2.4)

if (COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif()

# enable testing
enable_testing()

# project name and language
project(DeleteDirectory C)

# check build type
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# make uppercase string of build type
string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)

# check build type again
if (NOT uppercase_CMAKE_BUILD_TYPE MATCHES "^(DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL)$")
    message(FATAL_ERROR "Invalid value for CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()

## set output directory (build/)
#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

## enable Win32 resource
#enable_language(RC)

if (WIN32)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # using Clang
        set(CMAKE_C_FLAGS "-static")
        set(CMAKE_CXX_FLAGS "-static")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # using GCC
        set(CMAKE_C_FLAGS "-static -static-libgcc -static-libstdc++")
        set(CMAKE_CXX_FLAGS "-static -static-libgcc -static-libstdc++")
    elseif (MSVC)
        # replace "/MD" with "/MT" (building without runtime DLLs)
        set(CompilerFlags
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        foreach(CompilerFlags ${CompilerFlags})
            string(REPLACE "/MD" "/MT" ${CompilerFlags} "${${CompilerFlags}}")
        endforeach()
    endif()
endif()

##############################################################################

option(USE_CXX "Use C++" ON)
option(NO_STRSAFE "Don't use <strsafe.h>" OFF)

if (NO_STRSAFE)
    add_definitions(-DNO_STRSAFE)
endif()

if (USE_CXX)
    enable_language(CXX)

    if (MSVC)
        string(APPEND CMAKE_CXX_FLAGS " /EHsc ")
    endif()

    # libDeleteDirectoryA.a
    add_library(DeleteDirectoryA STATIC DeleteDirectory.cpp)
    set_target_properties(DeleteDirectoryA PROPERTIES COMPILE_FLAGS "-DMBCS -D_MBCS")
    target_link_libraries(DeleteDirectoryA PRIVATE shlwapi)

    # libDeleteDirectoryW.a
    add_library(DeleteDirectoryW STATIC DeleteDirectory.cpp)
    set_target_properties(DeleteDirectoryW PROPERTIES COMPILE_FLAGS "-DUNICODE -D_UNICODE")
    target_link_libraries(DeleteDirectoryW PRIVATE shlwapi)

    # libDeleteDirectory.a
    add_library(DeleteDirectory STATIC empty.c)
    target_link_libraries(DeleteDirectory PRIVATE DeleteDirectoryA DeleteDirectoryW)

    # DeleteDirectoryTestA.exe
    add_executable(DeleteDirectoryTestA DeleteDirectoryTest.cpp)
    set_target_properties(DeleteDirectoryTestA PROPERTIES COMPILE_FLAGS "-DMBCS -D_MBCS")
    target_link_libraries(DeleteDirectoryTestA PRIVATE DeleteDirectory)

    # DeleteDirectoryTestW.exe
    add_executable(DeleteDirectoryTestW DeleteDirectoryTest.cpp)
    set_target_properties(DeleteDirectoryTestW PROPERTIES COMPILE_FLAGS "-DUNICODE -D_UNICODE")
    target_link_libraries(DeleteDirectoryTestW PRIVATE DeleteDirectory)
else()
    # libDeleteDirectoryA.a
    add_library(DeleteDirectoryA STATIC DeleteDirectory.c)
    set_target_properties(DeleteDirectoryA PROPERTIES COMPILE_FLAGS "-DMBCS -D_MBCS")
    target_link_libraries(DeleteDirectoryA PRIVATE shlwapi)

    # libDeleteDirectoryW.a
    add_library(DeleteDirectoryW STATIC DeleteDirectory.c)
    set_target_properties(DeleteDirectoryW PROPERTIES COMPILE_FLAGS "-DUNICODE -D_UNICODE")
    target_link_libraries(DeleteDirectoryW PRIVATE shlwapi)

    # libDeleteDirectory.a
    add_library(DeleteDirectory STATIC empty.c)
    target_link_libraries(DeleteDirectory PRIVATE DeleteDirectoryA DeleteDirectoryW)

    # DeleteDirectoryTestA.exe
    add_executable(DeleteDirectoryTestA DeleteDirectoryTest.c)
    set_target_properties(DeleteDirectoryTestA PROPERTIES COMPILE_FLAGS "-DMBCS -D_MBCS")
    target_link_libraries(DeleteDirectoryTestA PRIVATE DeleteDirectory)

    # DeleteDirectoryTestW.exe
    add_executable(DeleteDirectoryTestW DeleteDirectoryTest.c)
    set_target_properties(DeleteDirectoryTestW PROPERTIES COMPILE_FLAGS "-DUNICODE -D_UNICODE")
    target_link_libraries(DeleteDirectoryTestW PRIVATE DeleteDirectory)
endif()

add_test(NAME DeleteDirectoryTestA COMMAND DeleteDirectoryTestA)
add_test(NAME DeleteDirectoryTestW COMMAND DeleteDirectoryTestW)

install(TARGETS DeleteDirectory DESTINATION lib)
install(FILES DeleteDirectory.h DESTINATION include)

##############################################################################
