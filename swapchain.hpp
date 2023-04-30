#pragma once
#include <webgpu/webgpu.h>

class SwapChain {
 public:
  SwapChain(const WGPUDevice& device,
            const WGPUSurface& surface,
            const WGPUAdapter& adapter);
  ~SwapChain();

  operator const WGPUSwapChain&() const { return swap_chain_; }
  operator WGPUSwapChain&() { return swap_chain_; }

 private:
  WGPUSwapChainDescriptor swap_chain_descriptor_;
  WGPUSwapChain swap_chain_;
};
