#include "d2d_device_resources.h"

#include "d2d_geometry.h"
#include "d2d_color.h"
#include "d2d_consts.h"

#include <d2d/dwrite_converter.h>

#include <wincodec.h>

#include <cmath>
#include <utils/string.h>
#include <fstream>
#include <numbers>
#include <ranges>
#include <stdexcept>
#include <variant>
#include <functional>
#include <string_view>

namespace
{
using namespace karin;

D2D1_CAP_STYLE toD2DCapStyle(StrokeStyle::CapStyle capStyle)
{
    switch (capStyle)
    {
    case StrokeStyle::CapStyle::Butt:
        return D2D1_CAP_STYLE_FLAT;
    case StrokeStyle::CapStyle::Round:
        return D2D1_CAP_STYLE_ROUND;
    case StrokeStyle::CapStyle::Square:
        return D2D1_CAP_STYLE_SQUARE;
    case StrokeStyle::CapStyle::Triangle:
        return D2D1_CAP_STYLE_TRIANGLE;
    default:
        throw std::invalid_argument("Unknown cap style");
    }
}

D2D1_LINE_JOIN toD2DJoinStyle(StrokeStyle::JoinStyle joinStyle)
{
    switch (joinStyle)
    {
    case StrokeStyle::JoinStyle::Miter:
        return D2D1_LINE_JOIN_MITER;
    case StrokeStyle::JoinStyle::Round:
        return D2D1_LINE_JOIN_ROUND;
    case StrokeStyle::JoinStyle::Bevel:
        return D2D1_LINE_JOIN_BEVEL;
    default:
        throw std::invalid_argument("Unknown join style");
    }
}

D2D1_EXTEND_MODE toD2DExtendMode(ExtendMode extendMode)
{
    switch (extendMode)
    {
    case ExtendMode::CLAMP:
        return D2D1_EXTEND_MODE_CLAMP;
    case ExtendMode::REPEAT:
        return D2D1_EXTEND_MODE_WRAP;
    case ExtendMode::MIRROR:
        return D2D1_EXTEND_MODE_MIRROR;
    default:
        throw std::invalid_argument("Unknown extend mode");
    }
}

DWRITE_TEXT_ALIGNMENT toDWriteTextAlignment(TextFormat::HorizontalAlignment alignment)
{
    switch (alignment)
    {
    case TextFormat::HorizontalAlignment::LEADING:
        return DWRITE_TEXT_ALIGNMENT_LEADING;
    case TextFormat::HorizontalAlignment::TRAILING:
        return DWRITE_TEXT_ALIGNMENT_TRAILING;
    case TextFormat::HorizontalAlignment::CENTER:
        return DWRITE_TEXT_ALIGNMENT_CENTER;
    case TextFormat::HorizontalAlignment::JUSTIFIED:
        return DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
    default:
        throw std::invalid_argument("Unknown text alignment");
    }
}

DWRITE_PARAGRAPH_ALIGNMENT toDWriteParagraphAlignment(TextFormat::VerticalAlignment alignment)
{
    switch (alignment)
    {
    case TextFormat::VerticalAlignment::TOP:
        return DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
    case TextFormat::VerticalAlignment::CENTER:
        return DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
    case TextFormat::VerticalAlignment::BOTTOM:
        return DWRITE_PARAGRAPH_ALIGNMENT_FAR;
    default:
        throw std::invalid_argument("Unknown paragraph alignment");
    }
}

DWRITE_WORD_WRAPPING toDWriteWordWrapping(TextFormat::Wrapping wordWrapping)
{
    switch (wordWrapping)
    {
    case TextFormat::Wrapping::NONE:
        return DWRITE_WORD_WRAPPING_NO_WRAP;
    case TextFormat::Wrapping::CHARACTER:
        return DWRITE_WORD_WRAPPING_CHARACTER;
    case TextFormat::Wrapping::WORD:
        return DWRITE_WORD_WRAPPING_WRAP;
    default:
        throw std::invalid_argument("Unknown word wrapping");
    }
}

DWRITE_TRIMMING_GRANULARITY toDWriteTrimmingGranularity(TextFormat::Trimming trimming)
{
    switch (trimming)
    {
    case TextFormat::Trimming::NONE:
        return DWRITE_TRIMMING_GRANULARITY_NONE;
    case TextFormat::Trimming::WORD:
        return DWRITE_TRIMMING_GRANULARITY_CHARACTER;
    case TextFormat::Trimming::CHARACTER:
        return DWRITE_TRIMMING_GRANULARITY_CHARACTER;
    default:
        throw std::invalid_argument("Unknown trimming granularity");
    }
}

DWRITE_FLOW_DIRECTION toDWriteFlowDirection(TextFormat::Direction flowDirection)
{
    switch (flowDirection)
    {
    case TextFormat::Direction::TOP_TO_BOTTOM:
        return DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM;
    case TextFormat::Direction::BOTTOM_TO_TOP:
        return DWRITE_FLOW_DIRECTION_BOTTOM_TO_TOP;
    case TextFormat::Direction::LEFT_TO_RIGHT:
        return DWRITE_FLOW_DIRECTION_LEFT_TO_RIGHT;
    case TextFormat::Direction::RIGHT_TO_LEFT:
        return DWRITE_FLOW_DIRECTION_RIGHT_TO_LEFT;
    default:
        throw std::invalid_argument("Unknown flow direction");
    }
}

DWRITE_READING_DIRECTION toDWriteReadingDirection(TextFormat::Direction readingDirection)
{
    switch (readingDirection)
    {
    case TextFormat::Direction::TOP_TO_BOTTOM:
        return DWRITE_READING_DIRECTION_TOP_TO_BOTTOM;
    case TextFormat::Direction::BOTTOM_TO_TOP:
        return DWRITE_READING_DIRECTION_BOTTOM_TO_TOP;
    case TextFormat::Direction::LEFT_TO_RIGHT:
        return DWRITE_READING_DIRECTION_LEFT_TO_RIGHT;
    case TextFormat::Direction::RIGHT_TO_LEFT:
        return DWRITE_READING_DIRECTION_RIGHT_TO_LEFT;
    default:
        throw std::invalid_argument("Unknown reading direction");
    }
}

DWRITE_LINE_SPACING_METHOD toDWriteLineSpacingMethod(TextFormat::LineSpacingMode lineSpacingMode)
{
    switch (lineSpacingMode)
    {
    case TextFormat::LineSpacingMode::DEFAULT:
        return DWRITE_LINE_SPACING_METHOD_DEFAULT;
    case TextFormat::LineSpacingMode::UNIFORM:
        return DWRITE_LINE_SPACING_METHOD_UNIFORM;
    case TextFormat::LineSpacingMode::PROPORTIONAL:
        return DWRITE_LINE_SPACING_METHOD_PROPORTIONAL;
    default:
        throw std::invalid_argument("Unknown line spacing method");
    }
}
}

