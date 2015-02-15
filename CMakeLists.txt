project(bunsan_curl)

include(${CMAKE_SOURCE_DIR}/system-config.cmake OPTIONAL)

cmake_minimum_required(VERSION 2.8)

include(BunsanCommon)

bunsan_use_bunsan(common)
bunsan_use_boost(
    system
    filesystem
    regex
    thread
)
bunsan_use_curl()

bunsan_add_shared_library(${PROJECT_NAME}
    src/easy.cpp
    src/easy_escape.cpp
    src/error_category.cpp
    src/global.cpp
    src/http_version.cpp

    src/http/header.cpp
    src/http/header_parser.cpp
    src/http/response_head.cpp
    src/http/status.cpp

    src/options/option_set.cpp

    src/detail/curl_delete.cpp
    src/detail/slist.cpp
    src/detail/string_list.cpp
)

bunsan_install_headers()
bunsan_install_targets(${PROJECT_NAME})

bunsan_include_tests()
