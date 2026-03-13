#ifndef SRC_GUI_APPLICATION_H
#define SRC_GUI_APPLICATION_H

#include "window.h"

#include <karin/system/application.h>
#include <karin/graphics/text_engine.h>

#include <memory>
#include <vector>

namespace karin::gui
{
class Application
{
public:
    Application();
    ~Application() = default;

    std::shared_ptr<Window> createWindow(
        const std::wstring& title,
        int x,
        int y,
        int width,
        int height
    );

    void run();

private:
    // ApplicationContextとかにまとめてもいいかも
    std::unique_ptr<TextEngine> m_textEngine;

    std::vector<std::shared_ptr<Window>> m_windows;
};
} // karin::gui

#endif //SRC_GUI_APPLICATION_H