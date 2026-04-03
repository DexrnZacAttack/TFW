//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#include "TestFramework/ui/tui/components/TestListComponent.h"

#include "../../../../include/TestFramework/test/tests/Test.h"
#include "TestFramework/TestFramework.h"
#include "TestFramework/ui/tui/TerminalUserInterface.h"
#include "TestFramework/ui/tui/components/TestButton.h"
#include "TestFramework/ui/tui/components/TestStatusComponent.h"
#include "ftxui/component/component.hpp"

#include <ranges>

namespace tfw::ui::tui::components {
    TestListComponent::TestListComponent(TerminalUserInterface &tui, TestFramework &framework) : m_tui(tui), m_framework(framework) {
        this->m_buttons = this->m_framework.testManager().tests()
        | std::views::enumerate
        | std::views::transform([&](auto p) {
            return this->transformTest(p);
        })
        | std::ranges::to<std::vector>();

        this->m_list = ftxui::Container::Vertical(this->m_buttons, &this->m_focused);
        Add(this->m_list);
    }

    ftxui::Element TestListComponent::OnRender() {
        const float pos = m_buttons.empty()
                  ? 0.0f
                  : static_cast<float>(this->m_focused) / static_cast<float>(this->m_buttons.size());

        return ftxui::vbox({
            // button list
            this->m_list->Render() | ftxui::focusPositionRelative(0.0f, pos) | ftxui::vscroll_indicator | ftxui::yframe | ftxui::flex,
            ftxui::separator(),
            // ftxui::text(std::to_string(this->m_focused)), //debug
            //bottom window
            TestStatusComponent(this->m_tui, this->m_framework).Render()
        });
    }

    ftxui::Component TestListComponent::transformTest(
        std::tuple<long, const std::unique_ptr<tfw::test::tests::AbstractTest> &> p) const {
            auto [i, test] = p;

            auto button = TestButton::create(test.get(), [&test, this](auto&) {
                return this->m_tui.switchObservedTest(test.get());
            });

            return button;
    }
}