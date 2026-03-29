//
// Created by DexrnZacAttack on 2/28/26 using zPc-i2.
//
#ifndef TFW_TESTOUTPUTLOGGER_H
#define TFW_TESTOUTPUTLOGGER_H
#include "TestFramework/ui/IPostable.h"
#include "ftxui/component/screen_interactive.hpp"

#include <iosfwd>
#include <iostream>
#include <ostream>

namespace tfw::test::logging::loggers {
    class ITestLogger {
    public:
        explicit ITestLogger(ui::IPostable *postable) : m_postable(postable) {}
        virtual ~ITestLogger() = default;

        template<typename T>
        ITestLogger& operator<<(const T& value) {
            this->outputStream() << value;

            this->m_postable->postEvent(ui::Event::TEST_LOG_OUTPUT);

            return *this;
        }

        ITestLogger& operator<<(std::ostream &(*value)(std::ostream &)) {
            this->outputStream() << value;
            this->m_postable->postEvent(ui::Event::TEST_LOG_OUTPUT);

            return *this;
        }

        virtual std::istream &inputStream() noexcept = 0;
        virtual std::ostream &outputStream() noexcept = 0;
        virtual const std::istream &inputStream() const noexcept = 0;
        virtual const std::ostream &outputStream() const noexcept = 0;

    protected:
        ui::IPostable *m_postable;
    };
}

#endif // TFW_TESTOUTPUTLOGGER_H