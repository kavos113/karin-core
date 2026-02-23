#ifndef SRC_GRAPHICS_TEXT_WINDOWS_DWRITE_FONT_RENDERER_H
#define SRC_GRAPHICS_TEXT_WINDOWS_DWRITE_FONT_RENDERER_H

#include <font_renderer_platform.h>

#include <dwrite.h>
#include <d2d1_1.h>
#include <wrl/client.h>

namespace karin
{
class DwriteFontRenderer : public IPlatformFontRenderer
{
public:
    DwriteFontRenderer(Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext);
    ~DwriteFontRenderer() override = default;

    void renderText(const std::vector<TextLayouter::GlyphPosition>& glyphs, IFontFace* face, float fontSize) override;

private:
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_deviceContext;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_WINDOWS_DWRITE_FONT_RENDERER_H