#include <karin/system.h>
#include <karin/graphics.h>
#include <karin/common.h>

#include <memory>

int main()
{
    karin::Application& app = karin::Application::instance();
    karin::Window window = app.createWindow(L"Hello Graphics", 100, 100, 800, 600);

    karin::Renderer renderer(&window, app.systemFont());
    renderer.setClearColor(karin::Color(karin::Color::Green));

    karin::Pattern magentaPattern = karin::SolidColorPattern(karin::Color(255, 0, 255));
    karin::TextLayout textLayout = {
        "日本語も描画できます。とても長い文章の場合、改行やトリミングが適切に行われるか確認します。",
        karin::TextFormat{
            .font = {
                // .family = "Meiryo UI",
                .family = "Ubuntu",
            },
            .size = 24.0f,
            .lineSpacing = 1.5f,
            .baseline = 1.0f,
            .lineSpacingMode = karin::TextFormat::LineSpacingMode::PROPORTIONAL,
            .trimming = karin::TextFormat::Trimming::CHARACTER,
            .wrapping = karin::TextFormat::Wrapping::WORD,
        },
        {400.0f, 200.0f},
    };

    renderer.addDrawCommand(
        [&magentaPattern, &textLayout](karin::GraphicsContext& gc)
        {
            gc.drawRect(karin::Rectangle(50.0f, 50.0f, 400.0f, 200.0f), magentaPattern);
            gc.drawText(textLayout, karin::Point(50.0f, 50.0f), magentaPattern);
        }
    );

    renderer.update();

    window.setStatus(karin::Window::ShowStatus::SHOW);

    karin::Event event;
    while (app.waitEvent(event)) {}

    renderer.cleanUp();

    return 0;
}
