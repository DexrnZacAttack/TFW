/** @file ConsoleTestLogger.h
 *
 * @author DexrnZacAttack
 * @date 3/28/26
 * 
 * @device zPc-i2
 */
#ifndef TFW_CONSOLETESTLOGGER_H
#define TFW_CONSOLETESTLOGGER_H
#include "TestFramework/test/logging/loggers/ITestLogger.h"

namespace tfw::test::logging::loggers {
    class ConsoleTestLogger : public logging::loggers::ITestLogger {
    public:
        explicit ConsoleTestLogger(ui::IPostable *postable) : ITestLogger(postable) {}

        std::istream &inputStream() noexcept override {
            return std::cin;
        }
        const std::istream &inputStream() const noexcept override {
            return std::cin;
        }

        std::ostream &outputStream() noexcept override {
            return std::cout;
        }
        const std::ostream &outputStream() const noexcept override {
            return std::cout;
        }
    };
}

#endif // TFW_CONSOLETESTLOGGER_H