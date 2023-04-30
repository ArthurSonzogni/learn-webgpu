#pragma once

#include <webgpu/webgpu.h>

/// A wrapper around a WGPUAdapter.
class Adapter {
 public:
  Adapter(const WGPUInstance& instance, const WGPUSurface& surface);
  void ListAdapterFeatures();
  ~Adapter();

  operator bool() { return adapter_ != nullptr; }

  operator const WGPUAdapter&() const { return adapter_; }
  operator WGPUAdapter&() { return adapter_; }

 private:
  WGPUAdapter adapter_;
};
