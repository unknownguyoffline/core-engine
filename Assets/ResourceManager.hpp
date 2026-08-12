#pragma once
#include "Assets/FontManager.hpp"
#include "Assets/MaterialManager.hpp"
#include "Assets/MeshManager.hpp"

class ResourceManager
{
public:
    FontManager &GetFontManager();
    MeshManager &GetMeshManager();
    MaterialManager &GetMaterialManager();
    TextureManager &GetTextureManager();
    ShaderManager &GetShaderManager();
    const FontManager &GetFontManager() const;
    const MeshManager &GetMeshManager() const;
    const MaterialManager &GetMaterialManager() const;
    const TextureManager &GetTextureManager() const;
    const ShaderManager &GetShaderManager() const;

    void Initialize();
    void Terminate();

private:
    FontManager mFontManager;
    MeshManager mMeshManager;
    MaterialManager mMaterialManager;
    TextureManager mTextureManager;
    ShaderManager mShaderManager;
};