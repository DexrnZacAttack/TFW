//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#include "TestFramework/tui/components/TestStatusComponent.h"

#include "TestFramework/TestFramework.h"
#include <fmt/format.h>

namespace tfw::tui::components {
    TestStatusComponent::TestStatusComponent(TUI &tui, TestFramework &framework) : m_tui(tui), m_framework(framework) {
    }

    ftxui::Element TestStatusComponent::OnRender() {
        int total = m_framework.tests().size();
        int failed = 0;
        int succeeded = 0;
        int waiting = 0;

        std::ranges::for_each(m_framework.tests(), [&waiting, &succeeded, &failed](const std::unique_ptr<test::AbstractTest> &test) {
            if (!test->watch().started() || !test->watch().stopped()) {
                waiting++;
                return;
            }

            if (test->result().has_value()) {
                if (test->result().value().passed) {
                    succeeded++;
                } else {
                    failed++;
                }
            }
        });

        if (waiting == 0) {
            return ftxui::hbox({
                ftxui::text(fmt::format("{}/{} ({} succeeded, {} failed)", succeeded + failed, total, succeeded, failed))
            });
        }

        return ftxui::hbox({
            ftxui::text(fmt::format("Running tests: {}/{} ({} succeeded, {} failed)", succeeded + failed, total, succeeded, failed))
        });
    }
}