namespace karin
{
void D2DDeviceResources::clear()
{
    for (auto& val : m_solidColorBrushes | std::views::values)
    {
        val.Reset();
    }
}

Microsoft::WRL::ComPtr<ID2D1Brush> D2DDeviceResources::brush(Pattern& pattern)
{
    return std::visit(
        [this]<typename T0>(const T0& p) -> Microsoft::WRL::ComPtr<ID2D1Brush>
        {
            using T = std::decay_t<T0>;
            if constexpr (std::is_same_v<T, SolidColorPattern>)
            {
                return solidColorBrush(p);
            }
            else if constexpr (std::is_same_v<T, LinearGradientPattern>)
            {
                return linearGradientBrush(p);
            }
            else if constexpr (std::is_same_v<T, RadialGradientPattern>)
            {
                return radialGradientBrush(p);
            }
            else if constexpr (std::is_same_v<T, ImagePattern>)
            {
                return bitmapBrush(p);
            }
            else
            {
                throw std::runtime_error("Unsupported pattern type");
            }
        }, pattern
    );
}

Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> D2DDeviceResources::solidColorBrush(const SolidColorPattern& pattern)
{
    if (auto it = m_solidColorBrushes.find(pattern.hash()); it != m_solidColorBrushes.end())
    {
        return it->second;
    }

    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
    HRESULT hr = m_deviceContext->CreateSolidColorBrush(
        toD2DColor(pattern.color()),
        &brush
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create solid color brush");
    }

    m_solidColorBrushes[pattern.hash()] = brush;
    return brush;
}

Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> D2DDeviceResources::linearGradientBrush(
    const LinearGradientPattern& pattern
)
{
    if (auto it = m_linearGradientBrushes.find(pattern.hash()); it != m_linearGradientBrushes.end())
    {
        return it->second;
    }

    Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> gradientStops;
    std::vector<D2D1_GRADIENT_STOP> stops(pattern.gradientPoints.points.size());
    for (const auto& [offset, color] : pattern.gradientPoints.points)
    {
        stops.push_back(D2D1::GradientStop(offset, toD2DColor(color)));
    }

    D2D1_EXTEND_MODE extendMode = D2D1_EXTEND_MODE_CLAMP;
    switch (pattern.gradientPoints.extendMode)
    {
    case ExtendMode::CLAMP:
        extendMode = D2D1_EXTEND_MODE_CLAMP;
        break;

    case ExtendMode::REPEAT:
        extendMode = D2D1_EXTEND_MODE_WRAP;
        break;

    case ExtendMode::MIRROR:
        extendMode = D2D1_EXTEND_MODE_MIRROR;
        break;
    }

    HRESULT hr = m_deviceContext->CreateGradientStopCollection(
        stops.data(),
        static_cast<UINT32>(stops.size()),
        D2D1_GAMMA_2_2,
        extendMode,
        &gradientStops
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create gradient stop collection");
    }

    D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES properties = {
        .startPoint = toD2DPoint(pattern.start),
        .endPoint = toD2DPoint(pattern.end)
    };

    Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> brush;
    hr = m_deviceContext->CreateLinearGradientBrush(
        properties,
        gradientStops.Get(),
        &brush
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create linear gradient brush");
    }

    m_linearGradientBrushes[pattern.hash()] = brush;
    return brush;
}

Microsoft::WRL::ComPtr<ID2D1RadialGradientBrush> D2DDeviceResources::radialGradientBrush(
    const RadialGradientPattern& pattern
)
{
    if (auto it = m_radialGradientBrushes.find(pattern.hash()); it != m_radialGradientBrushes.end())
    {
        return it->second;
    }

    Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> gradientStops;
    std::vector<D2D1_GRADIENT_STOP> stops(pattern.gradientPoints.points.size());
    for (const auto& [offset, color] : pattern.gradientPoints.points)
    {
        stops.push_back(D2D1::GradientStop(offset, toD2DColor(color)));
    }

    D2D1_EXTEND_MODE extendMode = toD2DExtendMode(pattern.gradientPoints.extendMode);

    HRESULT hr = m_deviceContext->CreateGradientStopCollection(
        stops.data(),
        static_cast<UINT32>(stops.size()),
        D2D1_GAMMA_2_2,
        extendMode,
        &gradientStops
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create gradient stop collection");
    }

    D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES properties = {
        .center = toD2DPoint(pattern.center),
        .gradientOriginOffset = toD2DPoint(pattern.offset),
        .radiusX = pattern.radiusX,
        .radiusY = pattern.radiusY
    };

    Microsoft::WRL::ComPtr<ID2D1RadialGradientBrush> brush;
    hr = m_deviceContext->CreateRadialGradientBrush(
        properties,
        gradientStops.Get(),
        &brush
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create radial gradient brush");
    }

    m_radialGradientBrushes[pattern.hash()] = brush;
    return brush;
}

Microsoft::WRL::ComPtr<ID2D1BitmapBrush> D2DDeviceResources::bitmapBrush(const ImagePattern& pattern)
{
    if (auto it = m_bitmapBrushes.find(pattern.hash()); it != m_bitmapBrushes.end())
    {
        return it->second;
    }

    Microsoft::WRL::ComPtr<ID2D1Bitmap> image = bitmap(pattern.image);
    if (!image)
    {
        throw std::runtime_error("Failed to create bitmap for image pattern");
    }

    D2D1_BITMAP_BRUSH_PROPERTIES properties = {
        .extendModeX = toD2DExtendMode(pattern.extendModeX),
        .extendModeY = toD2DExtendMode(pattern.extendModeY),
        .interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
    };

    D2D1_BRUSH_PROPERTIES brushProperties = {
        .opacity = 1.0f,
        .transform = D2D1::Matrix3x2F::Translation(pattern.offset.x, pattern.offset.y) *
        D2D1::Matrix3x2F::Scale(pattern.scaleX, pattern.scaleY)
    };

    Microsoft::WRL::ComPtr<ID2D1BitmapBrush> brush;
    HRESULT hr = m_deviceContext->CreateBitmapBrush(
        image.Get(),
        properties,
        brushProperties,
        &brush
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create bitmap brush");
    }

    m_bitmapBrushes[pattern.hash()] = brush;
    return brush;
}

Microsoft::WRL::ComPtr<ID2D1StrokeStyle> D2DDeviceResources::strokeStyle(const StrokeStyle& style)
{
    if (auto it = m_strokeStyles.find(style); it != m_strokeStyles.end())
    {
        return it->second;
    }

    D2D1_STROKE_STYLE_PROPERTIES properties = {
        .startCap = toD2DCapStyle(style.start_cap_style),
        .endCap = toD2DCapStyle(style.end_cap_style),
        .dashCap = toD2DCapStyle(style.dash_cap_style),
        .lineJoin = toD2DJoinStyle(style.join_style),
        .miterLimit = style.miter_limit,
        .dashStyle = style.dash_pattern.empty() ? D2D1_DASH_STYLE_SOLID : D2D1_DASH_STYLE_CUSTOM,
        .dashOffset = style.dash_offset,
    };
    Microsoft::WRL::ComPtr<ID2D1StrokeStyle> strokeStyle;
    const FLOAT* dashes = style.dash_pattern.empty() ? nullptr : style.dash_pattern.data();
    HRESULT hr = m_factory->CreateStrokeStyle(
        properties,
        dashes,
        static_cast<UINT32>(style.dash_pattern.size()),
        &strokeStyle
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create D2D stroke style");
    }

    m_strokeStyles[style] = strokeStyle;
    return strokeStyle;
}

Microsoft::WRL::ComPtr<ID2D1PathGeometry> D2DDeviceResources::pathGeometry(const PathImpl& path)
{
    if (auto it = m_pathGeometries.find(path.id()); it != m_pathGeometries.end())
    {
        return it->second;
    }

    Microsoft::WRL::ComPtr<ID2D1PathGeometry> geometry;
    HRESULT hr = m_factory->CreatePathGeometry(&geometry);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create D2D path geometry");
    }

