CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
PROJECT(boohttp)
SET(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
SET(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
SET(CURL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/libs/curl/include)
SET(CURL_LIB_DIR ${PROJECT_SOURCE_DIR}/libs/curl/lib)
SET(CURL_LIB libcurl_imp
	libeay32
	ssleay32.lib)
SET(LIB_SRC src/message.cpp
    src/response.cpp
    src/request.cpp
    src/url.cpp
    src/container.cpp
    src/header.cpp
    src/postdata.cpp
    src/client.cpp
    src/body.cpp)

INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR} ${INCLUDE_DIR})
LINK_DIRECTORIES(${CURL_LIB_DIR})

ADD_LIBRARY(boohttp ${LIB_SRC})

ADD_EXECUTABLE(main src/main.cpp)
TARGET_LINK_LIBRARIES(main ${CURL_LIB} boohttp)
