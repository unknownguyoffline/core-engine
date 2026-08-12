#include "TextureManager.hpp"

void TextureManager::Initialize()
{
    mSampler.CreateSampler();
}

void TextureManager::Terminate()
{
}

std::string TextureManager::LoadTexture(std::string_view identifier, std::string_view filename, ImageFormat format)
{
    Texture texture;
    texture.Load(filename, format);

    mTextureMap[identifier.data()] = texture;

    uint32_t index = mTextureDescriptorIndex.size();
    mTextureDescriptorIndex[identifier.data()] = index;

    return identifier.data();
}

std::string TextureManager::CreateTexture(std::string_view identifier, void *data, const glm::uvec2 &size, ImageFormat format, Filter minFilter, Filter magFilter, AddressMode addressMode)
{
    Texture texture;
    texture.Create(data, size, format, minFilter, magFilter, addressMode);

    mTextureMap[identifier.data()] = texture;

    uint32_t index = mTextureDescriptorIndex.size();
    mTextureDescriptorIndex[identifier.data()] = index;

    return identifier.data();
}

void TextureManager::DestroyTexture(std::string_view identifier)
{
    mTextureMap[identifier.data()].Destroy();
}

const Texture &TextureManager::GetTexture(std::string_view identifier)
{
    return mTextureMap.at(identifier.data());
}

Texture &TextureManager::GetTextureRef(std::string_view identifier)
{
    return mTextureMap.at(identifier.data());
}

bool TextureManager::HasTexture(std::string_view identifier)
{
    return mTextureMap.contains(identifier.data());
}

uint32_t TextureManager::GetCount()
{
    return mTextureMap.size();
}
const std::unordered_map<std::string, Texture> &TextureManager::GetMap() const
{
    return mTextureMap;
}

void TextureManager::SetTextureDescriptor(const Descriptor &descriptor)
{
    uint32_t index = 0;
    for (const auto &[id, texture] : mTextureMap)
    {
        descriptor.UpdateImageIndex(texture.GetImage(), ImageLayout::ShaderRead, texture.GetSampler(), 0, index);
        mTextureDescriptorIndex[id] = index;
        index++;
    }
}

uint32_t TextureManager::GetTextureDescriptorIndex(std::string_view identifier) const
{
    if (identifier.size() == 0)
        return UINT32_MAX;

    return mTextureDescriptorIndex.at(identifier.data());
}
void TextureManager::Clear()
{
    mTextureMap.clear();
}

// std::unordered_map<std::string, Texture> TextureManager::mTextureMap;
// Sampler TextureManager::mSampler;
// Descriptor TextureManager::mDescriptor;
// std::unordered_map<std::string, uint32_t> TextureManager::mTextureDescriptorIndex;
