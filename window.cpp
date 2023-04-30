#include "window.hpp"

Window::Window(int width, int height, const char* title) {
  if (!glfwInit()) {
    return;
  }
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
}

Window::~Window() {
  glfwDestroyWindow(window_);
  glfwTerminate();
}
