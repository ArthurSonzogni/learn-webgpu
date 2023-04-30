cmake_minimum_required(VERSION 3.0)
project(
  LearnWebGPU
  VERSION 0.1.0
  LANGUAGES CXX C
  )

include(FetchContent)

FetchContent_Declare(glfw3
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG        7482de6071d21db77a7236155da44c172a7f6c9e
  FIND_PACKAGE_ARGS NAMES glfw3
  )
FetchContent_Declare(
  webgpu
  GIT_REPOSITORY https://github.com/eliemichel/WebGPU-distribution
  GIT_TAG        wgpu-v0.1.0
  FIND_PACKAGE_ARGS NAMES webgpu
)

# Include https://github.com/eliemichel/glfw3webgpu
FetchContent_Declare(glfw3webgpu
  GIT_REPOSITORY https://github.com/eliemichel/glfw3webgpu
  GIT_TAG        v1.0.1
  FIND_PACKAGE_ARGS NAMES glfw3webgpu
  )

FetchContent_MakeAvailable(glfw3)
FetchContent_MakeAvailable(webgpu)
FetchContent_MakeAvailable(glfw3webgpu)

add_executable(App main.cpp)
target_link_libraries(App PRIVATE glfw)
target_link_libraries(App PRIVATE webgpu)
target_link_libraries(App PRIVATE glfw3webgpu)

set_target_properties(App PROPERTIES
  CXX_STANDARD 20
  COMPILE_WARNING_AS_ERROR ON
  )

if (MSVC)
  target_compile_options(App PRIVATE /W4)
else()
  target_compile_options(App PRIVATE -Wall -Wextra -pedantic)
endif()
