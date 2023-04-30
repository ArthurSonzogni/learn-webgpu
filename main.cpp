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
#include "swapchain.hpp"

#define FAIL_IF_NULL(expr)                               \
  if (expr == nullptr) {                                 \
    std::cerr << "Fail to create" << #expr << std::endl; \
    return EXIT_FAILURE;                                 \
  }

int main(int, char**) {
  Instance instance;
  FAIL_IF_NULL(instance);

  Window window(640, 480, "WebGPU Test");
  FAIL_IF_NULL(window);

  WGPUSurface surface = glfwGetWGPUSurface(instance, window);
  Adapter adapter(instance, surface);

  Device device(adapter);
  FAIL_IF_NULL(device);

  WGPUQueue queue = wgpuDeviceGetQueue(device);
  FAIL_IF_NULL(queue);

  SwapChain swapChain(device, surface, adapter);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    WGPUTextureView nextTexture = wgpuSwapChainGetCurrentTextureView(swapChain);
    FAIL_IF_NULL(nextTexture);

    // Create the command encoder.
    WGPUCommandEncoderDescriptor encoderDesc = {
        .nextInChain = nullptr,
        .label = "My command encoder",
    };
    WGPUCommandEncoder encoder =
        wgpuDeviceCreateCommandEncoder(device, &encoderDesc);

    // Add a renderpass:
    WGPURenderPassColorAttachment renderPassColorAttachment = {
        .view = nextTexture,
        .resolveTarget = nullptr,
        .loadOp = WGPULoadOp_Clear,
        .storeOp = WGPUStoreOp_Store,
        .clearValue = WGPUColor{0.9, 0.1, 0.2, 1.0},
    };
    WGPURenderPassDescriptor renderPassDesc = {
      .nextInChain = nullptr,
      .colorAttachmentCount = 1,
      .colorAttachments = &renderPassColorAttachment,
      .depthStencilAttachment = nullptr,
      .timestampWriteCount = 0,
      .timestampWrites = nullptr,
    };
    WGPURenderPassEncoder renderPass =
        wgpuCommandEncoderBeginRenderPass(encoder, &renderPassDesc);
    wgpuRenderPassEncoderEnd(renderPass);

    wgpuTextureViewRelease(nextTexture);

    WGPUCommandBufferDescriptor cmdBufferDescriptor = {
        .nextInChain = nullptr,
        .label = "Command buffer",
    };
    WGPUCommandBuffer command =
        wgpuCommandEncoderFinish(encoder, &cmdBufferDescriptor);

    wgpuQueueSubmit(queue, 1, &command);

    wgpuSwapChainPresent(swapChain);
  }

  return EXIT_SUCCESS;
}
