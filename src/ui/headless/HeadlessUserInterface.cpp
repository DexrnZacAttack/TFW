/** @file HeadlessUserInterface.cpp
 *
 * @author DexrnZacAttack
 * @date 3/28/26
 *
 * @device zPc-i2
 */
#include "TestFramework/ui/headless/HeadlessUserInterface.h"

#include "TestFramework/TestFramework.h"
#include "TestFramework/test/Test.h"
#include "TestFramework/test/logging/loggers/ConsoleTestLogger.h"

#include <iostream>
#include <print>

namespace tfw::ui::headless {
    void HeadlessUserInterface::postEvent(const Event &ev) {
        if (ev == Event::TEST_STARTING) {
            const auto t = this->currentTest.load();

            std::println("");
            std::println("==============================");
            std::println("Starting test '{}'", t->name());
            std::println("==============================");
            std::println("");
        }

        if (ev == Event::TEST_FINISHED) {
            const auto t = this->currentTest.load();

            std::println("");
            std::println("==============================");
            std::println("Test '{}' finished in {}", t->name(), t->watch().duration());
            std::println("{}", t->result()->toString());
            std::println("==============================");
            ++m_testsRan;
        }
    }

    void HeadlessUserInterface::run() {
        //stall until all tests are finished
        while (m_testsRan != this->m_framework.tests().size()) {}

        //they're finished, print results
        std::println("\n");
        std::println("=============== Results ===============");
        for (auto &t : this->m_framework.tests()) {
            std::println("\nTest: '{}'", t->name());
            std::println("Duration: {}", t->watch().duration());
            if (t->result().has_value()) {
                std::println("{}", t->result()->toString());
            }
        }

        std::println("=======================================");
    }

    test::logging::loggers::ITestLogger * HeadlessUserInterface::makeLogger() {
        return new test::logging::loggers::ConsoleTestLogger(this);
    }
}