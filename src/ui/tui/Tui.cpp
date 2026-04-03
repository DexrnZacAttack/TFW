//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#include "TestFramework/ui/tui/TerminalUserInterface.h"

#include "TestFramework/test/tests/Test.h"
#include "TestFramework/TestFramework.h"
#include "TestFramework/ui/tui/components/TestListComponent.h"
#include "ftxui/Scroller.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/loop.hpp"

namespace tfw::ui::tui {
    TerminalUserInterface::TerminalUserInterface(TestFramework &framework) : IUserInterface(framework),
                                         m_screen(
                                             ftxui::ScreenInteractive::Fullscreen()) {
        this->m_screen.TrackMouse(true);
    }

    void TerminalUserInterface::run() {
        std::shared_ptr<components::TestListComponent> list = ftxui::Make<components::TestListComponent>(*this, this->m_framework);
        ftxui::Component output = ftxui::Scroller(ftxui::Renderer([this] {
                        return ftxui::paragraph(this->currentOutput != nullptr ? this->currentOutput->str() : "");
        }));

        ftxui::Component windows = ftxui::Container::Horizontal({});
        windows->Add(list);
        windows->Add(output);

        auto renderer = ftxui::Renderer(windows, [this, windows, list, output] {
            std::string testName = "No test selected";
            if (this->currentTest != nullptr) {
                testName = std::string(this->currentTest.load()->name());
            }

            return ftxui::hbox({
                //window on left side of screen showing tests
                ftxui::window(ftxui::text("Tests"), list->Render())
                | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 75),
                //window on right side of screen showing focused test log output
                ftxui::window(
                    ftxui::text(testName),
                    [this, &output] {
                            const tfw::test::tests::AbstractTest *t = this->currentTest.load();

                            if (t != nullptr && t->result().has_value()) {
                                test::result::TestResult res = t->result().value();

                                return ftxui::vbox({
                                    output->Render(),
                                    ftxui::hbox({
                                        // idk if this is the right char, just yoinked from wikipedia
                                        // wish the window would do the connection without the separator
                                        ftxui::separatorCharacter("─") | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 1),
                                        ftxui::text("Status"),
                                        ftxui::separator() | ftxui::flex
                                    }),
                                    ftxui::paragraph(res.toString())
                                });
                            }

                            return output->Render();
                        }()
                )
                | ftxui::flex
            });
        });
        renderer |= ftxui::CatchEvent([&](const ftxui::Event &ev) {
            if (ev == ftxui::Event::Character('q')) {
                this->m_screen.Exit();
                return true;
            }

            if (ev == ftxui::Event::Character('t')) {
                return this->switchObservedTest(this->m_framework.testManager().currentTest());
            }

            return false;
        });

        auto loop = ftxui::Loop(&this->m_screen, renderer);

        this->m_screen.Loop(renderer);
    }

    bool TerminalUserInterface::switchObservedTest(tfw::test::tests::AbstractTest *test) {
        if (test != nullptr && test->watch().started()) {
            this->currentTest = test;
            this->currentOutput = &static_cast<test::logging::loggers::TuiTestLogger *>(test->logger())->stream();
            this->postEvent(Event::SWITCH_OBSERVED_TEST);

            return true;
        }

        return false;
    }

    test::logging::loggers::ITestLogger * TerminalUserInterface::makeLogger() {
        return new test::logging::loggers::TuiTestLogger(this);
    }

}