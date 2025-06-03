#ifndef SRC_SYSTEM_PLATFORM_H
#define SRC_SYSTEM_PLATFORM_H

#include <memory>

#include "application_impl.h"

#ifdef KARIN_PLATFORM_WINDOWS
#include "windows/win_application_impl.h"
#endif

namespace karin
{
inline std::unique_ptr<IApplicationImpl> createApplicationImpl()
{
#ifdef KARIN_PLATFORM_WINDOWS
    return std::make_unique<WinApplicationImpl>();
#endif

    return nullptr;
}
}

#endif //SRC_SYSTEM_PLATFORM_H
