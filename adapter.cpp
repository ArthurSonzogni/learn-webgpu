#include "adapter.hpp"

#include <iostream>
#include <vector>
#include "webgpu-release.h"

Adapter::Adapter(const WGPUInstance& instance, const WGPUSurface& surface) {
  WGPURequestAdapterOptions options = {
      .nextInChain = nullptr,
      .compatibleSurface = surface,
  };
  wgpuInstanceRequestAdapter(
      instance, &options,
      [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const*,
         void* that) {
        if (status == WGPURequestAdapterStatus_Success) {
          reinterpret_cast<Adapter*>(that)->adapter_ = adapter;
        }
      },
      this);
  ListAdapterFeatures();
}

void Adapter::ListAdapterFeatures() {
  std::vector<WGPUFeatureName> features;
  size_t featureCount = wgpuAdapterEnumerateFeatures(adapter_, nullptr);
  features.resize(featureCount);
  wgpuAdapterEnumerateFeatures(adapter_, features.data());
  std::cout << "Adapter features:" << std::endl;
  for (auto f : features) {
    std::cout << " - " << f << std::endl;
  }
}

Adapter::~Adapter() {
  wgpuAdapterRelease(adapter_);
}
