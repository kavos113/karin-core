#include <karin/common.h>

#include <memory>
#include <random>

#include "window.h"
#include "container_node.h"
#include "rectangle_node.h"
#include "view_node.h"

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    karin::gui::Window window(L"Hello Layout", 100, 100, 800, 600);

    auto rootView = std::make_unique<karin::gui::ContainerNode>(karin::Size(800, 600));

    for (int i = 0; i < 10; ++i)
    {
        auto rect = std::make_unique<karin::gui::RectangleNode>(
            karin::Size(100, 100),
            karin::Color(dis(gen), dis(gen), dis(gen))
        );
        rootView->addChild(std::move(rect));
    }

    rootView->setLayoutDirection(karin::gui::ContainerNode::LayoutDirection::Row);
    rootView->setGap(10.0f);
    rootView->setWrapMode(karin::gui::ContainerNode::WrapMode::Wrap);

    window.setRootView(std::move(rootView));
    window.run();
}