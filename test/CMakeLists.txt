FILE(GLOB_RECURSE MINISQL_TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*/*test.cpp)

SET(TEST_MAIN_PATH ${PROJECT_SOURCE_DIR}/test/main_test.cpp)
ADD_EXECUTABLE(minisql_test ${MINISQL_TEST_SOURCES} ${TEST_MAIN_PATH})
ADD_LIBRARY(minisql_test_main ${TEST_MAIN_PATH})
TARGET_LINK_LIBRARIES(minisql_test_main glog gtest)
TARGET_LINK_LIBRARIES(minisql_test minisql_shared glog gtest)

foreach (test_source ${MINISQL_TEST_SOURCES})
    # Create test suit
    get_filename_component(test_filename ${test_source} NAME)
    string(REPLACE ".cpp" "" test_name ${test_filename})
    MESSAGE(STATUS "Create test suit: ${test_name}")

    # Add the test target separately and as part of "make check-tests".
    add_executable(${test_name} EXCLUDE_FROM_ALL ${test_source})
    target_link_libraries(${test_name} minisql_shared glog gtest minisql_test_main)
    # target_link_libraries(${test_name} minisql_shared glog gtest gtest_main)

    # Set test target properties and dependencies.
    set_target_properties(${test_name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
            COMMAND ${test_name}
            )

    # Add the test under CTest.
    add_test(${test_name} ${CMAKE_BINARY_DIR}/test/${test_name} --gtest_color=yes
            --gtest_output=xml:${CMAKE_BINARY_DIR}/test/${test_name}.xml)
endforeach (test_source ${MINISQL_TEST_SOURCES})