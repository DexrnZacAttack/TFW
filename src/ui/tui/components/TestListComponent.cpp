//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#include "TestFramework/ui/tui/components/TestListComponent.h"

#include "TestFramework/test/tests/Test.h"
#include "TestFramework/TestFramework.h"
#include "TestFramework/ui/tui/TerminalUserInterface.h"
#include "TestFramework/ui/tui/components/TestButton.h"
#include "TestFramework/ui/tui/components/TestStatusComponent.h"
#include "ftxui/component/component.hpp"
#include "TestFramework/test/tests/AbstractTest.h"

#include <ranges>

namespace tfw::ui::tui::components {
    TestListComponent::TestListComponent(TerminalUserInterface &tui, TestFramework &framework) : m_tui(tui), m_framework(framework) {
#ifdef __cpp_lib_ranges_enumerate
        this->m_buttons = this->m_framework.testManager().tests()
        | std::views::enumerate
        | std::views::transform([&](const std::tuple<long, const std::unique_ptr<tfw::test::tests::AbstractTest> &> &p) {
            auto [idx, test] = p;
            return this->transformTest(idx, test.get());
        })
        | std::ranges::to<std::vector>();
#else
        this->m_buttons = std::views::iota(static_cast<size_t>(0), this->m_framework.testManager().tests().size())
        | std::views::transform([&](const size_t idx) {
            return this->transformTest(idx, this->m_framework.testManager().testAt(idx));
        })
        | std::ranges::to<std::vector>();
#endif

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

    ftxui::Component TestListComponent::transformTest(long idx, tfw::test::tests::AbstractTest *test) const {
        auto button = TestButton::create(test, [test, this](auto&) {
            return this->m_tui.switchObservedTest(test);
        });

        return button;
    }
}