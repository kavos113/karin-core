#ifndef SRC_GUI_APPLICATION_CONTEXT_H
#define SRC_GUI_APPLICATION_CONTEXT_H

#include <karin/graphics/text_engine.h>
#include <memory>

namespace karin::gui
{
class ApplicationContext
{
public:
    ApplicationContext();
    ~ApplicationContext();

    std::unique_ptr<TextEngine> textEngine;
};

ApplicationContext& getAppContext();
} // karin::gui

#endif //SRC_GUI_APPLICATION_CONTEXT_H