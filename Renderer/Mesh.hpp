#pragma once
#include "Utility.hpp"
#include "Vertex.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

struct Vertex
{
    glm::vec3 position = glm::vec3(0);
    glm::vec2 uv = glm::vec3(0);
    glm::vec3 normal = glm::vec3(0);

    Vertex(glm::vec3 position, glm::vec2 uv, glm::vec3 normal)
        : position(position), uv(uv), normal(normal)
    {
    }
    Vertex() = default;

    static VertexLayout GetVertexLayout(uint32_t binding, uint32_t startLocation)
    {
        VertexLayout layout;

        layout.attributes.emplace_back(binding, 0 + startLocation, offsetof(Vertex, position), ImageFormat::RGB32);
        layout.attributes.emplace_back(binding, 1 + startLocation, offsetof(Vertex, uv), ImageFormat::RG32);
        layout.attributes.emplace_back(binding, 2 + startLocation, offsetof(Vertex, normal), ImageFormat::RGB32);

        layout.bindings.emplace_back(binding, sizeof(Vertex), InputRate::Vertex);

        return layout;
    }
};

class Mesh
{
public:
    Mesh();
    Mesh(void *vertices, size_t vertexSize, uint32_t *indices, size_t indexSize);
    Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);

    void SetData(const void *vertices, size_t vertexSize, const uint32_t *indices, size_t indexSize);
    void SetData(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);

    void *GetVertexData() const
    {
        return mStagingVertexBuffer.map;
    }

    void *GetIndexData() const
    {
        return mStagingIndexBuffer.map;
    }

    bool IsValid() const;

    void Destroy();

    const Buffer &GetVertexBuffer() const;
    const Buffer &GetIndexBuffer() const;

    const std::string &GetName() const;
    void SetName(const std::string &name);

    static void Initialize();

    size_t GetVertexSize() const
    {
        return mVertexSize;
    }

    size_t GetIndexSize() const
    {
        return mIndexSize;
    }

    bool IsStandardMesh() const
    {
        return mStandardMesh;
    }

    const glm::vec3 &GetMinVertex() const
    {
        return mMinVertex;
    }

    const glm::vec3 &GetMaxVertex() const
    {
        return mMaxVertex;
    }

    const glm::vec3 &GetCenter() const
    {
        return mCenter;
    }

private:
    std::string mName;

    friend class Renderer;

    size_t mVertexSize = 0;
    size_t mIndexSize = 0;

    Buffer mStagingVertexBuffer;
    Buffer mStagingIndexBuffer;

    Buffer mVertexBuffer;
    Buffer mIndexBuffer;

    glm::vec3 mMinVertex = glm::vec3(FLT_MAX);
    glm::vec3 mMaxVertex = glm::vec3(FLT_MIN);
    glm::vec3 mCenter = glm::vec3(0);

    bool mStandardMesh = false;
    bool mIsValid = false;
};
