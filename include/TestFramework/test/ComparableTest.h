//
// Created by DexrnZacAttack on 2/27/26 using zPc-i2.
//
#ifndef TFW_COMPARABLETEST_H
#define TFW_COMPARABLETEST_H
#include "Test.h"

namespace tfw {
    namespace test {
        template<typename Return, typename Func, typename... Args>
        requires std::equality_comparable<Return> && std::invocable<Func, tfw::test::util::TestOutputLogger &, Args...>
        class ComparableTest : public Test<Func, Args...> {
        public:
            explicit constexpr ComparableTest(const std::string_view &name, tui::IPostable &postable, const Return expected, Func&& f, Args&&... args) : Test<Func, Args...>(name, postable, std::forward<Func>(f), std::forward<Func>(args)...), m_expected(expected) {}

            constexpr const result::TestResult &operator()() override {
                this->m_watch.start();

                try {
                    Return r = this->m_test(this->m_logger, std::forward<Args>(this->m_args)...);
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

        private:
            const Return m_expected;
        };
    } // tests
} // tfw

#endif // TFW_COMPARABLETEST_H