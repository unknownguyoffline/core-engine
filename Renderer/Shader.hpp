#pragma once
#include "Renderer/GraphicsPipeline.hpp"
#include "Vertex.hpp"
#include <string_view>
#include <vector>
#include <vulkan/vulkan_core.h>

struct ShaderSettings
{
    SampleCount sampleCount = SampleCount::One;
    CullMode cullMode = CullMode::Back;
    bool enableDepthWrite = false;
    bool enableDepthTest = false;
    PrimitiveType primitive = PrimitiveType::Triangle;
    CompareType compare = CompareType::Less;
};

class Shader
{
public:
    void Load(std::string_view vertexFilename, std::string_view fragmentFilename, const RenderPass &renderPass, uint32_t subpass);
    void Load(std::string_view vertexFilename, std::string_view fragmentFilename, std::string_view geometryFilename, std::string_view tesellationFilename, const RenderPass &renderPass, uint32_t subpass);
    void Create(const std::vector<uint32_t> &vertexCode, const std::vector<uint32_t> &fragmentCode, const RenderPass &renderPass, uint32_t subpass);
    void Create(const std::vector<uint32_t> &vertexCode, const std::vector<uint32_t> &fragmentCode, const std::vector<uint32_t> &geometryCode, const std::vector<uint32_t> &tessellationCode, const RenderPass &renderPass, uint32_t subpass);

    void Destroy();

    template <typename... Descriptors>
    void AddDescriptor(const Descriptor &descriptor, const Descriptors... descriptors);
    void AddDescriptor();

    template <typename... EnableColorBlend>
    void AddColorBlendAttachment(bool enableBlending, EnableColorBlend... colorBlend);
    void AddColorBlendAttachment();

    void AddLayout(const VertexLayout &layout);

    void SetPushConstantSize(size_t size);

    void SetDepthBias(bool enable, float slopeFactor, float constantFactor);

    const ShaderSettings &GetSettings() const;
    ShaderSettings &GetSettings();
    const GraphicsPipeline &GetGraphicsPipeline() const;

    const std::string &GetVertexFilename() const;
    const std::string &GetFragmentFilename() const;
    const std::string &GetGeometryFilename() const;
    const std::string &GetTessellationFilename() const;

private:
    void SetupPipelineSettings(const ShaderSettings &settings);

    GraphicsPipeline mGraphicsPipeline;

    ShaderSettings mSettings;

    VkShaderModule mVertexShaderModule = VK_NULL_HANDLE;
    VkShaderModule mFragmentShaderModule = VK_NULL_HANDLE;
    VkShaderModule mGeometryShaderModule = VK_NULL_HANDLE;
    VkShaderModule mTessellationShaderModule = VK_NULL_HANDLE;

    std::string mVertexFilename;
    std::string mFragmentFilename;
    std::string mGeometryFilename;
    std::string mTessellationFilename;

    std::vector<const Descriptor *> mDescriptors;
};

template <typename... Descriptors>
inline void Shader::AddDescriptor(const Descriptor &descriptor, const Descriptors... descriptors)
{
    mGraphicsPipeline.AddDescriptors(descriptor);
    mDescriptors.push_back(&descriptor);
    AddDescriptor(descriptors...);
}
template <typename... EnableColorBlend>
inline void Shader::AddColorBlendAttachment(bool enableBlending, EnableColorBlend... colorBlend)
{
    mGraphicsPipeline.AddColorBlendAttachment(enableBlending);
    AddColorBlendAttachment(colorBlend...);
}
