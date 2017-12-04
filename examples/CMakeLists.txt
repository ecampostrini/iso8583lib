file(GLOB FACTORY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB FACTORY_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_executable(main ${FACTORY_SOURCES} ${FACTORY_HEADERS})
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(main ${PROJECT})
