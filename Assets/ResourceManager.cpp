#include "ResourceManager.hpp"

FontManager &ResourceManager::
    GetFontManager()
{
    return mFontManager;
}

MeshManager &ResourceManager::GetMeshManager()
{
    return mMeshManager;
}

MaterialManager &ResourceManager::GetMaterialManager()
{
    return mMaterialManager;
}

TextureManager &ResourceManager::GetTextureManager()
{
    return mTextureManager;
}

ShaderManager &ResourceManager::GetShaderManager()
{
    return mShaderManager;
}

const FontManager &ResourceManager::GetFontManager() const
{
    return mFontManager;
}

const MeshManager &ResourceManager::GetMeshManager() const
{
    return mMeshManager;
}

const MaterialManager &ResourceManager::GetMaterialManager() const
{
    return mMaterialManager;
}

const TextureManager &ResourceManager::GetTextureManager() const
{
    return mTextureManager;
}

const ShaderManager &ResourceManager::GetShaderManager() const
{
    return mShaderManager;
}

void ResourceManager::Initialize()
{
    mTextureManager.Initialize();
}

void ResourceManager::Terminate()
{
}
