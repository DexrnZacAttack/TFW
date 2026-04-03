/** @file TestManager.h
 *
 * @author DexrnZacAttack
 * @date 3/30/26
 *
 * @device zPc-i2
 */
#ifndef TFW_TESTMANAGER_H
#define TFW_TESTMANAGER_H
#include "TestFramework/test/result/TestResult.h"
#include "TestFramework/test/tests/ComparableTest.h"

#include <atomic>
#include <functional>
#include <memory>

namespace tfw::ui {
    class IUserInterface;
}

namespace tfw::test {
    class TestManager {
      public:
        struct StagedTestRunner {
            explicit StagedTestRunner(tests::AbstractTest *test) : m_test(test) {}

            StagedTestRunner &then(const std::function<void(tests::AbstractTest *, result::TestResult)> &onFinish);

            void run();

          private:
            tests::AbstractTest *m_test;

            std::vector<std::function<void(tests::AbstractTest *, result::TestResult)>> m_onFinish;
        };

        explicit TestManager(ui::IUserInterface *ui);

        const std::vector<std::unique_ptr<tests::AbstractTest>> &tests() const { return this->m_tests; }

        std::vector<std::unique_ptr<tests::AbstractTest>> &tests() { return this->m_tests; }

        const std::atomic<tests::AbstractTest *> &currentTest() const { return this->m_runningTest; }

        std::atomic<tests::AbstractTest *> &currentTest() { return this->m_runningTest; }

        [[nodiscard]] StagedTestRunner stage(tests::AbstractTest *test);

        const std::unordered_map<tests::AbstractTest *, const test::result::TestResult> &results() const { return this->m_results; }

        std::unordered_map<tests::AbstractTest *, const test::result::TestResult> &results() { return this->m_results; }

        template <typename Func, typename... Args> constexpr void addTest(const bool condition, std::unique_ptr<tests::Test<Func, Args...>> test) {
            if (condition)
                this->m_tests.push_back(std::move(test));
        }

        template <typename Return, typename Func, typename... Args>
            requires std::equality_comparable<Return>
        constexpr void addComparableTest(const bool condition, std::unique_ptr<tests::ComparableTest<Return, Func, Args...>> test) {
            if (condition)
                this->m_tests.push_back(std::move(test));
        }

        template <typename Return, typename Func, typename... Args>
            requires std::equality_comparable<Return>
        constexpr void addComparableTest(std::unique_ptr<tests::ComparableTest<Return, Func, Args...>> test) {
            this->addComparableTest(true, std::move(test));
        }

        template <typename Func, typename... Args> constexpr void addTest(std::unique_ptr<tests::Test<Func, Args...>> test) {
            this->addTest(true, std::move(test));
        }

        template <typename Func, typename... Args> constexpr void addTest(const std::string_view &name, Func &&f, Args &&...args) {
            this->addTest<Func, Args...>(true, std::move(std::make_unique<tests::Test<Func, Args...>>(name, this->m_ui, false, std::forward<Func>(f),
                                                                                                     std::forward<Args>(args)...)));
        }

        template <typename Func, typename... Args>
        constexpr void addTest(const bool condition, const std::string_view &name, Func &&f, Args &&...args) {
            this->addTest(condition, std::move(std::make_unique<tests::Test<Func, Args...>>(name, this->m_ui, false, std::forward<Func>(f),
                                                                                           std::forward<Args>(args)...)));
        }

        template <typename Return, typename Func, typename... Args>
            requires std::equality_comparable<Return>
        constexpr void addComparableTest(const std::string_view &name, const Return expected, Func &&f, Args &&...args) {
            this->addComparableTest(true, std::move(std::make_unique<tests::ComparableTest<Return, Func, Args...>>(
                                              name, this->m_ui, expected, false, std::forward<Func>(f), std::forward<Args>(args)...)));
        }

        template <typename Return, typename Func, typename... Args>
            requires std::equality_comparable<Return>
        constexpr void addComparableTest(const bool condition, const std::string_view &name, const Return expected, Func &&f, Args &&...args) {
            this->addComparableTest(condition, std::move(std::make_unique<tests::ComparableTest<Return, Func, Args...>>(
                                                   name, this->m_ui, expected, false, std::forward<Func>(f), std::forward<Args>(args)...)));
        }

        template <typename Func, typename... Args> constexpr void addThrowingTest(const std::string_view &name, Func &&f, Args &&...args) {
            this->addTest<Func, Args...>(true, std::move(std::make_unique<tests::Test<Func, Args...>>(name, this->m_ui, true, std::forward<Func>(f),
                                                                                                     std::forward<Args>(args)...)));
        }

        template <typename Func, typename... Args>
        constexpr void addThrowingTest(const bool condition, const std::string_view &name, Func &&f, Args &&...args) {
            this->addTest(condition, std::move(std::make_unique<tests::Test<Func, Args...>>(name, this->m_ui, true, std::forward<Func>(f),
                                                                                           std::forward<Args>(args)...)));
        }

        template <typename Return, typename Func, typename... Args>
            requires std::equality_comparable<Return>
        constexpr void addThrowingComparableTest(const std::string_view &name, const Return expected, Func &&f, Args &&...args) {
            this->addComparableTest(true, std::move(std::make_unique<tests::ComparableTest<Return, Func, Args...>>(
                                              name, this->m_ui, expected, true, std::forward<Func>(f), std::forward<Args>(args)...)));
        }

        template <typename Return, typename Func, typename... Args>
            requires std::equality_comparable<Return>
        constexpr void addThrowingComparableTest(const bool condition, const std::string_view &name, const Return expected, Func &&f,
                                                 Args &&...args) {
            this->addComparableTest(condition, std::move(std::make_unique<tests::ComparableTest<Return, Func, Args...>>(
                                                   name, this->m_ui, expected, true, std::forward<Func>(f), std::forward<Args>(args)...)));
        }

      private:
        ui::IUserInterface *m_ui;

        std::atomic<tests::AbstractTest *> m_runningTest;

        std::vector<std::unique_ptr<tests::AbstractTest>> m_tests;
        std::unordered_map<tests::AbstractTest *, const test::result::TestResult> m_results;
    };
} // namespace tfw::test

#endif // TFW_TESTMANAGER_H