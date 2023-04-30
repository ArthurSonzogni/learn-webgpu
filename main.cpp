#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>
#include <webgpu/webgpu.h>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>
#include "adapter.hpp"
#include "device.hpp"
#include "instance.hpp"
#include "webgpu-release.h"
#include "window.hpp"

int main(int, char**) {
  Window window(640, 480, "WebGPU Test");
  if (!window) {
    std::cerr << "Could not create window!" << std::endl;
    return EXIT_FAILURE;
  }

  Instance instance;
  if (!instance) {
    std::cerr << "Could not create WebGPU instance!" << std::endl;
    return EXIT_FAILURE;
  }

  WGPUSurface surface = glfwGetWGPUSurface(instance, window);
  Adapter adapter(instance, surface);
  Device device(adapter);

  WGPUQueue queue = wgpuDeviceGetQueue(device);
  std::cout << "Got queue: " << queue << std::endl;

  WGPUCommandEncoderDescriptor encoderDesc = {
      .nextInChain = nullptr,
      .label = "My command encoder",
  };
  WGPUCommandEncoder encoder =
      wgpuDeviceCreateCommandEncoder(device, &encoderDesc);

  wgpuCommandEncoderInsertDebugMarker(encoder, "Do one thing");
  wgpuCommandEncoderInsertDebugMarker(encoder, "Do another thing");

  std::cout << "Encoder: " << encoder << std::endl;

  WGPUCommandBufferDescriptor cmdBufferDescriptor = {
      .nextInChain = nullptr,
      .label = "Command buffer",
  };
  WGPUCommandBuffer command =
      wgpuCommandEncoderFinish(encoder, &cmdBufferDescriptor);

  // Finally submit the command queue
  std::cout << "Submitting command..." << std::endl;
  wgpuQueueSubmit(queue, 1, &command);

  WGPUSwapChainDescriptor swapChainDesc = {
      .nextInChain = nullptr,
      .usage = WGPUTextureUsage_RenderAttachment,
      .format = wgpuSurfaceGetPreferredFormat(surface, adapter),
      .width = 640,
      .height = 480,
      .presentMode = WGPUPresentMode_Fifo,
  };
  WGPUSwapChain swapChain =
      wgpuDeviceCreateSwapChain(device, surface, &swapChainDesc);
  std::cout << "Got swap chain: " << swapChain << std::endl;

  wgpuSwapChainRelease(swapChain);

  return EXIT_SUCCESS;
}
