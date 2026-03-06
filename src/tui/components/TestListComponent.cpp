//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#include "TestFramework/tui/components/TestListComponent.h"

#include "ftxui/component/component.hpp"
#include "TestFramework/TestFramework.h"
#include "TestFramework/test/Test.h"
#include "TestFramework/tui/Tui.h"
#include "TestFramework/tui/components/TestButton.h"
#include "TestFramework/tui/components/TestStatusComponent.h"

#include <ranges>

namespace tfw::tui::components {
    TestListComponent::TestListComponent(TUI &tui, TestFramework &framework) : m_tui(tui), m_framework(framework) {
        this->m_buttons = this->m_framework.tests()
        | std::views::enumerate
        | std::views::transform([&](auto p) {
            return this->transformTest(p);
        })
        | std::ranges::to<std::vector>();

        this->m_list = ftxui::Container::Vertical(this->m_buttons, &this->m_focused);
        Add(this->m_list);
    }

    ftxui::Element TestListComponent::OnRender() {
        const int sz = (this->m_buttons.size() <= 1)
            ? this->m_buttons.size()
            : ((this->m_focused) / (this->m_buttons.size() - 1));

        return ftxui::vbox({
            // button list
            this->m_list->Render() | ftxui::focusPositionRelative(0.0f, sz) | ftxui::flex | ftxui::vscroll_indicator | ftxui::yframe,
            ftxui::separator(),
            // ftxui::text(std::to_string(this->m_focused)), //debug
            //bottom window
            TestStatusComponent(this->m_tui, this->m_framework).Render()
        });
    }

    ftxui::Component TestListComponent::transformTest(
        std::tuple<long, const std::unique_ptr<test::AbstractTest> &> p) const {
            auto [i, test] = p;

            auto button = TestButton::create(test.get(), [&test, this](auto&) {
                return this->m_tui.switchObservedTest(test.get());
            });

            return button;
    }
}