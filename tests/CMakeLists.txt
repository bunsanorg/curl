bunsan_tests_project_header()

bunsan_tests_project_add_test(easy easy.cpp)
bunsan_tests_project_add_test(slist slist.cpp)
bunsan_tests_project_add_test(wrapper wrapper.cpp)

bunsan_find_python3()

macro(bunsan_curl_server_test target)
    bunsan_tests_project_add_executable(test_${target} ${target}.cpp)
    set(test_executable ${CMAKE_CURRENT_BINARY_DIR}/test_${target})
    bunsan_tests_project_add_custom_test(${target}
        ${PYTHON_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/server.py
        ${test_executable}
    )
endmacro()

bunsan_curl_server_test(easy_options)

bunsan_tests_project_footer()
