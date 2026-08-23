#include "ShaderManager.hpp"
#include "Renderer/GraphicsContext.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Utility.hpp"

std::string ShaderManager::Load(std::string_view identifier, std::string_view vertexFile, std::string_view fragmentFile, std::string_view geometryFile, std::string_view tessellationFile, std::function<void(Shader &shader)> setupCallback)
{
    Shader &shader = mShaderMap[identifier.data()];
    setupCallback(shader);
    shader.Load(vertexFile, fragmentFile, geometryFile, tessellationFile, Renderer::GetRenderPass(), 0);
    return identifier.data();
}
std::string ShaderManager::Load(std::string_view identifier, std::string_view vertexFile, std::string_view fragmentFile, std::function<void(Shader &shader)> setupCallback)
{
    return Load(identifier, vertexFile, fragmentFile, "", "", setupCallback);
}
std::string ShaderManager::Create(std::string_view identifier, const std::vector<uint32_t> &vertexCode, const std::vector<uint32_t> &fragmentCode, const std::vector<uint32_t> &geometryCode, const std::vector<uint32_t> &tessellationCode, std::function<void(Shader &shader)> setupCallback)
{
    Shader &shader = mShaderMap[identifier.data()];
    setupCallback(shader);
    shader.Create(vertexCode, fragmentCode, geometryCode, tessellationCode, Renderer::GetRenderPass(), 0);
    return identifier.data();
}
std::string ShaderManager::Create(std::string_view identifier, const std::vector<uint32_t> &vertexCode, const std::vector<uint32_t> &fragmentCode, std::function<void(Shader &shader)> setupCallback)
{
    return Create(identifier, vertexCode, fragmentCode, {}, {}, setupCallback);
}

Shader &ShaderManager::Get(std::string_view identifier)
{
    return mShaderMap.at(identifier.data());
}

const Shader &ShaderManager::Get(std::string_view identifier) const
{
    return mShaderMap.at(identifier.data());
}

bool ShaderManager::Has(std::string_view identifier)
{
    return mShaderMap.contains(identifier.data());
}

const std::unordered_map<std::string, Shader> &ShaderManager::GetMap() const
{
    return ShaderManager::mShaderMap;
}
const BuiltinShaderIdentifier &ShaderManager::GetBuiltinIdentifier()
{
    return mBuiltinShaderIdentifier;
}

// uint64_t ShaderManager::mLastShaderId = 0;
// std::unordered_map<std::string, Shader> ShaderManager::mShaderMap;
// BuiltinShaderIdentifier ShaderManager::mBuiltinShaderIdentifier;