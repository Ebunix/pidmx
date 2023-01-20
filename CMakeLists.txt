cmake_minimum_required(VERSION 3.0.0)
project(pidmx VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)


add_library(imgui STATIC)
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


add_library(glad STATIC)
target_sources(glad PRIVATE
	glad/src/glad.c
)
target_include_directories(glad PUBLIC
	glad/include
)


add_library(js STATIC)
target_sources(js PRIVATE
	js/exec.cpp
	js/globals.cpp
	js/init.cpp
	js/template.cpp
	js/impl/console.cpp
	js/impl/require.cpp
)
target_compile_definitions(js PUBLIC V8_COMPRESS_POINTERS V8_ENABLE_SANDBOX)
target_include_directories(js PUBLIC
	js
	js/libv8/include
)
target_link_libraries(js ${CMAKE_HOME_DIRECTORY}/js/libv8/x64.debug/libv8_monolith.a)
target_link_libraries(js ${CMAKE_HOME_DIRECTORY}/js/libv8/x64.debug/libv8_libbase.a)
target_link_libraries(js ${CMAKE_HOME_DIRECTORY}/js/libv8/x64.debug/libv8_libplatform.a)


add_executable(pidmx 
	main.cpp

	src/ImGuiExt.cpp
	src/pidmx_utils.cpp
	
	src/engine/Universe.cpp
	src/engine/Engine.cpp
	src/engine/Modulator.cpp
	src/engine/Parameter.cpp
	src/engine/Fixture.cpp
	src/engine/JsTools.cpp
	src/engine/Console.cpp

	src/PanelFixtureMap.cpp
	src/PanelChannelMonitor.cpp
	src/PanelChannelEditor.cpp
	src/PanelConsole.cpp
)
target_include_directories(pidmx PUBLIC src)

target_link_libraries(pidmx glfw)
target_link_libraries(pidmx glad)
target_link_libraries(pidmx js)
target_link_libraries(pidmx imgui)
target_link_libraries(pidmx OpenGL::GL)