cmake_minimum_required(VERSION 3.0.0)
project(rewind VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(rewind 
	src/rewind.cpp
)
target_include_directories(rewind PUBLIC src)

add_executable(rewind_test src/rewind_test.cpp)
target_link_libraries(rewind_test rewind)
add_test(
	NAME main_test
	COMMAND $<TARGET_FILE:rewind_test>
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
