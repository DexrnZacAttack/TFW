//
// Created by DexrnZacAttack on 2/27/26 using zPc-i2.
//
#ifndef TFW_TEST_H
#define TFW_TEST_H
#include "TestFramework/test/logging/ITestLoggerProvider.h"
#include "TestFramework/test/logging/loggers/TuiTestLogger.h"
#include "TestFramework/test/result/TestResult.h"
#include "TestFramework/test/tests/AbstractTest.h"
#include "TestFramework/test/util/ChronoWatch.h"

#include <exception>
#include <string_view>
#include <tuple>
#include <utility>

namespace tfw::test::tests {
    template <typename T>
    concept PostableLoggerProvider = std::is_base_of_v<ui::IPostable, T> && std::is_base_of_v<logging::ITestLoggerProvider, T>;

    template<typename Func, typename... Args>
    requires std::invocable<Func, logging::loggers::ITestLogger &, Args...>
    class Test : public AbstractTest {
    public:
        explicit constexpr Test(const std::string_view &name, ui::IPostable *postable, logging::ITestLoggerProvider *loggerProvider,
                              const bool shouldThrow, Func&& f, Args&&... args) : AbstractTest(name, postable, loggerProvider, shouldThrow), m_test(std::forward<Func>(f)), m_args(std::forward<Args>(args)...) {}

        template <PostableLoggerProvider Provider>
        explicit constexpr Test(const std::string_view &name, Provider *postableLoggerProvider, const bool shouldThrow, Func&& f, Args&&... args) : AbstractTest(name, postableLoggerProvider, postableLoggerProvider, shouldThrow), m_test(std::forward<Func>(f)), m_args(std::forward<Args>(args)...) {}

        constexpr const result::TestResult &operator()() override {
            if (this->m_shouldThrow) {
                return this->runNoCatch();
            }

            return this->run();
        }

    protected:
        // TODO this can be cleaned up, but I'm rushing this for a friend atm.
        constexpr const virtual result::TestResult &run() {
            auto args = std::tuple_cat(std::forward_as_tuple(*this->m_logger), this->m_args);

            this->m_watch.start();

            try {
                if constexpr (std::same_as<std::invoke_result_t<decltype(this->m_test), logging::loggers::ITestLogger &, Args...>, result::TestResult>) {
                    result::TestResult r = std::apply(this->m_test, args);
                    this->m_watch.stop();
                    this->m_result.emplace(r);
                } else if constexpr (std::same_as<std::invoke_result_t<decltype(this->m_test), logging::loggers::ITestLogger &, Args...>, void>) {
                    std::apply(this->m_test, args);
                    this->m_watch.stop();
                    this->m_result.emplace(true);
                } else if constexpr (std::is_same_v<decltype(*this), Test<Func, Args...>>) {
                    static_assert(false, "Provided function needs to return either void or a TestResult.");
                }
            } catch (...) {
                this->m_watch.stop();
                this->m_result.emplace(false, std::current_exception());
            }

            return this->m_result.value();
        }

        constexpr const virtual result::TestResult &runNoCatch() {
            auto args = std::tuple_cat(std::forward_as_tuple(*this->m_logger), this->m_args);

            this->m_watch.start();

            if constexpr (std::same_as<std::invoke_result_t<decltype(this->m_test), logging::loggers::ITestLogger &, Args...>, result::TestResult>) {
                result::TestResult r = std::apply(this->m_test, args);
                this->m_watch.stop();
                this->m_result.emplace(r);
            } else if constexpr (std::same_as<std::invoke_result_t<decltype(this->m_test), logging::loggers::ITestLogger &, Args...>, void>) {
                std::apply(this->m_test, args);
                this->m_watch.stop();
                this->m_result.emplace(true);
            } else if constexpr (std::is_same_v<decltype(*this), Test<Func, Args...>>) {
                static_assert(false, "Provided function needs to return either void or a TestResult.");
            }

            return this->m_result.value();
        }

    protected:
        Func m_test;
        std::tuple<Args...> m_args;
    };
}

#endif // TFW_TEST_H