#pragma once

#include <string>
#include <vector>

namespace exqudens::vulkan {

  struct Configuration {

    std::string applicationName;
    unsigned int applicationVersionMajor;
    unsigned int applicationVersionMinor;
    unsigned int applicationVersionPatch;
    std::string engineName;
    unsigned int engineVersionMajor;
    unsigned int engineVersionMinor;
    unsigned int engineVersionPatch;
    bool validationLayersEnabled;
    std::vector<const char*> validationLayers;
    std::vector<const char*> extensions;
    std::vector<const char*> deviceExtensions;
    bool computeQueueFamilyRequired;
    bool transferQueueFamilyRequired;
    bool graphicsQueueFamilyRequired;
    bool presentQueueFamilyRequired;
    bool anisotropyRequired;

  };

}
