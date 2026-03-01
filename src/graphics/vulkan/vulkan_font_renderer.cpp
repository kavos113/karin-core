#include "vulkan_font_renderer.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "freetype/freetype_font_face.h"
#include "vulkan_pipeline.h"
#include "vulkan_renderer_impl.h"

namespace
{
using namespace karin;

FragPushConstants createFragPushConstantData(const Pattern& pattern)
{
    return std::visit(
        []<typename T0>(const T0& p) -> FragPushConstants
        {
            using T = std::decay_t<T0>;
            if constexpr (std::is_same_v<T, SolidColorPattern>)
            {
                Color color = p.color();
                return FragPushConstants{
                    .color = {color.r, color.g, color.b, color.a},
                    .patternType = static_cast<uint32_t>(PatternType::SolidColor)
                };
            }
            else if constexpr (std::is_same_v<T, LinearGradientPattern>)
            {
                return FragPushConstants{
                    .color = {p.start.x, p.start.y, p.end.x, p.end.y},
                    .patternType = static_cast<uint32_t>(PatternType::LinearGradient),
                };
            }
            else if constexpr (std::is_same_v<T, RadialGradientPattern>)
            {
                return FragPushConstants{
                    .color = {p.center.x, p.center.y, p.offset.x, p.offset.y},
                    .patternType = static_cast<uint32_t>(PatternType::RadialGradient),
                    .patternParams = {p.radiusX, p.radiusY, 0.0f, 0.0f},
                };
            }
            else if constexpr (std::is_same_v<T, ImagePattern>)
            {
                return FragPushConstants{
                    .color = {p.offset.x, p.offset.y, p.scaleX, p.scaleY},
                    .patternType = static_cast<uint32_t>(PatternType::Image),
                    .patternParams = {p.image.width(), p.image.height(), 1.0f, 0.0f}
                };
            }
            else
            {
                throw std::runtime_error("Unsupported pattern type");
            }
        }, pattern
    );
}

VertexPushConstants createVertexPushConstantData(const Transform2D& transform, const Point& position)
{
    glm::mat4 translateMatrix = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(position.x, position.y, 0.0f)
    );
    return VertexPushConstants{
        .model = translateMatrix * glm::make_mat4(transform.colMajorData())
    };
}
}

namespace karin
{
VulkanFontRenderer::VulkanFontRenderer(VulkanRendererImpl* renderer, size_t maxFramesInFlight)
    : m_glyphCache(std::make_unique<VulkanGlyphCache>(maxFramesInFlight)), m_renderer(renderer)
{
}

VulkanFontRenderer::~VulkanFontRenderer() = default;

void VulkanFontRenderer::cleanup() const
{
    m_glyphCache->cleanup();
}

void VulkanFontRenderer::drawText(
    const TextBlob& text, Point start, Pattern& pattern,
    const Transform2D& transform
) const
{
    auto* ftFontFace = dynamic_cast<FreetypeFontFace*>(text.fontFace.get());
    if (!ftFontFace)
    {
        std::cerr << "Unsupported font face type in VulkanFontRenderer::drawText" << std::endl;
        return;
    }

    FontMetrics fontMetrics = ftFontFace->getFontMetrics();
    float scale = text.fontEmSize / fontMetrics.unitsPerEm;

    std::vector<GlyphUploadPosition> glyphsToUpload;
    glyphsToUpload.reserve(text.glyphs.size());
    for (const auto& glyphPos : text.glyphs)
    {
        VulkanGlyphCache::GlyphInfo glyphInfo = m_glyphCache->getGlyph(glyphPos.glyphIndex, text.format.hash(), ftFontFace->face(), text.fontEmSize);
        GlyphMetrics metrics = ftFontFace->getGlyphMetrics(glyphPos.glyphIndex);

        glyphsToUpload.push_back({
            .position = Rectangle(
                glyphPos.position.pos.x,
                glyphPos.position.pos.y - metrics.bearingY * scale,
                glyphPos.position.size.width,
                glyphPos.position.size.height
            ),
            .atlasUV = glyphInfo.uv
        });
    }
    
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    for (const auto& glyph : glyphsToUpload)
    {
        Rectangle pos = Rectangle(
            -text.layoutSize.width / 2.0f + glyph.position.pos.x,
            -text.layoutSize.height / 2.0f + glyph.position.pos.y,
            glyph.position.size.width,
            glyph.position.size.height
        );

        size_t baseIndex = vertices.size();

        vertices.push_back(
            {
                .pos = {pos.pos.x, pos.pos.y},
                .uv = {glyph.atlasUV.pos.x, glyph.atlasUV.pos.y}
            }
        );
        vertices.push_back(
            {
                .pos = {pos.pos.x + pos.size.width, pos.pos.y},
                .uv = {glyph.atlasUV.pos.x + glyph.atlasUV.size.width, glyph.atlasUV.pos.y}
            }
        );
        vertices.push_back(
            {
                .pos = {pos.pos.x + pos.size.width, pos.pos.y + pos.size.height},
                .uv = {
                    glyph.atlasUV.pos.x + glyph.atlasUV.size.width,
                    glyph.atlasUV.pos.y + glyph.atlasUV.size.height
                }
            }
        );
        vertices.push_back(
            {
                .pos = {pos.pos.x, pos.pos.y + pos.size.height},
                .uv = {glyph.atlasUV.pos.x, glyph.atlasUV.pos.y + glyph.atlasUV.size.height}
            }
        );

        indices.push_back(static_cast<uint16_t>(baseIndex));
        indices.push_back(static_cast<uint16_t>(baseIndex + 1));
        indices.push_back(static_cast<uint16_t>(baseIndex + 2));
        indices.push_back(static_cast<uint16_t>(baseIndex + 2));
        indices.push_back(static_cast<uint16_t>(baseIndex + 3));
        indices.push_back(static_cast<uint16_t>(baseIndex));
    }

    m_renderer->addCommand(
        vertices, indices,
        createFragPushConstantData(pattern),
        createVertexPushConstantData(transform, Point(
            start.x + text.layoutSize.width / 2.0f,
            start.y + text.layoutSize.height / 2.0f
        )),
        pattern,
        VulkanRendererImpl::PipelineType::Text
    );
}
} // karin