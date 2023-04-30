#include "instance.hpp"
#include <glfw3webgpu.h>
#include "webgpu-release.h"

Instance::Instance() {
  WGPUInstanceDescriptor options = {
      .nextInChain = nullptr,
  };
  instance_ = wgpuCreateInstance(&options);
}

Instance::~Instance() {
  wgpuInstanceRelease(instance_);
}
