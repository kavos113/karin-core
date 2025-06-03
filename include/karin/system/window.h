#ifndef SRC_SYSTEM_WINDOW_H
#define SRC_SYSTEM_WINDOW_H

#include <memory>
#include <string>

namespace karin
{

class IWindowImpl;

class Window
{
public:
    enum class ShowStatus : uint8_t
    {
        HIDE,
        SHOW,
        MINIMIZE,
        MAXIMIZE
    };

    // TODO: size, point, rect classes
    Window(
        const std::wstring &title,
        int x = 0,
        int y = 0,
        int width = DEFAULT_WIDTH,
        int height = DEFAULT_HEIGHT
    );
    ~Window();

    void setStatus(ShowStatus status);
    ShowStatus status() const;

private:
    ShowStatus m_showStatus = ShowStatus::HIDE;

    std::unique_ptr<IWindowImpl> m_impl;

    static constexpr int DEFAULT_WIDTH = 800;
    static constexpr int DEFAULT_HEIGHT = 600;
};

} // karin

#endif //SRC_SYSTEM_WINDOW_H
