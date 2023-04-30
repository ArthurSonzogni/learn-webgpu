#pragma once

#include <webgpu/webgpu.h>

/// A wrapper around a WGPUDevice.
class Device {
 public:
  Device(const WGPUAdapter& adapter);
  ~Device();

  operator bool() { return device_ != nullptr; }

  operator const WGPUDevice&() const { return device_; }
  operator WGPUDevice&() { return device_; }
  WGPUDevice* operator*() { return &device_; }

 private:
  WGPUDevice device_;
};
