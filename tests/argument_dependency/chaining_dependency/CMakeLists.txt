set(DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TEST_NAME chaining_dependency)

add_executable(${TEST_NAME} "${DIR}/${TEST_NAME}.cpp")

enable_static_analyzers(${TEST_NAME} ON)

target_include_directories(${TEST_NAME} PRIVATE ${arser_HeaderFiles})

set_target_properties(${TEST_NAME} PROPERTIES CXX_STANDARD ${CMAKE_CXX_VERSION})

target_link_libraries(${TEST_NAME} PRIVATE arser)
target_compile_options(${TEST_NAME} PRIVATE ${arser_CompilerOptions})

add_test(NAME correct_dependency_chain   COMMAND ${TEST_NAME} --C1 --flagC --flagB --flagA)
add_test(NAME incorrect_dependency_chain COMMAND ${TEST_NAME} --C2 --flagB --flagC --flagA)
