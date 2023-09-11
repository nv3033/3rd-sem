cmake_minimum_required (VERSION 3.8)
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  cmake_policy(SET CMP0079 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
set(PROJECT_NAME 3rd-sem)
project (${PROJECT_NAME})
add_subdirectory ("src")
add_executable(3rd-sem
"src/3rd sem.cpp"
"src/3rd sem.h"
"src/include/hexsagon_header.h"
"external/stb_image.h"
"external/json.hpp" "src/include/hexsagon_header.h")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)
add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm)
add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)
add_subdirectory(external/imgui)
target_link_libraries(${PROJECT_NAME} ImGui)
target_include_directories(ImGui PUBLIC external/)
target_link_libraries(ImGui PRIVATE glad glfw)
