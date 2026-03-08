#include <karin/common.h>

#include <memory>

#include "window.h"
#include "container_node.h"
#include "rectangle_node.h"
#include "view_node.h"

int main()
{
    karin::gui::Window window(L"Hello Layout", 100, 100, 800, 600);

    auto rootView = std::make_unique<karin::gui::ContainerNode>(karin::Size(800, 600));

    auto redRect = std::make_unique<karin::gui::RectangleNode>(karin::Size(100, 100), karin::Color(karin::Color::Red));
    auto greenRect = std::make_unique<karin::gui::RectangleNode>(karin::Size(100, 100), karin::Color(karin::Color::Green));
    auto blueRect = std::make_unique<karin::gui::RectangleNode>(karin::Size(100, 100), karin::Color(karin::Color::Blue));

    rootView->addChild(std::move(redRect));
    rootView->addChild(std::move(greenRect));
    rootView->addChild(std::move(blueRect));

    rootView->setLayoutDirection(karin::gui::ContainerNode::LayoutDirection::Row);
    rootView->setGap(10.0f);

    window.setRootView(std::move(rootView));
    window.run();
}