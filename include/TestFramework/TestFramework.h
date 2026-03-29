//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#ifndef TFW_TESTFRAMEWORK_H
#define TFW_TESTFRAMEWORK_H
#include "TestFramework/test/ComparableTest.h"
#include "TestFramework/test/Test.h"
#include "TestFramework/ui/EUserInterface.h"
#include "TestFramework/ui/tui/TerminalUserInterface.h"
#include "ftxui/dom/elements.hpp"

#include <concepts>
#include <future>
#include <memory>

namespace tfw {
    class TestFramework {
    public:
        explicit TestFramework(ui::EUserInterface ui);

        /** Parses args from the main function and returns a new TestFramework
         *
         * ### Args:
         * - @code--ui-mode@endcode: Selects which user interface to use
         *   - terminal
         *   - headless
         */
        static TestFramework fromArgs(int argc, char *argv[]);

        template <typename Func, typename... Args>
        constexpr void addTest(const bool condition, std::unique_ptr<test::Test<Func, Args...> > test) {
            if (condition)
                this->m_tests.push_back(std::move(test));
        }

        template <typename Return, typename Func, typename... Args>
            requires std::equality_comparable<Return>
        constexpr void addComparableTest(const bool condition, std::unique_ptr<test::ComparableTest<Return, Func, Args...> > test) {
            if (condition)
                this->m_tests.push_back(std::move(test));
        }

        template <typename Return, typename Func, typename... Args>
            requires std::equality_comparable<Return>
        constexpr void addComparableTest(std::unique_ptr<test::ComparableTest<Return, Func, Args...> > test) {
            this->addComparableTest(true, std::move(test));
        }

        template <typename Func, typename... Args>
        constexpr void addTest(std::unique_ptr<test::Test<Func, Args...> > test) {
            this->addTest(true, std::move(test));
        }

        template <typename Func, typename... Args>
        constexpr void addTest(const std::string_view &name, Func &&f, Args &&... args) {
            this->addTest<Func, Args...>(
                true, std::move(std::make_unique<test::Test<Func, Args...> >(name, this->m_ui.get(), std::forward<Func>(f), std::forward<Args>(args)...)));
        }

        template <typename Func, typename... Args>
        constexpr void addTest(const bool condition, const std::string_view &name, Func &&f, Args &&... args) {
            this->addTest(condition, std::move(
                              std::make_unique<test::Test<Func, Args...> >(name, this->m_ui.get(), std::forward<Func>(f), std::forward<Args>(args)...)));
        }

        template <typename Return, typename Func, typename... Args>
            requires std::equality_comparable<Return>
        constexpr void addComparableTest(const std::string_view &name, const Return expected, Func &&f, Args &&... args) {
            this->addComparableTest(true, std::move(
                                        std::make_unique<test::ComparableTest<Return, Func, Args...> >(
                                            name, this->m_ui.get(), expected, std::forward<Func>(f), std::forward<Args>(args)...)));
        }

        template <typename Return, typename Func, typename... Args>
            requires std::equality_comparable<Return>
        constexpr void addComparableTest(const bool condition, const std::string_view &name, const Return expected, Func &&f, Args &&... args) {
            this->addComparableTest(condition, std::move(
                                        std::make_unique<test::ComparableTest<Return, Func, Args...> >(
                                            name, this->m_ui.get(), expected, std::forward<Func>(f), std::forward<Args>(args)...)));
        }

        void run();

        const std::vector<std::unique_ptr<test::AbstractTest> > &tests() {
            return this->m_tests;
        }

        const std::atomic<test::AbstractTest *> &currentTest() {
            return this->m_runningTest;
        }

        const std::unordered_map<int, std::pair<const test::AbstractTest *, const test::result::TestResult> > &results() {
            return this->m_results;
        }

    private:
        void exit();

    private:
        std::unique_ptr<tfw::ui::IUserInterface> m_ui;

        /// Thread that runs the tasks
        std::jthread m_taskRunnerThread;

        std::atomic<test::AbstractTest *> m_runningTest;
        std::future<const test::result::TestResult &> m_currentFuture;

        std::vector<std::unique_ptr<test::AbstractTest> > m_tests;
        std::unordered_map<int, std::pair<const test::AbstractTest *, const test::result::TestResult> > m_results;
    };
} // tfw

#endif // TFW_TESTFRAMEWORK_H