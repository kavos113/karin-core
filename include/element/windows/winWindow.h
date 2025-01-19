#ifndef WIN_WINDOW_H
#define WIN_WINDOW_H

#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>

namespace element
{

class winWindow
{
public:
    winWindow() = default;
    winWindow(const winWindow&) = delete;
    winWindow& operator=(const winWindow&) = delete;
    winWindow(winWindow&&) = delete;
    winWindow& operator=(winWindow&&) = delete;
    ~winWindow();

    HRESULT Create(
        const wchar_t* title = L"",
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int width = CW_USEDEFAULT,
        int height = CW_USEDEFAULT
    );
    [[nodiscard]] bool IsActive() const;
    [[nodiscard]] bool IsShow() const;
    [[nodiscard]] HWND GetHwnd() const;
    void Destroy();
    void Show();

private:
    HWND m_hwnd{nullptr};
    bool m_isShow{false};
};

}  // namespace element

#endif  // WIN_WINDOW_H