    auto commands = path.commands();

    Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
    hr = geometry->Open(&sink);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to open D2D geometry sink");
    }

    sink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
    sink->BeginFigure(toD2DPoint(path.startPoint()), D2D1_FIGURE_BEGIN_FILLED);
    for (const auto& command : commands)
    {
        std::visit(
            [&sink]<typename T0>(const T0& args)
            {
                using T = std::decay_t<T0>;
                if constexpr (std::is_same_v<T, PathImpl::LineArgs>)
                {
                    sink->AddLine(toD2DPoint(args.end));
                }
                else if constexpr (std::is_same_v<T, PathImpl::ArcArgs>)
                {
                    constexpr float twoPi = 2.0f * std::numbers::pi_v<float>;

                    auto end = Point(
                        args.center.x + args.radiusX * std::cos(args.endAngle),
                        args.center.y + args.radiusY * std::sin(-args.endAngle) // bottom is big
                    );
                    float startAngle = std::fmod(args.startAngle, twoPi);
                    float endAngle = std::fmod(args.endAngle, twoPi);

                    if (startAngle < 0.0f)
                    {
                        startAngle += twoPi;
                    }

                    if (endAngle < 0.0f)
                    {
                        endAngle += twoPi;
                    }

                    D2D1_SWEEP_DIRECTION sweepDirection =
                        args.isSmallArc
                            ? (endAngle < startAngle
                                   ? D2D1_SWEEP_DIRECTION_CLOCKWISE
                                   : D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE)
                            : (endAngle < startAngle
                                   ? D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE
                                   : D2D1_SWEEP_DIRECTION_CLOCKWISE);
                    sink->AddArc(
                        D2D1::ArcSegment(
                            toD2DPoint(end),
                            D2D1::SizeF(args.radiusX, args.radiusY),
                            0.0f, // rotation angle
                            sweepDirection,
                            args.isSmallArc ? D2D1_ARC_SIZE_SMALL : D2D1_ARC_SIZE_LARGE
                        )
                    );
                }
            }, command
        );
    }
    sink->EndFigure(D2D1_FIGURE_END_CLOSED);
    hr = sink->Close();
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to close D2D geometry sink");
    }

    m_pathGeometries[path.id()] = geometry;
    return geometry;
}

