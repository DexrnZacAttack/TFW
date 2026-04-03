//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#ifndef TFW_TUI_H
#define TFW_TUI_H
#include "TestFramework/test/logging/ITestLoggerProvider.h"
#include "TestFramework/ui/IPostable.h"
#include "TestFramework/ui/IUserInterface.h"

#include "ftxui/component/screen_interactive.hpp"

namespace tfw::ui::tui {
    class TerminalUserInterface : public ui::IUserInterface {
    public:
        explicit TerminalUserInterface(TestFramework &framework);

        void run() override;

        bool switchObservedTest(tfw::test::tests::AbstractTest *test);

        void postEvent(const ui::Event &ev) override {
            this->m_screen.PostEvent(ftxui::Event::Custom);
        }

        IPostable &asPostable() {
            return *this;
        }

        test::logging::loggers::ITestLogger *makeLogger() override;

    protected:
        //components
        ftxui::ScreenInteractive m_screen;
        // const ftxui::Component m_renderer;
        // ftxui::Loop m_loop;
    };
}

#endif // TFW_TUI_H