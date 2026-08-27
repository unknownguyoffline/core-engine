#include "Shader.hpp"
#include "Renderer.hpp"

void Shader::Load(std::string_view vertexFilename, std::string_view fragmentFilename, const RenderPass &renderPass, uint32_t subpass)
{
    Load(vertexFilename, fragmentFilename, "", "", renderPass, subpass);
}

void Shader::Load(std::string_view vertexFilename, std::string_view fragmentFilename, std::string_view geometryFilename, std::string_view tessellationFilename, const RenderPass &renderPass, uint32_t subpass)
{
    if (!vertexFilename.empty())
    {
        mVertexFilename = vertexFilename;
        mVertexShaderModule = CreateShaderFromFile(GraphicsContext::GetCurrentContext().GetDevice(), vertexFilename.data());
        mGraphicsPipeline.SetVertexShader(mVertexShaderModule);
    }

    if (!fragmentFilename.empty())
    {
        mFragmentFilename = fragmentFilename;
        mFragmentShaderModule = CreateShaderFromFile(GraphicsContext::GetCurrentContext().GetDevice(), fragmentFilename.data());
        mGraphicsPipeline.SetFragmentShader(mFragmentShaderModule);
    }

    if (!geometryFilename.empty())
    {
        mGeometryFilename = geometryFilename;
        mGeometryShaderModule = CreateShaderFromFile(GraphicsContext::GetCurrentContext().GetDevice(), geometryFilename.data());
        mGraphicsPipeline.SetGeometryShader(mGeometryShaderModule);
    }

    if (!tessellationFilename.empty())
    {
        mTessellationFilename = tessellationFilename;
        mTessellationShaderModule = CreateShaderFromFile(GraphicsContext::GetCurrentContext().GetDevice(), tessellationFilename.data());
        mGraphicsPipeline.SetTessellationShader(mTessellationShaderModule);
    }

    SetupPipelineSettings(mSettings);
    mGraphicsPipeline.CreatePipeline(renderPass, subpass);
}

void Shader::Create(const std::vector<uint32_t> &vertexCode, const std::vector<uint32_t> &fragmentCode, const RenderPass &renderPass, uint32_t subpass)
{
    Create(vertexCode, fragmentCode, {}, {});
}

void Shader::Create(const std::vector<uint32_t> &vertexCode, const std::vector<uint32_t> &fragmentCode, const std::vector<uint32_t> &geometryCode, const std::vector<uint32_t> &tessellationCode, const RenderPass &renderPass, uint32_t subpass)
{
    if (!vertexCode.empty())
    {
        mVertexShaderModule = CreateShaderModuleFromMemory(GraphicsContext::GetCurrentContext().GetDevice(), vertexCode);
        mGraphicsPipeline.SetVertexShader(mVertexShaderModule);
    }

    if (!fragmentCode.empty())
    {
        mFragmentShaderModule = CreateShaderModuleFromMemory(GraphicsContext::GetCurrentContext().GetDevice(), fragmentCode);
        mGraphicsPipeline.SetFragmentShader(mFragmentShaderModule);
    }

    if (!geometryCode.empty())
    {
        mGeometryShaderModule = CreateShaderModuleFromMemory(GraphicsContext::GetCurrentContext().GetDevice(), geometryCode);
        mGraphicsPipeline.SetGeometryShader(mGeometryShaderModule);
    }

    if (!tessellationCode.empty())
    {
        mTessellationShaderModule = CreateShaderModuleFromMemory(GraphicsContext::GetCurrentContext().GetDevice(), tessellationCode);
        mGraphicsPipeline.SetTessellationShader(mTessellationShaderModule);
    }

    SetupPipelineSettings(mSettings);
    mGraphicsPipeline.CreatePipeline(renderPass, subpass);
}

void Shader::Destroy()
{
    vkDestroyShaderModule(GraphicsContext::GetCurrentContext().GetDevice(), mVertexShaderModule, nullptr);
    vkDestroyShaderModule(GraphicsContext::GetCurrentContext().GetDevice(), mFragmentShaderModule, nullptr);
    vkDestroyShaderModule(GraphicsContext::GetCurrentContext().GetDevice(), mGeometryShaderModule, nullptr);
    vkDestroyShaderModule(GraphicsContext::GetCurrentContext().GetDevice(), mTessellationShaderModule, nullptr);

    mDescriptors.clear();

    mGraphicsPipeline.DestroyPipeline();
}

void Shader::AddDescriptor()
{
}

const GraphicsPipeline &Shader::GetGraphicsPipeline() const
{
    return mGraphicsPipeline;
}

const std::string &Shader::GetVertexFilename() const
{
    return mVertexFilename;
}
const std::string &Shader::GetFragmentFilename() const
{
    return mFragmentFilename;
}
const std::string &Shader::GetGeometryFilename() const
{
    return mGeometryFilename;
}
const std::string &Shader::GetTessellationFilename() const
{
    return mTessellationFilename;
}

void Shader::SetupPipelineSettings(const ShaderSettings &settings)
{
    mGraphicsPipeline.SetCullMode(settings.cullMode);
    mGraphicsPipeline.SetSampleCount(settings.sampleCount);
    mGraphicsPipeline.EnableDepthTesting(settings.enableDepthTest);
    mGraphicsPipeline.EnableDepthWrite(settings.enableDepthWrite);
    mGraphicsPipeline.SetPrimitive(settings.primitive);
    mGraphicsPipeline.SetCompareOp(settings.compare);
}

void Shader::AddColorBlendAttachment()
{
}
void Shader::AddLayout(const VertexLayout &layout)
{
    for (const VertexAttribute &attribute : layout.attributes)
    {
        mGraphicsPipeline.AddAttribute(attribute.binding, attribute.location, attribute.format, attribute.offset);
    }

    for (const VertexBinding &binding : layout.bindings)
    {
        mGraphicsPipeline.AddBinding(binding.binding, binding.stride, binding.inputRate);
    }
}
void Shader::SetPushConstantSize(size_t size)
{
    mGraphicsPipeline.SetPushConstant(ShaderStage::All, size);
}
void Shader::SetDepthBias(bool enable, float slopeFactor, float constantFactor)
{
    mGraphicsPipeline.SetDepthBias(enable, slopeFactor, constantFactor);
}
const ShaderSettings &Shader::GetSettings() const
{
    return mSettings;
}
ShaderSettings &Shader::GetSettings()
{
    return mSettings;
}
