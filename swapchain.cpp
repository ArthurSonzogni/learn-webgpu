#include "swapchain.hpp"

#include "webgpu-release.h"

SwapChain::SwapChain(const WGPUDevice& device, const WGPUSurface& surface, const WGPUAdapter& adapter):
      swap_chain_descriptor_{
          .nextInChain = nullptr,
          .usage = WGPUTextureUsage_RenderAttachment,
          .format = wgpuSurfaceGetPreferredFormat(surface, adapter),
          .width = 640,
          .height = 480,
          .presentMode = WGPUPresentMode_Fifo,
  },
  swap_chain_{
  wgpuDeviceCreateSwapChain(device, surface, &swap_chain_descriptor_)} {}

SwapChain::~SwapChain() {
  wgpuSwapChainRelease(swap_chain_);
}
