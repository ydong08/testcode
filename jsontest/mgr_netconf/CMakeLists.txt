

cmake_minimum_required(VERSION 2.8.12)

project(netopeer2-mgr C)

set(PROJECT_PUBLIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../")
message("public dir: ${PROJECT_PUBLIC_DIR}")

set(OPENSSL_ROOT_DIR "${PROJECT_PUBLIC_DIR}/openssl-1.0.1p/OPSSL")
set(OPENSSL_LIBRARIES_DIR "${OPENSSL_ROOT_DIR}/lib/")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
message("openssl include: ${OPENSSL_INCLUDE_DIR}")

set(JSON_ROOT_DIR "${PROJECT_PUBLIC_DIR}/libjson-1.5/")
set(JSON_LIBRARIES_DIR "${JSON_ROOT_DIR}")
#set(JSON_INCLUDE_DIR "${JSON_ROOT_DIR}/mjson-1.5")
set(JSON_INCLUDE_DIR "${JSON_ROOT_DIR}")
message("json include: ${JSON_INCLUDE_DIR}")

set(CURL_ROOT_DIR "${PROJECT_PUBLIC_DIR}/curl-7.48.0/curl_install")
set(CURL_LIBRARIES_DIR "${CURL_ROOT_DIR}/lib")
set(CURL_INCLUDE_DIR "${CURL_ROOT_DIR}/include")
message("curl include: ${CURL_INCLUDE_DIR}")

# source files
set(MGR_SRC netconf_mgr.c)

# netopeer2-mgr target
add_executable(netopeer2-mgr ${MGR_SRC})

find_library(CURL_LIBRARY NAMES curl
             HINTS ${CURL_LIBRARIES_DIR})
find_library(MJSON_LIBRARY NAMES mjson
             HINTS ${JSON_LIBRARIES_DIR})
find_library(CRYPTO_LIBRARY NAMES crypto
             HINTS ${OPENSSL_LIBRARIES_DIR}) 

# dependencies - libcurl
target_link_libraries(netopeer2-mgr ${CURL_LIBRARY})
include_directories(${CURL_INCLUDE_DIR})

# dependencies - libcrypto
target_link_libraries(netopeer2-mgr ${CRYPTO_LIBRARY})
include_directories(${OPENSSL_INCLUDE_DIR})

# dependencies - libmjson
target_link_libraries(netopeer2-mgr ${MJSON_LIBRARY})
include_directories(${JSON_INCLUDE_DIR})

# install binary
install(TARGETS netopeer2-mgr DESTINATION ${CMAKE_INSTALL_BINDIR})
