#pragma once
#include "EntityComponentSystem/EntityComponentSystem.hpp"

class ModelImporter
{
public:
    virtual void Import(std::string_view filename, Scene &scene) = 0;

private:
};

class AssimpImporter : public ModelImporter
{
public:
    void Import(std::string_view filename, Scene &scene) override;
};
