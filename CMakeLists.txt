cmake_minimum_required (VERSION 2.8)

project (libbase C CXX)

if (UNIX)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release")
    endif()
	
    set (CXX_FLAGS
         -g
         -fPIC
         -Wall
         -Wno-unused-parameter
         -Wno-unused-function
         -Wunused-variable
         -Wunused-value
         -Wshadow
         -Wcast-qual
         -Wcast-align
         -Wwrite-strings
         -Wsign-compare
         -Winvalid-pch
         -fms-extensions
         -Wfloat-equal
         -Wextra
         -std=c++11
    )
    include_directories (${PROJECT_SOURCE_DIR}/
                        ${PROJECT_SOURCE_DIR}/include
						${PROJECT_SOURCE_DIR}/thirdpart
						${PROJECT_SOURCE_DIR}/thirdpart/cjson
						${PROJECT_SOURCE_DIR}/thirdpart/zlib)
	
	link_directories(${PROJECT_SOURCE_DIR}/libs/x86 )
	
else (UNIX)
	set (CMAKE_GENERATOR "Visual Studio 14 2015")
	set (CXX_FLAGS -DWIN32 -D_WIN32 -D_WINDOWS -EHsc)
    include_directories(${PROJECT_SOURCE_DIR}/
                        ${PROJECT_SOURCE_DIR}/include
						${PROJECT_SOURCE_DIR}/thirdpart
						${PROJECT_SOURCE_DIR}/thirdpart/cjson
						${PROJECT_SOURCE_DIR}/thirdpart/zlib)

    # On windows, Release mode
    link_directories(${PROJECT_SOURCE_DIR}/libs/x86 )

endif (UNIX)

string (REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

if (UNIX)
	set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb -ffunction-sections")
	set(CMAKE_C_FLAGS_RELEASE "-O3 -ggdb -ffunction-sections")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -ggdb -D_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g -ggdb -DNDEBUG")
	set(CMAKE_LD_FLAGS "-Wl,-gc-sections  -static-libstdc++ -static-libgcc")
    set(DEPENDENT_LIBRARIES pthread rt curl z)
else (UNIX)
	set(CMAKE_CXX_FLAGS_RELEASE "/MT")
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
    set(DEPENDENT_LIBRARIES libcurl ws2_32 wldap32)
endif (UNIX)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/out)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/out)

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

message (STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
message (STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})
message (STATUS "CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS})
message (STATUS "EXECUTABLE_OUTPUT_PATH " ${EXECUTABLE_OUTPUT_PATH})
message (STATUS "LIBRARY_OUTPUT_PATH " ${LIBRARY_OUTPUT_PATH})

add_subdirectory (base)
