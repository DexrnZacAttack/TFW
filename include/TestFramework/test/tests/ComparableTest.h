//
// Created by DexrnZacAttack on 2/27/26 using zPc-i2.
//
#ifndef TFW_COMPARABLETEST_H
#define TFW_COMPARABLETEST_H
#include "TestFramework/test/tests/Test.h"

namespace tfw::test::tests {
    template<typename Return, typename Func, typename... Args>
        requires std::equality_comparable<Return> && std::invocable<Func, tfw::test::logging::loggers::ITestLogger &, Args...>
    class ComparableTest : public Test<Func, Args...> {
    public:
        explicit constexpr ComparableTest(const std::string_view &name, ui::IPostable *postable, logging::ITestLoggerProvider *loggerProvider, const bool shouldThrow, const Return expected, Func&& f, Args&&... args) : Test<Func, Args...>(name, postable, loggerProvider, shouldThrow, std::forward<Func>(f), std::forward<Args>(args)...), m_expected(expected) {}

        template <PostableLoggerProvider Provider>
        explicit constexpr ComparableTest(const std::string_view &name, Provider *postableLoggerProvider, const bool shouldThrow, const Return expected, Func&& f, Args&&... args) : ComparableTest(name, postableLoggerProvider, postableLoggerProvider, expected, shouldThrow, std::forward<Func>(f), std::forward<Args>(args)...) {}

    protected:
        constexpr const result::TestResult &run() override {
            auto args = std::tuple_cat(std::forward_as_tuple(*this->m_logger), this->m_args);

            this->m_watch.start();

            try {
                Return r = std::apply(this->m_test, args);
                this->m_watch.stop();//stop early before we do the comparison, otherwise the result is off by ~500ns

                if (r == m_expected) {
                    this->m_result.emplace(r == m_expected, "Return value matches the expected value");
                } else {
                    this->m_result.emplace(r == m_expected, "Return value does not match expected value");
                }
            } catch (...) {
                this->m_watch.stop();
                this->m_result.emplace(false, std::current_exception());
            }

            return this->m_result.value();
        }

        constexpr const result::TestResult &runNoCatch() override {
            auto args = std::tuple_cat(std::forward_as_tuple(*this->m_logger), this->m_args);

            this->m_watch.start();

            Return r = std::apply(this->m_test, args);
            this->m_watch.stop();//stop early before we do the comparison, otherwise the result is off by ~500ns

            if (r == m_expected) {
                this->m_result.emplace(r == m_expected, "Return value matches the expected value");
            } else {
                this->m_result.emplace(r == m_expected, "Return value does not match expected value");
            }

            return this->m_result.value();
        }
    private:
        const Return m_expected;
    };
}

#endif // TFW_COMPARABLETEST_H