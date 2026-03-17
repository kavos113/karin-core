#include "application_context.h"

namespace karin::gui
{
ApplicationContext::ApplicationContext()
{
    textEngine = std::make_unique<TextEngine>();
}

ApplicationContext::~ApplicationContext() = default;
} // karin::gui