Image D2DDeviceResources::createImage(const std::vector<std::byte>& data, uint32_t width, uint32_t height)
{
    D2D1_BITMAP_PROPERTIES bitmapProperties = {
        .pixelFormat = D2D1::PixelFormat(
            DXGI_FORMAT_R8G8B8A8_UNORM,
            D2D1_ALPHA_MODE_PREMULTIPLIED
        ),
        .dpiX = DEFAULT_DPI,
        .dpiY = DEFAULT_DPI,
    };

    Microsoft::WRL::ComPtr<ID2D1Bitmap> bitmap;
    HRESULT hr = m_deviceContext->CreateBitmap(
        D2D1::SizeU(width, height),
        data.data(),
        width * 4,
        &bitmapProperties,
        &bitmap
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create D2D bitmap");
    }

    std::string_view dataView(reinterpret_cast<const char*>(data.data()), data.size());
    size_t hash = std::hash<std::string_view>{}(dataView);

    m_bitmaps[hash] = bitmap;
    return Image(hash, width, height);
}

Microsoft::WRL::ComPtr<ID2D1Bitmap> D2DDeviceResources::bitmap(const Image& image)
{
    if (auto it = m_bitmaps.find(image.hash()); it != m_bitmaps.end())
    {
        return it->second;
    }

    throw std::runtime_error("Bitmap creation not implemented");
}

