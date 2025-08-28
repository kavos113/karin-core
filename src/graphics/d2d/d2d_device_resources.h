#ifndef SRC_GRAPHICS_GRAPHICS_D2D_DEVICE_RESOURCES_H
#define SRC_GRAPHICS_GRAPHICS_D2D_DEVICE_RESOURCES_H
#include <d2d1_1.h>
#include <dwrite.h>
#include <map>
#include <unordered_map>
#include <wrl/client.h>

#include <karin/graphics/stroke_style.h>
#include <karin/graphics/pattern.h>
#include <karin/graphics/image.h>
#include <karin/graphics/text_layout.h>

#include <path_impl.h>

namespace karin
{
class D2DDeviceResources
{
public:
    explicit D2DDeviceResources(
        Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext,
        Microsoft::WRL::ComPtr<ID2D1Factory1> factory,
        Microsoft::WRL::ComPtr<IDWriteFactory> dwriteFactory
    )
        : m_deviceContext(std::move(deviceContext)),
          m_factory(std::move(factory)),
          m_dwriteFactory(std::move(dwriteFactory))
    {
    }

    ~D2DDeviceResources() = default;

    void clear();

    Image createImage(const std::vector<std::byte>& data, uint32_t width, uint32_t height);

    Microsoft::WRL::ComPtr<ID2D1Brush> brush(Pattern& pattern);
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> solidColorBrush(const SolidColorPattern& pattern);
    Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> linearGradientBrush(const LinearGradientPattern& pattern);
    Microsoft::WRL::ComPtr<ID2D1RadialGradientBrush> radialGradientBrush(const RadialGradientPattern& pattern);
    Microsoft::WRL::ComPtr<ID2D1BitmapBrush> bitmapBrush(const ImagePattern& pattern);
    Microsoft::WRL::ComPtr<ID2D1StrokeStyle> strokeStyle(const StrokeStyle& style);
    Microsoft::WRL::ComPtr<ID2D1PathGeometry> pathGeometry(const PathImpl& path);
    Microsoft::WRL::ComPtr<ID2D1Bitmap> bitmap(const Image& image);
    Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout(const TextLayout& layout);

private:
    static D2D1_CAP_STYLE toD2DCapStyle(StrokeStyle::CapStyle capStyle);
    static D2D1_LINE_JOIN toD2DJoinStyle(StrokeStyle::JoinStyle joinStyle);
    static D2D1_EXTEND_MODE toD2DExtendMode(ExtendMode extendMode);
    static DWRITE_TEXT_ALIGNMENT toDWriteTextAlignment(TextFormat::HorizontalAlignment alignment);
    static DWRITE_PARAGRAPH_ALIGNMENT toDWriteParagraphAlignment(TextFormat::VerticalAlignment alignment);
    static DWRITE_WORD_WRAPPING toDWriteWordWrapping(TextFormat::Wrapping wordWrapping);
    static DWRITE_TRIMMING_GRANULARITY toDWriteTrimmingGranularity(TextFormat::Trimming trimming);
    static DWRITE_FLOW_DIRECTION toDWriteFlowDirection(TextFormat::Direction flowDirection);
    static DWRITE_READING_DIRECTION toDWriteReadingDirection(TextFormat::Direction readingDirection);

    // TODO: create before starting draw calls?
    std::unordered_map<size_t, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> m_solidColorBrushes;
    std::unordered_map<size_t, Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush>> m_linearGradientBrushes;
    std::unordered_map<size_t, Microsoft::WRL::ComPtr<ID2D1RadialGradientBrush>> m_radialGradientBrushes;
    std::unordered_map<size_t, Microsoft::WRL::ComPtr<ID2D1BitmapBrush>> m_bitmapBrushes;
    std::map<StrokeStyle, Microsoft::WRL::ComPtr<ID2D1StrokeStyle>> m_strokeStyles;
    std::unordered_map<uint32_t, Microsoft::WRL::ComPtr<ID2D1PathGeometry>> m_pathGeometries;
    std::unordered_map<size_t, Microsoft::WRL::ComPtr<ID2D1Bitmap>> m_bitmaps;
    std::unordered_map<size_t, Microsoft::WRL::ComPtr<IDWriteTextFormat>> m_textFormats;
    std::unordered_map<size_t, Microsoft::WRL::ComPtr<IDWriteTextLayout>> m_textLayouts;

    Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_deviceContext;
    Microsoft::WRL::ComPtr<ID2D1Factory1> m_factory;
    Microsoft::WRL::ComPtr<IDWriteFactory> m_dwriteFactory;
};
} // karin

#endif //SRC_GRAPHICS_GRAPHICS_D2D_DEVICE_RESOURCES_H
