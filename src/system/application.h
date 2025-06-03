#ifndef SRC_SYSTEM_APPLICATION_H
#define SRC_SYSTEM_APPLICATION_H
#include <memory>

#include "application_impl.h"

namespace karin
{

class Application
{
public:
    static Application& instance();

    void run();

private:
    Application() = default;
    ~Application() = default;

    std::unique_ptr<IApplicationImpl> impl_;
};

} // karin

#endif //SRC_SYSTEM_APPLICATION_H
