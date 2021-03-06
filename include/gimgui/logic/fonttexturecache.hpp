#pragma once
#include <cstdint>
#include <memory>
#include <functional>
#include <unordered_map>
#include <gimgui/data/font.hpp>
#include <gimgui/data/codepointsizeid.hpp>
#include <gimgui/data/texturecoordinates.hpp>
#include <gimgui/logic/rectanglepacker.hpp>

namespace gim
{
    namespace internal
    {
        class FontTextureCache
        {
            struct Vec2
            {
                int32_t x;
                int32_t y;
            };
            public:
                template <typename TextureAdaptor>
                FontTextureCache(TextureAdaptor textureAdaptor);
                std::unique_ptr<TextureCoordinates> glyphCoords(uint32_t codePoint, uint32_t size, uint32_t fontId);
                TextureCoordinates add(const Glyph& glyph, uint32_t fontId);
                TextureCoordinates solidCoords() const;
            private:
                BitMap flipBitMap(const BitMap& bitMap) const;
                std::function<void(uint32_t, uint32_t)> mResizeStorage;
                std::function<void(uint32_t, uint32_t, const BitMap&)> mWriteBitmap;

                RectanglePacker mGlyphPacker;
                TextureCoordinates generateTexCoords(const Rectangle, bool flipped) const;
                std::unordered_map<CodePointSizeId, std::pair<Rectangle, bool>> mGlyphRectangles;
                uint32_t mCurrentSize;
                Rectangle mSolidCoords;
        };
#include <gimgui/logic/fonttexturecache.inl>
    }
}
