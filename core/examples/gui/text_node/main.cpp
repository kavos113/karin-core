#include <karin/common.h>
#include <karin/gui.h>

#include <memory>
#include <random>

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    karin::gui::Application app;
    auto window = app.createWindow("Hello Layout", 100, 100, 800, 600);

    auto rootView = std::make_unique<karin::gui::ContainerNode>(karin::Size(800, 600));

    for (int i = 0; i < 10; ++i)
    {
        auto rect = std::make_unique<karin::gui::RectangleNode>(
            karin::Size(100, 100),
            karin::Color(dis(gen), dis(gen), dis(gen))
        );
        rootView->addChild(std::move(rect));
    }

    karin::SolidColorPattern pattern(karin::Color(0, 0, 255));
    auto textNode = std::make_unique<karin::gui::TextNode>(
        "Hello, World! This is a long text to test the wrapping behavior of the TextNode.",
        karin::TextStyle(16.0f, "Arial"),
        karin::ParagraphStyle(),
        pattern
    );
    rootView->addChild(std::move(textNode));

    rootView->setLayoutDirection(karin::gui::ContainerNode::LayoutDirection::Row);
    rootView->setGap(10.0f);
    rootView->setWrapMode(karin::gui::ContainerNode::WrapMode::Wrap);

    window->setRootView(std::move(rootView));
    app.run();
}