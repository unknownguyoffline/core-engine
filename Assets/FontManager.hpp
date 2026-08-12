#pragma once
#include "Assets/Font.hpp"
#include "Assets/FontImporter.hpp"
#include <cstdint>
#include <string_view>

class FontManager
{
public:
    std::string Load(std::string_view filename, std::string_view identifier);
    void Destroy(std::string_view identifier);
    const std::unordered_map<std::string, Font> &GetMap() const;
    const Font &GetFont(std::string_view identifier) const;
    bool HasFont(std::string_view identifier);
    void Clear();

private:
    std::unordered_map<std::string, Font> mFontMap;
    FontImporter mImporter;
    uint64_t mLastId;
};