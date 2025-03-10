#ifndef KARIN_WINDOWS_WIN_WINDOWS_H
#define KARIN_WINDOWS_WIN_WINDOWS_H

#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>

#include <thread>
#include <vector>

#include "color/Color.h"
#include "direct2d/d2dWindow.h"
#include "geometry/Point.h"
#include "geometry/Rectangle.h"
#include "geometry/Size.h"
#include "winText.h"

namespace karin
{

class winWindow
{
public:
    enum class ShowStatus : std::uint8_t
    {
        HIDE,
        SHOW,
        MINIMIZE,
        MAXIMIZE
    };

    enum class WindowAction : std::uint8_t
    {
        SHOW,
        HIDE,
        MINIMIZE,
        MAXIMIZE,
        SIZE,
        POSITION,
        RECTANGLE,
        BACKGROUND_COLOR,
        ACTIVE,
        SHOWSTATUS,
        HWND
    };

    winWindow() = default;
    winWindow(const winWindow&) = delete;
    winWindow& operator=(const winWindow&) = delete;
    winWindow(winWindow&&) = delete;
    winWindow& operator=(winWindow&&) = delete;
    ~winWindow();

    static HRESULT RegisterWindowClass();
    static HRESULT UnregisterWindowClass();
    HRESULT Create(
        const wchar_t* title = L"",
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int width = CW_USEDEFAULT,
        int height = CW_USEDEFAULT
    );
    [[nodiscard]] bool IsActive() const;
    [[nodiscard]] ShowStatus GetShowStatus() const;
    [[nodiscard]] HWND GetHwnd() const;

    void Show();
    void Run();
    void Hide();
    void Minimize();
    void Maximize();

    void Add(std::unique_ptr<winText> text);

    [[nodiscard]] Size GetSize() const;
    void SetSize(Size size);
    [[nodiscard]] Point GetPosition() const;
    void SetPosition(Point position);
    [[nodiscard]] Rectangle GetRectangle() const;
    void SetRectangle(Rectangle rect);
    [[nodiscard]] Color GetBackgroundColor() const;
    void SetBackgroundColor(Color color);

    static LRESULT CALLBACK
    WinWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd{nullptr};
    ShowStatus m_showStatus{ShowStatus::HIDE};
    d2dWindow m_d2dWindow;

    Rectangle m_rect{};
    Color m_backgroundColor{Color::Colors::White};

    std::vector<std::unique_ptr<winText>> m_texts;  // change to Component

    static constexpr auto class_name = L"karin_window";
};

}  // namespace karin

#endif  // KARIN_WINDOWS_WIN_WINDOWS_H
