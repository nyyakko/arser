set(DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TEST_NAME boolean_parsing)

add_executable(${TEST_NAME} "${DIR}/${TEST_NAME}.cpp")

enable_static_analyzers(${TEST_NAME} ON)

target_include_directories(${TEST_NAME} PRIVATE ${arser_HeaderFiles})

set_target_properties(${TEST_NAME} PROPERTIES CXX_STANDARD ${CMAKE_CXX_VERSION})

target_link_libraries(${TEST_NAME} PRIVATE arser)
target_compile_options(${TEST_NAME} PRIVATE ${arser_CompilerOptions})

add_test(NAME parse_boolean_from_number COMMAND ${TEST_NAME} --fromNumber 1)
add_test(NAME parse_boolean_from_string COMMAND ${TEST_NAME} --fromString true)
