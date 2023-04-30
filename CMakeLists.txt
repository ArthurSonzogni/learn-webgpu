cmake_minimum_required(VERSION 3.0)
project(
  LearnWebGPU
  VERSION 0.1.0
  LANGUAGES CXX C
  )

include(FetchContent)

add_executable(App 
  adapter.cpp
  adapter.hpp
  device.cpp
  device.hpp
  instance.cpp
  instance.hpp
  main.cpp
  window.cpp
  window.hpp
  swapchain.cpp
  swapchain.hpp
  )

set_target_properties(App PROPERTIES
  CXX_STANDARD 20
  COMPILE_WARNING_AS_ERROR OFF
  )

if (MSVC)
  target_compile_options(App PRIVATE /W4)
else()
  target_compile_options(App PRIVATE -Wall -Wextra -pedantic)
endif()

if (EMSCRIPTEN)
  target_link_libraries(App PUBLIC "-s USE_GLFW=3")
else()
  FetchContent_Declare(glfw3
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG        7482de6071d21db77a7236155da44c172a7f6c9e
    FIND_PACKAGE_ARGS NAMES glfw3
    )
  FetchContent_MakeAvailable(glfw3)
  target_link_libraries(App PRIVATE glfw)

  FetchContent_Declare(glfw3webgpu
    GIT_REPOSITORY https://github.com/eliemichel/glfw3webgpu
    GIT_TAG        v1.0.1
    FIND_PACKAGE_ARGS NAMES glfw3webgpu
    )
  FetchContent_MakeAvailable(glfw3webgpu)
  target_link_libraries(App PRIVATE glfw3webgpu)
endif()

FetchContent_Declare(
  webgpu
  GIT_REPOSITORY https://github.com/eliemichel/WebGPU-distribution
  GIT_TAG        wgpu-v0.1.0
  FIND_PACKAGE_ARGS NAMES webgpu
)
FetchContent_MakeAvailable(webgpu)
target_link_libraries(App PRIVATE webgpu)

