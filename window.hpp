#pragma once

#include <GLFW/glfw3.h>

/// A wrapper around a GLFWwindow.
class Window {
 public:
  Window(int width, int height, const char* title);
  ~Window();

  operator bool() { return window_ != nullptr; }
  bool operator!() { return window_ == nullptr; }

  operator const GLFWwindow*() const { return window_; }
  operator GLFWwindow*() { return window_; }

 private:
  GLFWwindow* window_;
};
