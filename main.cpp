#include <GLFW/glfw3.h>
#include <webgpu/webgpu.h>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>
#include "adapter.hpp"
#include "device.hpp"
#include "instance.hpp"
#include "swapchain.hpp"
#include "webgpu-release.h"
#include "window.hpp"


#ifdef EMSCRIPTEN
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/html5_webgpu.h>
#else
#include <glfw3webgpu.h>
#endif

#define FAIL_IF_NULL(expr)                               \
  if (expr == nullptr) {                                 \
    std::cerr << "Fail to create" << #expr << std::endl; \
    return EXIT_FAILURE;                                 \
  }

int main(int, char**) {
  Instance instance;
  FAIL_IF_NULL(instance);

#ifdef EMSCRIPTEN
  WGPUSurfaceDescriptorFromCanvasHTMLSelector canvasDesc = {};
  canvasDesc.chain.sType = WGPUSType_SurfaceDescriptorFromCanvasHTMLSelector;
  canvasDesc.selector = "canvas";

  WGPUSurfaceDescriptor surfDesc = {};
  surfDesc.nextInChain = &canvasDesc.chain;

  WGPUSurface surface = wgpuInstanceCreateSurface(instance, &surfDesc);
#else
  Window window(640, 480, "WebGPU Test");
  FAIL_IF_NULL(window);

  WGPUSurface surface = glfwGetWGPUSurface(instance, window);
#endif
  Adapter adapter(instance, surface);

  Device device(adapter);
  FAIL_IF_NULL(device);

  WGPUQueue queue = wgpuDeviceGetQueue(device);
  FAIL_IF_NULL(queue);

  SwapChain swapChain(device, surface, adapter);
  
  WGPUTextureFormat swapChainFormat =
      wgpuSurfaceGetPreferredFormat(surface, adapter);

  // Create a triangle:
  std::vector<float> vertexData = {
      -0.5, -0.5,  //
      +0.5, -0.5,  //
      +0.0, +0.5,  //
  };
  WGPUBufferDescriptor bufferDesc = {
      .nextInChain = nullptr,
      .label = "Some GPU-side data buffer",
      .usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex,
      .size = 16,
      .mappedAtCreation = false,
  };
  WGPUBuffer buffer = wgpuDeviceCreateBuffer(device, &bufferDesc);
  wgpuQueueWriteBuffer(queue, buffer, 0, vertexData.data(),
                       bufferDesc.size);

  WGPUVertexAttribute vertexAttrib = {
      .format = WGPUVertexFormat_Float32x2,
      .offset = 0,
      .shaderLocation = 0,
  };
  WGPUVertexBufferLayout vertexBufferLayout{
      .arrayStride = 2 * sizeof(float),
      .stepMode = WGPUVertexStepMode_Vertex,
      .attributeCount = 1,
      .attributes = &vertexAttrib,
  };

  const char* shaderSource = R"(
@vertex
fn vs_main(
  @location(0) in_vertex_position: vec2<f32>
) -> @builtin(position) vec4<f32> {
  return vec4<f32>(in_vertex_position, 0.0, 1.0);
}

@fragment
fn fs_main() -> @location(0) vec4<f32> {
    return vec4<f32>(1.0, 0.0, 0.0, 1.0);
}
)";

  // Use the extension mechanism to load a WGSL shader source code
  WGPUShaderModuleWGSLDescriptor shaderCodeDesc {
    .chain =
        {
            .next = nullptr,
            .sType = WGPUSType_ShaderModuleWGSLDescriptor,
        },
    .code = shaderSource,
  };

  WGPUShaderModuleDescriptor shaderDesc{
      .nextInChain = &shaderCodeDesc.chain,
      .hintCount = 0,
      .hints = nullptr,
  };

  WGPUShaderModule shaderModule =
      wgpuDeviceCreateShaderModule(device, &shaderDesc);

  WGPUBlendState blendState{
      .color =
          {
              .operation = WGPUBlendOperation_Add,
              .srcFactor = WGPUBlendFactor_SrcAlpha,
              .dstFactor = WGPUBlendFactor_OneMinusSrcAlpha,
          },
      .alpha =
          {
              .operation = WGPUBlendOperation_Add,
              .srcFactor = WGPUBlendFactor_Zero,
              .dstFactor = WGPUBlendFactor_One,
          },
  };

  WGPUColorTargetState colorTarget{
      .nextInChain = nullptr,
      .format = swapChainFormat,
      .blend = &blendState,
      .writeMask = WGPUColorWriteMask_All,
  };

  WGPUFragmentState fragmentState{
      .nextInChain = nullptr,
      .module = shaderModule,
      .entryPoint = "fs_main",
      .constantCount = 0,
      .constants = nullptr,
      .targetCount = 1,
      .targets = &colorTarget,
  };

  WGPUPipelineLayoutDescriptor layoutDesc = {
      .nextInChain = nullptr,
      .bindGroupLayoutCount = 0,
      .bindGroupLayouts = nullptr,
  };

  WGPURenderPipelineDescriptor pipelineDesc{
      .layout = wgpuDeviceCreatePipelineLayout(device, &layoutDesc),
      .vertex =
          {
              .module = shaderModule,
              .entryPoint = "vs_main",
              .constantCount = 0,
              .constants = nullptr,
              .bufferCount = 1,
              .buffers = &vertexBufferLayout,
          },
      .primitive =
          {
              .topology = WGPUPrimitiveTopology_TriangleList,
              .stripIndexFormat = WGPUIndexFormat_Undefined,
              .frontFace = WGPUFrontFace_CCW,
              .cullMode = WGPUCullMode_None,
          },
      .depthStencil = nullptr,
      .multisample =
          {
              .count = 1,
              .mask = ~0u,
              .alphaToCoverageEnabled = false,
          },
      .fragment = &fragmentState,
  };

  WGPURenderPipeline pipeline =
      wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);
  std::cout << "Render pipeline: " << pipeline << std::endl;

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    WGPUTextureView nextTexture = wgpuSwapChainGetCurrentTextureView(swapChain);
    if (nextTexture == nullptr) {
      continue;
    }
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
        .clearValue =
            WGPUColor{
                0.0,
                0.0,
                0.0,
                1.0,
            },
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

    wgpuRenderPassEncoderSetPipeline(renderPass, pipeline);
    wgpuRenderPassEncoderSetVertexBuffer(renderPass, 0, buffer, 0,
                                         vertexData.size() * sizeof(float));
    wgpuRenderPassEncoderDraw(renderPass, 3, 1, 0, 0);

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

  wgpuBufferDestroy(buffer);

  return EXIT_SUCCESS;
}
