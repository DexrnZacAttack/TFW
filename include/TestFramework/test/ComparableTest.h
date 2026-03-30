//
// Created by DexrnZacAttack on 2/27/26 using zPc-i2.
//
#ifndef TFW_COMPARABLETEST_H
#define TFW_COMPARABLETEST_H
#include "Test.h"

namespace tfw::test {
    template<typename Return, typename Func, typename... Args>
        requires std::equality_comparable<Return> && std::invocable<Func, tfw::test::logging::loggers::ITestLogger &, Args...>
    class ComparableTest : public Test<Func, Args...> {
    public:
        explicit constexpr ComparableTest(const std::string_view &name, ui::IPostable *postable, logging::ITestLoggerProvider *loggerProvider, const Return expected, Func&& f, Args&&... args) : Test<Func, Args...>(name, postable, loggerProvider, std::forward<Func>(f), std::forward<Args>(args)...), m_expected(expected) {}

        template <PostableLoggerProvider Provider>
        explicit constexpr ComparableTest(const std::string_view &name, Provider *postableLoggerProvider, const Return expected, Func&& f, Args&&... args) : ComparableTest(name, postableLoggerProvider, postableLoggerProvider, expected, std::forward<Func>(f), std::forward<Args>(args)...) {}

        constexpr const result::TestResult &operator()() override {
            auto args = std::tuple_cat(std::forward_as_tuple(*this->m_logger), this->m_args);

            this->m_watch.start();

#ifndef TFW_NO_SWALLOW_EXCEPTIONS
            try {
#endif
                Return r = std::apply(this->m_test, args);
                this->m_watch.stop();//stop early before we do the comparison, otherwise the result is off by ~500ns

                if (r == m_expected) {
                    this->m_result.emplace(r == m_expected, "Return value matches the expected value");
                } else {
                    this->m_result.emplace(r == m_expected, "Return value does not match expected value");
                }
#ifndef TFW_NO_SWALLOW_EXCEPTIONS
            } catch (...) {
                this->m_watch.stop();
                this->m_result.emplace(false, std::current_exception());
            }
#endif

            return this->m_result.value();
        }

    private:
        const Return m_expected;
    };
}

#endif // TFW_COMPARABLETEST_H