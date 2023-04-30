#pragma once

#include <webgpu/webgpu.h>

/// A wrapper around a WGPUInstance.
class Instance {
 public:
  Instance();
  ~Instance();

  WGPUInstance* operator&() { return &instance_; }
  operator bool() { return instance_ != nullptr; }
  operator const WGPUInstance&() const { return instance_; }
  operator WGPUInstance&() { return instance_; }

 private:
  WGPUInstance instance_;
};
