cmake_minimum_required(VERSION 3.0.0)
project(pidmx VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

option(NBT_BUILD_TESTS "" OFF)
add_subdirectory(libnbtplusplus)

add_library(imgui SHARED)
target_sources(imgui PRIVATE
	imgui/imgui.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
	imgui/imgui_draw.cpp
	imgui/backends/imgui_impl_glfw.cpp
	imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
	imgui
	imgui/backends
)
target_compile_definitions(imgui PUBLIC IMGUI_DEFINE_MATH_OPERATORS)


add_library(glad SHARED)
target_sources(glad PRIVATE
	src/glad/src/glad.c
)
target_include_directories(glad PUBLIC
	src/glad/include
)


add_library(js SHARED)
target_sources(js PRIVATE
	src/js/exec.cpp
	src/js/globals.cpp
	src/js/init.cpp
	src/js/template.cpp
	src/js/impl/console.cpp
	src/js/impl/require.cpp
	src/js/v8ObjectInterface.cpp
)
target_compile_definitions(js PUBLIC V8_COMPRESS_POINTERS V8_ENABLE_SANDBOX)
target_include_directories(js PUBLIC
	src/js
	src/js/libv8/include
)

# add_library(libv8_base IMPORTED SHARED)
# set_property(TARGET libv8_base PROPERTY IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/src/js/libv8/x64.release/libv8_libbase.so)
# add_library(libv8_platform IMPORTED SHARED)
# set_property(TARGET libv8_platform PROPERTY IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/src/js/libv8/x64.release/libv8_libplatform.so)
# add_library(libv8 IMPORTED SHARED)
# set_property(TARGET libv8 PROPERTY IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/src/js/libv8/x64.release/libv8.so)
# 
# target_link_libraries(js libv8_base)
# target_link_libraries(js libv8_platform)
# target_link_libraries(js libv8)

target_link_libraries(js ${CMAKE_HOME_DIRECTORY}/src/js/libv8/x64.debug/libv8_monolith.a)
target_link_libraries(js ${CMAKE_HOME_DIRECTORY}/src/js/libv8/x64.debug/libv8_libbase.a)
target_link_libraries(js ${CMAKE_HOME_DIRECTORY}/src/js/libv8/x64.debug/libv8_libplatform.a)


add_executable(pidmx 
	main.cpp
	src/pidmx_utils.cpp

	src/engine/command/CommandBuffer.cpp

        src/engine/core/Engine.cpp
	src/engine/core/Fixture.cpp
	src/engine/core/FixtureCollection.cpp
	src/engine/core/Show.cpp

	src/engine/ui/Tools.cpp
	src/engine/ui/MainPanel.cpp
        src/engine/ui/PatchFixturesPanel.cpp
	src/engine/ui/PanelConsole.cpp

	src/engine/Console.cpp
	src/engine/ui/ImGuiExt.cpp src/engine/ui/ImGuiExt.h src/engine/ui/Panel.h src/engine/core/Utils.h src/engine/ui/blackboard/Panel.cpp src/engine/ui/blackboard/Panel.h src/engine/ui/blackboard/Item.cpp src/engine/ui/blackboard/Item.h src/engine/ui/Panel.cpp src/engine/command/CommandBlackboard.h src/engine/ui/blackboard/ItemCollection.h src/engine/core/FixturePreset.cpp src/engine/ui/blackboard/ItemGroups.cpp src/engine/ui/blackboard/ItemGroups.h src/engine/ui/blackboard/ItemFixtureSheet.cpp src/engine/ui/blackboard/ItemFixtureSheet.h)

target_include_directories(pidmx PUBLIC src)

target_link_libraries(pidmx glfw)
target_link_libraries(pidmx glad)
target_link_libraries(pidmx js)
target_link_libraries(pidmx imgui)
target_link_libraries(pidmx OpenGL::GL)
target_link_libraries(pidmx nbt++)