Microsoft::WRL::ComPtr<IDWriteTextLayout> D2DDeviceResources::textLayout(const TextLayout& layout)
{
    if (auto it = m_textLayouts.find(layout.hash()); it != m_textLayouts.end())
    {
        return it->second;
    }

    Microsoft::WRL::ComPtr<IDWriteTextFormat> format;
    if (auto it = m_textFormats.find(layout.format.hash()); it != m_textFormats.end())
    {
        format = it->second;
    }
    else
    {
        HRESULT hr = m_dwriteFactory->CreateTextFormat(
            toWString(layout.format.font.family).c_str(),
            nullptr,
            toDWriteFontWeight(layout.format.font.weight),
            toDWriteFontStyle(layout.format.font.style),
            toDWriteFontStretch(layout.format.font.stretch),
            layout.format.size,
            toWString(layout.format.locale).c_str(),
            &format
        );
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to create DWrite text format");
        }

        hr = format->SetTextAlignment(toDWriteTextAlignment(layout.format.horizontalAlignment));
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to set text alignment in DWrite text format");
        }

        hr = format->SetParagraphAlignment(toDWriteParagraphAlignment(layout.format.verticalAlignment));
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to set paragraph alignment in DWrite text format");
        }

        hr = format->SetWordWrapping(toDWriteWordWrapping(layout.format.wrapping));
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to set word wrapping in DWrite text format");
        }
        DWRITE_TRIMMING trimming = {
            .granularity = toDWriteTrimmingGranularity(layout.format.trimming),
            .delimiter = 0, // No delimiter for now
            .delimiterCount = 0
        };
        hr = format->SetTrimming(&trimming, nullptr);
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to set trimming in DWrite text format");
        }

        hr = format->SetLineSpacing(
            toDWriteLineSpacingMethod(layout.format.lineSpacingMode),
            layout.format.lineSpacing,
            layout.format.baseline
        );
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to set line spacing in DWrite text format");
        }

        hr = format->SetFlowDirection(toDWriteFlowDirection(layout.format.flowDirection));
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to set flow direction in DWrite text format");
        }

        hr = format->SetReadingDirection(toDWriteReadingDirection(layout.format.readingDirection));
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to set reading direction in DWrite text format");
        }

        m_textFormats[layout.format.hash()] = format;
    }

    Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout;
    HRESULT hr = m_dwriteFactory->CreateTextLayout(
        toWString(layout.text).c_str(),
        static_cast<UINT32>(layout.text.size()),
        format.Get(),
        layout.size.width,
        layout.size.height,
        &textLayout
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create DWrite text layout");
    }

    DWRITE_TEXT_RANGE textRange = {
        .startPosition = 0,
        .length = static_cast<UINT32>(layout.text.size())
    };
    if (layout.underline)
    {
        hr = textLayout->SetUnderline(true, textRange);
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to set underline in DWrite text layout");
        }
    }
    if (layout.lineThrough)
    {
        hr = textLayout->SetStrikethrough(true, textRange);
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to set line-through in DWrite text layout");
        }
    }

    m_textLayouts[layout.hash()] = textLayout;
    return textLayout;
}
} // karin
