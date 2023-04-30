cmake_minimum_required(VERSION 3.0)
project(
  LearnWebGPU
  VERSION 0.1.0
  LANGUAGES CXX
  )

include(FetchContent)
FetchContent_Declare(glfw3
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG        7482de6071d21db77a7236155da44c172a7f6c9e
  FIND_PACKAGE_ARGS NAMES glfw3
  )
FetchContent_MakeAvailable(glfw3)

add_executable(App main.cpp)
target_link_libraries(App PRIVATE glfw)

set_target_properties(App PROPERTIES
  CXX_STANDARD 17
  COMPILE_WARNING_AS_ERROR ON
  )

if (MSVC)
  target_compile_options(App PRIVATE /W4)
else()
  target_compile_options(App PRIVATE -Wall -Wextra -pedantic)
endif()
