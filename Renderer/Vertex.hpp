#pragma once
#include "Types.hpp"
#include <cstddef>
#include <cstdint>

struct VertexAttribute
{
    uint32_t binding = 0;
    uint32_t location = 0;
    size_t offset = 0;
    ImageFormat format = ImageFormat::RGB32;

    VertexAttribute() = default;
    VertexAttribute(uint32_t binding, uint32_t location, size_t offset, ImageFormat format)
        : binding(binding), location(location), offset(offset), format(format)
    {
    }
};

struct VertexBinding
{
    uint32_t binding = 0;
    size_t stride = 0;
    InputRate inputRate;
    VertexBinding() = default;
    VertexBinding(uint32_t binding, size_t stride, InputRate inputRate)
        : binding(binding), stride(stride), inputRate(inputRate)
    {
    }
};

struct VertexLayout
{
    std::vector<VertexAttribute> attributes;
    std::vector<VertexBinding> bindings;
};