file(GLOB base_SRCS *.cc *.cpp *.c *.h log/*.c log/*.cc log/*.cpp log/*.h minizip/ioapi.c minizip/miniunz.c  minizip/mztools.c minizip/unzip.c minizip/zip.c minizip/*.h)
file(GLOB thirdpart_SRCS ../thirdpart/cjson/cJSON.c ../thirdpart/tinyxml2/tinyxml2.cpp)

if(WIN32)
list(APPEND base_SRCS ${PROJECT_SOURCE_DIR}/base/minizip/iowin32.c ${PROJECT_SOURCE_DIR}/base/minizip/iowin32.h)
endif()

message(STATUS "base_SRCS : " ${base_SRCS})

include_directories(${PROJECT_SOURCE_DIR}
                    ${PROJECT_SOURCE_DIR}/base/log
					${PROJECT_SOURCE_DIR}/base/minizip)


add_definitions(-D BASE_STATIC)
add_definitions(-D CURL_STATICLIB)

#如果生成动态库则使用add_library命令
add_library(base_static STATIC ${base_SRCS} ${minizip_SRCS} ${thirdpart_SRCS})
target_link_libraries(base_static ${DEPENDENT_LIBRARIES})

#add_library(base ${base_SRCS} ${thirdpart_SRCS})
#target_link_libraries(base ${DEPENDENT_LIBRARIES})