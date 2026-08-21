#pragma once
#include "ModelImporter.hpp"

class GLTFImporter : public ModelImporter
{
public:
    GLTFImporter()
    {
    }
    void Import(std::string_view filename, Scene &scene) override;
};

class GLBImporter : public ModelImporter
{
public:
    void Import(std::string_view filename, Scene &scene) override;
};