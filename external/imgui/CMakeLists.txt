cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(ImGui)

set(IMGUI_INCLUDES
	imgui.h
	backends/imgui_impl_glfw.h
	backends/imgui_impl_opengl3.h
)
set(IMGUI_SOURCES
	imgui.cpp
	backends/imgui_impl_glfw.cpp
	backends/imgui_impl_opengl3.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
    imgui_tables.cpp
    imgui_demo.cpp
)
add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)
target_include_directories(ImGui PUBLIC backends/)
target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
