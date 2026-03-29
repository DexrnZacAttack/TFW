/** @file ITestLoggerProvider.h
 *
 * @author DexrnZacAttack
 * @date 3/28/26
 * 
 * @device zPc-i2
 */
#ifndef TFW_ITESTLOGGERPROVIDER_H
#define TFW_ITESTLOGGERPROVIDER_H
#include "TestFramework/test/logging/loggers/ITestLogger.h"

namespace tfw::test::logging {
    class ITestLoggerProvider {
    public:
        virtual ~ITestLoggerProvider() = default;

        virtual logging::loggers::ITestLogger *makeLogger() = 0;
    };
}

#endif // TFW_ITESTLOGGERPROVIDER_H