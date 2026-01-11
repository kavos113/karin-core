#ifndef SRC_GRAPHICS_TEXT_WINDOWS_DWRITE_FONT_FACE_H
#define SRC_GRAPHICS_TEXT_WINDOWS_DWRITE_FONT_FACE_H

#include <font_face.h>

#include <hb.h>
#include <dwrite.h>
#include <wrl/client.h>

namespace karin
{
class DwriteFontFace : IFontFace
{
public:
    DwriteFontFace(const Microsoft::WRL::ComPtr<IDWriteFontFace>& face);
    ~DwriteFontFace() override = default;

    hb_font_t* getHbFont() override;
    Microsoft::WRL::ComPtr<IDWriteFontFace> face();

private:
    hb_font_t *m_hbFont;
    Microsoft::WRL::ComPtr<IDWriteFontFace> m_face;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_WINDOWS_DWRITE_FONT_FACE_H