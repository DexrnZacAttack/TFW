//
// Created by DexrnZacAttack on 2/28/26 using zPc-i2.
//
#ifndef TFW_TESTOUTPUTLOGGER_H
#define TFW_TESTOUTPUTLOGGER_H
#include "TestFramework/tui/IPostable.h"
#include "ftxui/component/screen_interactive.hpp"

#include <iosfwd>
#include <ostream>

namespace tfw::test::util {
    class TestOutputLogger {
    public:
        TestOutputLogger(tui::IPostable &postable) : m_postable(postable) {
        }

        template<typename T>
        TestOutputLogger& operator<<(const T& value) {
            this->m_stream << value;
            this->m_postable.postEvent(ftxui::Event::Custom);

            return *this;
        }

        TestOutputLogger& operator<<(std::ostream &(*value)(std::ostream &)) {
            this->m_stream << value;
            this->m_postable.postEvent(ftxui::Event::Custom);

            return *this;
        }

        const std::stringstream &stream() const noexcept {
            return this->m_stream;
        }

    private:
        tui::IPostable &m_postable;
        std::stringstream m_stream;
    };
}

#endif // TFW_TESTOUTPUTLOGGER_H