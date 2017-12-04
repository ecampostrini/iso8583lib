set(EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/examples)

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB EXAMPLE_HEADERS ${EXAMPLES_DIR}/*Example.hpp)

add_executable(isolib_test ${TEST_SOURCES} ${SOURCES})
target_include_directories(isolib_test PUBLIC ${THIRD_PARTY_INCLUDES}
                                              ${EXAMPLES_DIR})

enable_testing()
add_test(NAME UnitTest COMMAND isolib_test)
