file(GLOB TSOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
set(TEST_SOURCES ${TSOURCES})

add_executable(isolib_test ${TEST_SOURCES})
target_include_directories(isolib_test PUBLIC ${THIRD_PARTY_INCLUDES})

enable_testing()
add_test(NAME UnitTest COMMAND isolib_test)
