#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>
#include <webgpu/webgpu.h>
#include <functional>
#include <iostream>
#include <vector>
#include "webgpu-release.h"

WGPUAdapter requestAdapter(WGPUInstance instance,
                           WGPURequestAdapterOptions const* options) {
  struct UserData {
    WGPUAdapter adapter = nullptr;
    bool requestEnded = false;
  };
  UserData userData;

  auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status,
                                  WGPUAdapter adapter, char const* message,
                                  void* pUserData) {
    UserData& userData = *reinterpret_cast<UserData*>(pUserData);
    if (status == WGPURequestAdapterStatus_Success) {
      userData.adapter = adapter;
    } else {
      std::cout << "Could not get WebGPU adapter: " << message << std::endl;
    }
    userData.requestEnded = true;
  };

  // Call to the WebGPU request adapter procedure
  wgpuInstanceRequestAdapter(instance /* equivalent of navigator.gpu */,
                             options, onAdapterRequestEnded, (void*)&userData);

  return userData.adapter;
}

bool EnterGLFW(std::function<int(GLFWwindow*)> inner) {
  if (!glfwInit()) {
    std::cerr << "Could not initialize GLFW!" << std::endl;
    return 1;
  }
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow* window = glfwCreateWindow(640, 480, "Learn WebGPU", NULL, NULL);

  int ret = inner(window);

  //while (!glfwWindowShouldClose(window)) {
    //// Check whether the user clicked on the close button (and any other
    //// mouse/key event, which we don't use so far)
    //glfwPollEvents();
  //}
  // [...] Use the window
  glfwDestroyWindow(window);

  glfwTerminate();

  return ret;
}

int EnterWebGPU(GLFWwindow* window) {
  // 1. We create a descriptor
  WGPUInstanceDescriptor desc = {};
  desc.nextInChain = nullptr;

  // 2. We create the instance using this descriptor
  std::cout << "Requesting WGPU instance" << std::endl;
  WGPUInstance instance = wgpuCreateInstance(&desc);

  // 3. We can check whether there is actually an instance created
  if (!instance) {
    std::cerr << "Could not initialize WebGPU!" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "WGPU instance: " << instance << std::endl;

  WGPUSurface surface = glfwGetWGPUSurface(instance, window);

  std::cout << "Requesting adapter..." << std::endl;
  WGPURequestAdapterOptions adapterOpts = {
      .nextInChain = nullptr,
      .compatibleSurface = surface,
  };
  WGPUAdapter adapter = requestAdapter(instance, &adapterOpts);
  std::cout << "Got adapter: " << adapter << std::endl;

  std::vector<WGPUFeatureName> features;
  size_t featureCount = wgpuAdapterEnumerateFeatures(adapter, nullptr);
  features.resize(featureCount);
  wgpuAdapterEnumerateFeatures(adapter, features.data());
  std::cout << "Adapter features:" << std::endl;
  for (auto f : features) {
    std::cout << " - " << f << std::endl;
  }

  wgpuAdapterRelease(adapter);

  return EXIT_SUCCESS;
}

int main(int, char**) {
  return EnterGLFW(EnterWebGPU);
}
