#ifndef KARIN_SYSTEM_APPLICATION_H
#define KARIN_SYSTEM_APPLICATION_H
#include <memory>

namespace karin
{

class IApplicationImpl;

class Application
{
public:
    static Application& instance();

    void run();

private:
    Application();
    ~Application() = default;

    std::unique_ptr<IApplicationImpl> m_impl;
};

} // karin

#endif //KARIN_SYSTEM_APPLICATION_H
