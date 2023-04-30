#include "device.hpp"
#include <iostream>
#include <cassert>

WGPUDevice requestDevice(const WGPUAdapter& adapter,
                         WGPUDeviceDescriptor const* descriptor) {
  struct UserData {
    WGPUDevice device = nullptr;
    bool requestEnded = false;
  };
  UserData userData;

  auto onDeviceRequestEnded = [](WGPURequestDeviceStatus status,
                                 WGPUDevice device, char const* ,
                                 void* pUserData) {
    UserData& userData = *reinterpret_cast<UserData*>(pUserData);
    if (status == WGPURequestDeviceStatus_Success) {
      userData.device = device;
    }
    userData.requestEnded = true;
  };

  wgpuAdapterRequestDevice(adapter, descriptor, onDeviceRequestEnded,
                           (void*)&userData);

  assert(userData.requestEnded);

  return userData.device;
}

Device::Device(const WGPUAdapter& adapter) {
  WGPUDeviceDescriptor deviceDesc = {
      .nextInChain = nullptr,
      .label = "My Device",
      .requiredFeaturesCount = 0,
      .requiredLimits = nullptr,
      .defaultQueue =
          {
              .nextInChain = nullptr,
              .label = "The default queue",
          },
  };

  device_ = requestDevice(adapter, &deviceDesc);

  auto onDeviceError = [](WGPUErrorType /*type*/, char const* message,
                          void* /* pUserData */) {
    if (message)
      std::cout << " (" << message << ")";
    std::cout << std::endl;
  };
  wgpuDeviceSetUncapturedErrorCallback(device_, onDeviceError,
                                       nullptr /* pUserData */);
}

Device::~Device() {}
