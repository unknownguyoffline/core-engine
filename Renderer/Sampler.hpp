#pragma once
#include "Renderer/Types.hpp"
#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

class Sampler
{
public:
    void SetFilter(Filter minification, Filter magnification);
    void SetAddressMode(AddressMode u, AddressMode v, AddressMode w);
    void SetBorderColor(const glm::vec4 &color);
    void CreateSampler();
    void DestroySampler();
    void EnableCompare(bool enable, CompareType compareType);

    VkSampler GetHandle() const
    {
        return mHandle;
    }

private:
    VkSampler mHandle = VK_NULL_HANDLE;
    VkSamplerCreateInfo mCreateInfo =
        {
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .magFilter = VK_FILTER_NEAREST,
            .minFilter = VK_FILTER_NEAREST,
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .minLod = 1,
            .maxLod = 1,
            .borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
    };
};
