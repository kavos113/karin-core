#ifndef SRC_GUI_APPLICATION_H
#define SRC_GUI_APPLICATION_H

#include "window.h"

#include <karin/system/application.h>

#include <memory>
#include <vector>

namespace karin::gui
{
class ApplicationContext;

class Application
{
public:
    Application();
    ~Application();

    std::shared_ptr<Window> createWindow(
        const std::wstring& title,
        int x,
        int y,
        int width,
        int height
    );

    void run();

private:
    std::unique_ptr<ApplicationContext> m_context;

    std::vector<std::shared_ptr<Window>> m_windows;
};
} // karin::gui

#endif //SRC_GUI_APPLICATION_H