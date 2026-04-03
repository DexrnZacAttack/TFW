/** @file TuiTestLogger.h
 *
 * @author DexrnZacAttack
 * @date 3/28/26
 * 
 * @device zPc-i2
 */
#ifndef TFW_TUITESTLOGGER_H
#define TFW_TUITESTLOGGER_H
#include "TestFramework/test/logging/loggers/ITestLogger.h"
#include <sstream>

namespace tfw::test::logging::loggers {
    class TuiTestLogger : public logging::loggers::ITestLogger {
    public:
        explicit TuiTestLogger(ui::IPostable *postable) : ITestLogger(postable) {}

        const std::stringstream &stream() const noexcept {
            return this->m_stream;
        }

        std::stringstream &stream() noexcept {
            return this->m_stream;
        }

        std::istream &inputStream() noexcept override {
            return this->m_stream;
        }

        std::ostream &outputStream() noexcept override {
            return this->m_stream;
        }

        const std::istream &inputStream() const noexcept override {
            return this->m_stream;
        }

        const std::ostream &outputStream() const noexcept override {
            return this->m_stream;
        }

    private:
        std::stringstream m_stream;
    };
}

#endif // TFW_TUITESTLOGGER_H