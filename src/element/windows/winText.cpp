#include "windows/winText.h"

namespace element
{

HRESULT winText::Create(
    const std::wstring& text,
    const int x,
    const int y,
    const int width,
    const int height
)
{
    m_text = text;

    HRESULT hr = m_dwriteText.Create(
        text,
        D2D1::RectF(
            static_cast<float>(x),
            static_cast<float>(y),
            static_cast<float>(x + width),
            static_cast<float>(y + height)
        )
    );
    if (FAILED(hr))
    {
        return hr;
    }

    return S_OK;
}

void winText::Render(
    const Microsoft::WRL::ComPtr<ID2D1DeviceContext>& device_context
)
{
    m_dwriteText.Render(device_context);
}

HRESULT winText::SetText(const std::wstring& new_text)
{
    HRESULT hr = m_dwriteText.SetText(new_text);
    if (FAILED(hr))
    {
        return hr;
    }

    m_text = new_text;

    return S_OK;
}

const std::wstring& winText::GetText() const
{
    return m_text;
}

Size winText::GetSize() const
{
    return m_rect.GetSize();
}

HRESULT winText::SetSize(const Size size)
{
    HRESULT hr = m_dwriteText.SetSize(size.width, size.height);
    if (FAILED(hr))
    {
        return hr;
    }

    m_rect.SetSize(size);
    return S_OK;
}

Point winText::GetPosition() const
{
    return m_rect.GetPosition();
}

void winText::SetPosition(const Point position)
{
    m_dwriteText.SetPosition(position.x, position.y);
    m_rect.SetPosition(position);
}

Rectangle winText::GetRectangle() const
{
    return m_rect;
}

HRESULT winText::SetRectangle(const Rectangle rect)
{
    HRESULT hr = m_dwriteText.SetLayoutRect(
        D2D1::RectF(rect.x, rect.y, rect.x + rect.width, rect.y + rect.height)
    );
    if (FAILED(hr))
    {
        return hr;
    }

    m_rect = rect;
    return S_OK;
}

float winText::GetFontSize() const
{
    return m_font.size;
}

HRESULT winText::SetFontSize(float size)
{
    HRESULT hr = m_dwriteText.SetFontSize(size);
    if (FAILED(hr))
    {
        return hr;
    }

    m_font.size = size;
    return S_OK;
}

Font::HorizontalAlignment winText::GetHorizontalAlignment() const
{
    return m_font.horizontal_alignment;
}

HRESULT winText::SetHorizontalAlignment(Font::HorizontalAlignment alignment)
{
    DWRITE_TEXT_ALIGNMENT dwrite_alignment;
    switch (alignment)
    {
        case Font::HorizontalAlignment::LEADING:
            dwrite_alignment = DWRITE_TEXT_ALIGNMENT_LEADING;
            break;

        case Font::HorizontalAlignment::TRAILING:
            dwrite_alignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
            break;

        case Font::HorizontalAlignment::CENTER:
            dwrite_alignment = DWRITE_TEXT_ALIGNMENT_CENTER;
            break;

        case Font::HorizontalAlignment::JUSTIFIED:
            dwrite_alignment = DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
            break;

        default:
            return E_INVALIDARG;
    }

    HRESULT hr = m_dwriteText.SetHorizontalAlignment(dwrite_alignment);
    if (FAILED(hr))
    {
        return hr;
    }

    m_font.horizontal_alignment = alignment;
    return S_OK;
}

Font::VerticalAlignment winText::GetVerticalAlignment() const
{
    return m_font.vertical_alignment;
}

HRESULT winText::SetVerticalAlignment(Font::VerticalAlignment alignment)
{
    DWRITE_PARAGRAPH_ALIGNMENT dwrite_alignment;
    switch (alignment)
    {
        case Font::VerticalAlignment::TOP:
            dwrite_alignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
            break;

        case Font::VerticalAlignment::CENTER:
            dwrite_alignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
            break;

        case Font::VerticalAlignment::BOTTOM:
            dwrite_alignment = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
            break;

        default:
            return E_INVALIDARG;
    }

    HRESULT hr = m_dwriteText.SetVerticalAlignment(dwrite_alignment);
    if (FAILED(hr))
    {
        return hr;
    }

    m_font.vertical_alignment = alignment;
    return S_OK;
}

std::wstring winText::GetFontFamily() const
{
    return m_font.family;
}

HRESULT winText::SetFontFamily(const std::wstring& family)
{
    HRESULT hr = m_dwriteText.SetFontFamily(family);
    if (FAILED(hr))
    {
        return hr;
    }

    m_font.family = family;
    return S_OK;
}

Font::Style winText::GetFontStyle() const
{
    return m_font.style;
}

HRESULT winText::SetFontStyle(Font::Style style)
{
    DWRITE_FONT_STYLE dwrite_style;
    switch (style)
    {
        case Font::Style::NORMAL:
            dwrite_style = DWRITE_FONT_STYLE_NORMAL;
            break;

        case Font::Style::ITALIC:
            dwrite_style = DWRITE_FONT_STYLE_ITALIC;
            break;

        case Font::Style::OBLIQUE:
            dwrite_style = DWRITE_FONT_STYLE_OBLIQUE;
            break;

        default:
            return E_INVALIDARG;
    }

    HRESULT hr = m_dwriteText.SetFontStyle(dwrite_style);
    if (FAILED(hr))
    {
        return hr;
    }

    m_font.style = style;
    return S_OK;
}

Font::Stretch winText::GetFontStretch() const
{
    return m_font.stretch;
}

HRESULT winText::SetFontStretch(Font::Stretch stretch)
{
    DWRITE_FONT_STRETCH dwrite_stretch;
    switch (stretch)
    {
        case Font::Stretch::ULTRA_CONDENSED:
            dwrite_stretch = DWRITE_FONT_STRETCH_ULTRA_CONDENSED;
            break;

        case Font::Stretch::EXTRA_CONDENSED:
            dwrite_stretch = DWRITE_FONT_STRETCH_EXTRA_CONDENSED;
            break;

        case Font::Stretch::CONDENSED:
            dwrite_stretch = DWRITE_FONT_STRETCH_CONDENSED;
            break;

        case Font::Stretch::SEMI_CONDENSED:
            dwrite_stretch = DWRITE_FONT_STRETCH_SEMI_CONDENSED;
            break;

        case Font::Stretch::NORMAL:
            dwrite_stretch = DWRITE_FONT_STRETCH_NORMAL;
            break;

        case Font::Stretch::SEMI_EXPANDED:
            dwrite_stretch = DWRITE_FONT_STRETCH_SEMI_EXPANDED;
            break;

        case Font::Stretch::EXPANDED:
            dwrite_stretch = DWRITE_FONT_STRETCH_EXPANDED;
            break;

        case Font::Stretch::EXTRA_EXPANDED:
            dwrite_stretch = DWRITE_FONT_STRETCH_EXTRA_EXPANDED;
            break;

        case Font::Stretch::ULTRA_EXPANDED:
            dwrite_stretch = DWRITE_FONT_STRETCH_ULTRA_EXPANDED;
            break;

        default:
            return E_INVALIDARG;
    }

    HRESULT hr = m_dwriteText.SetFontStretch(dwrite_stretch);
    if (FAILED(hr))
    {
        return hr;
    }

    m_font.stretch = stretch;
    return S_OK;
}

}  // namespace element