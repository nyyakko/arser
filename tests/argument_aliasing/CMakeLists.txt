set(DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TEST_NAME argument_aliasing)

add_executable(${TEST_NAME} "${DIR}/${TEST_NAME}.cpp")

enable_static_analyzers(${TEST_NAME} ON)

target_include_directories(${TEST_NAME} PRIVATE "../arser/include")

set_target_properties(${TEST_NAME} PROPERTIES CXX_STANDARD ${CMAKE_CXX_VERSION})

target_link_libraries(${TEST_NAME} PRIVATE arser)
target_compile_options(${TEST_NAME} PRIVATE
    -Werror
    -Wall
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wimplicit-fallthrough
)

add_test(NAME full_argument_name  COMMAND ${TEST_NAME} --full)
add_test(NAME alias_argument_name COMMAND ${TEST_NAME} -f)
