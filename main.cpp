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
#include "swap_chain.hpp"

#define FAIL_IF_NULL(expr) \
  if (expr == nullptr) { \
    std::cerr << "Fail to create" << #expr << std::endl; \
    return EXIT_FAILURE; \
  }

int main(int, char**) {
  Window window(640, 480, "WebGPU Test");
  FAIL_IF_NULL(window);

  Instance instance;
  FAIL_IF_NULL(instance);

  WGPUSurface surface = glfwGetWGPUSurface(instance, window);
  Adapter adapter(instance, surface);
  Device device(adapter);
  FAIL_IF_NULL(device);

  WGPUQueue queue = wgpuDeviceGetQueue(device);

  WGPUCommandEncoderDescriptor encoderDesc = {
      .nextInChain = nullptr,
      .label = "My command encoder",
  };
  WGPUCommandEncoder encoder =
      wgpuDeviceCreateCommandEncoder(device, &encoderDesc);

  //wgpuCommandEncoderInsertDebugMarker(encoder, "Do one thing");
  //wgpuCommandEncoderInsertDebugMarker(encoder, "Do another thing");

  //std::cout << "Encoder: " << encoder << std::endl;

  //WGPUCommandBufferDescriptor cmdBufferDescriptor = {
      //.nextInChain = nullptr,
      //.label = "Command buffer",
  //};
  //WGPUCommandBuffer command =
      //wgpuCommandEncoderFinish(encoder, &cmdBufferDescriptor);

  //wgpuQueueSubmit(queue, 1, &command);

  //SwapChain swap_chain(device, surface, adapter);
  //(void)swap_chain;

  return EXIT_SUCCESS;
}
