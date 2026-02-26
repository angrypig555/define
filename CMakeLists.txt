cmake_minimum_required(VERSION 3.10.0)
project(define VERSION 0.1.0 LANGUAGES C CXX)


find_package(CURL REQUIRED)
find_path(CURLPP_INCLUDE_DIR curlpp/cURLpp.hpp)
find_library(CURLPP_LIBRARY curlpp)
find_package(nlohmann_json REQUIRED)
add_executable(define src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CURLPP_INCLUDE_DIR} ${CURL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CURLPP_LIBRARY} ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)
include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
