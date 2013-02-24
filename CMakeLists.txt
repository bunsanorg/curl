project(bunsan_curl)

include(${CMAKE_SOURCE_DIR}/system-config.cmake OPTIONAL)

cmake_minimum_required(VERSION 2.8)

include(BunsanCommon)

bunsan_use_bunsan(common)
bunsan_use_boost(system filesystem serialization unit_test_framework thread)
bunsan_use_curl()

bunsan_add_shared_library(${PROJECT_NAME}
    src/error_category.cpp
    src/error.cpp
    src/global.cpp
    src/easy.cpp
)

bunsan_install_headers()
bunsan_install_targets(${PROJECT_NAME})

bunsan_include_tests()
