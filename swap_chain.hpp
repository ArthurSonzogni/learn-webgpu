#pragma once
#include <webgpu/webgpu.h>

class SwapChain {
 public:
  SwapChain(const WGPUDevice& device,
            const WGPUSurface& surface,
            const WGPUAdapter& adapter);
  ~SwapChain();

 private:
  WGPUSwapChainDescriptor swap_chain_descriptor_;
  WGPUSwapChain swap_chain_;
};
