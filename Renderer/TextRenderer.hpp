#pragma once
#include "Assets/Font.hpp"
#include "Assets/FontManager.hpp"
#include "Assets/MeshManager.hpp"
#include "Assets/ShaderManager.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/GraphicsPipeline.hpp"
#include "Renderer/InstanceBuffer.hpp"
#include "Renderer/Transform.hpp"
#include "Renderer/UniformBuffer.hpp"
#include <string>

struct TextInstanceData
{
    glm::mat4 model = glm::mat4(1.f);
    glm::vec4 forgroundColor = glm::vec4(1.0);
    glm::vec4 backgroundColor = glm::vec4(1.0);
    uint32_t startIndex = 0;
    uint32_t count = 0;

    static VertexLayout GetLayout(uint32_t binding, uint32_t startLocation)
    {
        VertexLayout layout;
        layout.attributes.emplace_back(binding, startLocation + 0, offsetof(TextInstanceData, model) + (sizeof(glm::vec4) * 0), ImageFormat::RGBA32);
        layout.attributes.emplace_back(binding, startLocation + 1, offsetof(TextInstanceData, model) + (sizeof(glm::vec4) * 1), ImageFormat::RGBA32);
        layout.attributes.emplace_back(binding, startLocation + 2, offsetof(TextInstanceData, model) + (sizeof(glm::vec4) * 2), ImageFormat::RGBA32);
        layout.attributes.emplace_back(binding, startLocation + 3, offsetof(TextInstanceData, model) + (sizeof(glm::vec4) * 3), ImageFormat::RGBA32);
        layout.attributes.emplace_back(binding, startLocation + 4, offsetof(TextInstanceData, forgroundColor), ImageFormat::RGBA32);
        layout.attributes.emplace_back(binding, startLocation + 5, offsetof(TextInstanceData, backgroundColor), ImageFormat::RGBA32);
        layout.attributes.emplace_back(binding, startLocation + 6, offsetof(TextInstanceData, startIndex), ImageFormat::R32U);
        layout.attributes.emplace_back(binding, startLocation + 7, offsetof(TextInstanceData, count), ImageFormat::RG32U);
        layout.bindings.emplace_back(binding, sizeof(TextInstanceData), InputRate::Instance);

        return layout;
    }
};

struct TextUniformData
{
    glm::mat4 view;
    glm::mat4 projection;
};

struct TextPushConstant
{
    int mode = 0;
};

struct TextProperty
{
    float spacing = 1.f;
    glm::vec4 forgroundColor = glm::vec4(1);
    glm::vec4 backgroundColor = glm::vec4(0);
    Transform transform;
};

class TextRenderer
{
public:
    static void Initialize();
    static void Terminate();

    static void DrawText(const Font &font, const std::string &text, float spacing = 1.f, const glm::vec4 &forgroundColor = glm::vec4(1), const glm::vec4 &backgroundColor = glm::vec4(1), const Transform &transform = {});
    static void DrawCharacter(const Font &font, char ch, const glm::vec3 &position, const glm::vec4 &forgroundColor = glm::vec4(1), const glm::vec4 &backgroundColor = glm::vec4(0), const Transform &transform = {});

    static void DrawText(const Font &font, const std::string &text, const TextProperty &property);
    static void DrawCharacter(const Font &font, char ch, const glm::vec3 &position, const TextProperty &property);

    static void DrawText(const Font &font, const std::string &text, const std::function<TextProperty(char ch, uint32_t index, const glm::vec2 &position, float totalSize)> &callback);

    static void SetCamera(const Camera &camera);
    static void SetSpacing(float spacing);
    static void Flush();

    static TextPushConstant &GetPushConstant()
    {
        return mPushConstant;
    }

private:
    static GraphicsPipeline mTextPipeline;
    static UniformBuffer mUniformBuffer;
    static Descriptor mUniformDescriptor;
    static Descriptor mBezierDescriptor;
    static TextUniformData mUniformData;

    static Camera mCamera;
    static Shader mShader;

    static Buffer mVertexBuffer;
    static Buffer mIndexBuffer;

    static Buffer mQuadVertexBuffer;
    static Buffer mQuadIndexBuffer;

    static InstanceBuffer mInstanceBuffer;

    static std::vector<TextInstanceData> mInstanceData;

    static TextPushConstant mPushConstant;
};
