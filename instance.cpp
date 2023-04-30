#include "instance.hpp"
#include <glfw3webgpu.h>

Instance::Instance() {
  WGPUInstanceDescriptor options = {
      .nextInChain = nullptr,
  };
  instance_ = wgpuCreateInstance(&options);
}

Instance::~Instance() {}
