#include "instance.hpp"
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
