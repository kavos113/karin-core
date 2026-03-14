#include <karin/system.h>
#include <karin/graphics.h>
#include <karin/common.h>

int main()
{
    karin::Application& app = karin::Application::instance();
    std::unique_ptr<karin::Window> window = app.createWindow(L"Hello Graphics", 100, 100, 800, 600);

    karin::Renderer renderer(window.get());
    renderer.setClearColor(karin::Color(karin::Color::Green));

    karin::TextEngine textEngine;

    karin::Pattern magentaPattern = karin::SolidColorPattern(karin::Color(255, 0, 255));
    karin::TextBlob textBlob = textEngine.layoutText(
        "日本語も描画できます。とても長い文章の場合、改行やトリミングが適切に行われるか確認します。",
        karin::TextStyle(24.0f, "Meiryo UI"),
        karin::ParagraphStyle{
            .lineSpacing = 1.5f,
            .baseline = 1.0f,
            .lineSpacingMode = karin::ParagraphStyle::LineSpacingMode::PROPORTIONAL,
            .trimming = karin::ParagraphStyle::Trimming::CHARACTER,
            .wrapping = karin::ParagraphStyle::Wrapping::WORD,
        },
        {400.0f, 200.0f}
    );

    renderer.addDrawCommand(
        [&magentaPattern, &textBlob](karin::GraphicsContext& gc)
        {
            gc.drawRect(karin::Rectangle(50.0f, 50.0f, 400.0f, 200.0f), magentaPattern);
            gc.drawText(textBlob, karin::Point(50.0f, 50.0f), magentaPattern);
        }
    );

    renderer.update();

    window->setStatus(karin::Window::ShowStatus::SHOW);

    karin::Event event;
    while (app.waitEvent(event)) {}

    renderer.cleanUp();

    return 0;
}
