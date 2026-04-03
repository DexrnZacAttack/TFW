//
// Created by DexrnZacAttack on 3/4/26 using zPc-i2.
//
#include "TestFramework/ui/tui/components/TestButton.h"

#include "../../../../include/TestFramework/test/tests/Test.h"
#include "TestFramework/ui/tui/util/ColorUtil.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"

#include <fmt/format.h>

namespace tfw::ui::tui::components {
    TestButton::TestButton(const Options &options) : m_hovered(
        false), m_options(options) {
    }

    TestButton::TestButton(tfw::test::tests::AbstractTest *test,
                           const std::function<void(TestButton &button)> &invokeEvent, const Options &options)
        : TestButton(options) {
        this->m_options.invokeEvent = invokeEvent;
        this->m_options.test = test;
    }

    TestButton::TestButton(tfw::test::tests::AbstractTest *test,
                           const std::function<void(TestButton &button)> &invokeEvent) : TestButton(test, invokeEvent, {}) {
    }

    std::shared_ptr<TestButton> TestButton::create(
        tfw::test::tests::AbstractTest *test,
        std::function<void(TestButton &button)> invokeEvent,
        Options options) {
        return ftxui::Make<TestButton>(test, invokeEvent, options);
    }

    std::shared_ptr<TestButton> TestButton::create(
        tfw::test::tests::AbstractTest *test,
        std::function<void(TestButton &button)> invokeEvent) {
        return ftxui::Make<TestButton>(test, invokeEvent);
    }

    ftxui::Element TestButton::OnRender() {
        ftxui::Decorator fg;
        ftxui::Decorator bg;

        auto [foreground, background] = TestButton::getColorOptions(this->m_options.test);

        if (this->Focused()) {
            fg = ftxui::color(foreground.active);
            bg = ftxui::bgcolor(background.active);
        } else if (this->m_hovered) {
            fg = ftxui::color(foreground.hover);
            bg = ftxui::bgcolor(background.hover);
        } else {
            fg = ftxui::color(foreground.inactive);
            bg = ftxui::bgcolor(background.inactive);
        }

        ftxui::Element el = ftxui::text(std::string(this->m_options.test->name()));
        if (this->m_options.test->isComplete()) {
            el = ftxui::hbox({
                el | ftxui::flex,
                ftxui::separator(),
                ftxui::text(fmt::format("{}", this->m_options.test->watch().duration())) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20)
            });
        }

        return (el  | ftxui::border | fg | bg)
               | ftxui::reflect(m_bb);
    }

    bool TestButton::OnEvent(ftxui::Event event) {
        if (event.is_mouse()) {
            ftxui::Mouse ev = event.mouse();

            this->m_hovered = this->m_bb.Contain(ev.x, ev.y);

            if (this->active() && this->m_hovered
                && ev.motion == ev.Released
                && ev.button == ftxui::Mouse::Left) {
                this->TakeFocus();
                this->m_options.invokeEvent(*this);

                return true;
            }
        } else if (event == ftxui::Event::Return || event == ftxui::Event::Character(' ')) {
            this->TakeFocus();
            this->m_options.invokeEvent(*this);

            return true;
        }

        return false;
    }

    TestButton::Options &TestButton::options() {
        return this->m_options;
    }

    bool TestButton::Focusable() const {
        return true;
    }

    bool TestButton::active() const {
        return this->m_options.test->watch().started();
    }

    ftxui::Color TestButton::getColor(const tfw::test::tests::AbstractTest *test) {
        ftxui::Color color;
        //if running
        if (test->watch().running()) {
            color = ftxui::Color::LightSkyBlue1; //skyblue
        } else if (test->result().has_value()) {
            if (test->result().value().passed) { // finished, passed
                color = ftxui::Color::Aquamarine3; //aquamarine
            } else { //finished, failed
                color = ftxui::Color::IndianRedBis; //red
            }
        }

        return color;
    }

    TestButton::Options::HoverableAnimatedColorOptions TestButton::
    getColorOptions(const tfw::test::tests::AbstractTest *test) {
        const ftxui::Color c = getColor(test);

        return {
            {
                {
                    .enabled = test->watch().started(),
                    .inactive = util::ColorUtil::darken(ftxui::Color::White, 0.2f),
                    .active = ftxui::Color::White,
                },
                util::ColorUtil::darken(ftxui::Color::White, 0.1f),
            },
            {
                {
                    .enabled = test->watch().started(),
                    .inactive = util::ColorUtil::darken(c, 0.6f),
                    .active = util::ColorUtil::darken(c, 0.8f),
                },
                util::ColorUtil::darken(c, 0.7f),
            }
        };
    }
}