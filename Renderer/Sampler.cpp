#include "Sampler.hpp"
#include "Renderer/Converter.hpp"
#include "Renderer/GraphicsContext.hpp"
#include <vulkan/vulkan_core.h>

void Sampler::SetFilter(Filter minification, Filter magnification)
{
    mCreateInfo.minFilter = GetVulkanFilter(minification);
    mCreateInfo.magFilter = GetVulkanFilter(magnification);
}
void Sampler::SetAddressMode(AddressMode u, AddressMode v, AddressMode w)
{
    mCreateInfo.addressModeU = GetVulkanAddressMode(u);
    mCreateInfo.addressModeV = GetVulkanAddressMode(v);
    mCreateInfo.addressModeW = GetVulkanAddressMode(w);
}
void Sampler::SetBorderColor(const glm::vec4 &color)
{
}

void Sampler::CreateSampler()
{
    vkCreateSampler(GraphicsContext::GetCurrentContext().GetDevice(), &mCreateInfo, nullptr, &mHandle);
}
void Sampler::DestroySampler()
{
    vkDestroySampler(GraphicsContext::GetCurrentContext().GetDevice(), mHandle, nullptr);
}

void Sampler::EnableCompare(bool enable, CompareType compareType)
{
    mCreateInfo.compareEnable = enable;
    mCreateInfo.compareOp = GetVulkanCompareType(compareType);
}
