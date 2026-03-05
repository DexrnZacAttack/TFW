//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#ifndef TFW_TUI_H
#define TFW_TUI_H
#include "TestFramework/tui/IPostable.h"
#include "ftxui/component/loop.hpp"

#include "ftxui/component/screen_interactive.hpp"

#include <atomic>

namespace tfw {
    class TestFramework;
    namespace test {
        class AbstractTest;
    }
}

namespace tfw::tui {
    class TUI : public IPostable {
    public:
        explicit TUI(TestFramework &framework);

        void runRender();

        bool switchObservedTest(const test::AbstractTest *test);

        void postEvent(const ftxui::Event &ev) override {
            this->m_screen.PostEvent(ev);
        }

        IPostable &asPostable() {
            return *this;
        }
    private:
        //other
        TestFramework &m_framework;
    // fields
    public:
        //state
        std::atomic<const test::AbstractTest *> currentTest;
        const std::stringstream *currentOutput; // this is probably nasty when paired with threading, but it works so idc
    protected:
        //components
        ftxui::ScreenInteractive m_screen;
        // const ftxui::Component m_renderer;
        // ftxui::Loop m_loop;
    };
}

#endif // TFW_TUI_H