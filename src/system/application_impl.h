#ifndef SRC_SYSTEM_IAPPLICATIONIMPL_H
#define SRC_SYSTEM_IAPPLICATIONIMPL_H

#include <karin/system/event.h>

namespace karin
{
class IApplicationImpl
{
public:
    virtual ~IApplicationImpl() = default;

    virtual void shutdown() = 0;

    virtual bool waitEvent(Event& event) = 0;
};
} // karin

#endif //SRC_SYSTEM_IAPPLICATIONIMPL_H