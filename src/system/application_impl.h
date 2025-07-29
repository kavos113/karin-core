#ifndef SRC_SYSTEM_IAPPLICATIONIMPL_H
#define SRC_SYSTEM_IAPPLICATIONIMPL_H

namespace karin
{
class IApplicationImpl
{
public:
    virtual ~IApplicationImpl() = default;

    virtual void run() = 0;
    virtual void shutdown() = 0;
};
} // karin

#endif //SRC_SYSTEM_IAPPLICATIONIMPL_H
