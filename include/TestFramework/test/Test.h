//
// Created by DexrnZacAttack on 2/27/26 using zPc-i2.
//
#ifndef TFW_TEST_H
#define TFW_TEST_H
#include "TestFramework/test/exception/TestFailedException.h"
#include "TestFramework/test/util/ChronoWatch.h"
#include "fmt/args.h"
#include "util/TestOutputLogger.h"
#include "result/TestResult.h"

#include <exception>
#include <string_view>
#include <tuple>
#include <utility>

namespace tfw::tui {
    class TUI;
}

namespace tfw::test {
    class AbstractTest {
    public:
        virtual ~AbstractTest() = default;

        explicit constexpr AbstractTest(const std::string_view &name, tui::IPostable &postable) : m_postable(postable), m_logger(postable), m_name(
            name) {
        }

        constexpr virtual const result::TestResult &operator()() = 0;

        std::function<const result::TestResult &()> asFunction() {
            return std::bind(&AbstractTest::operator(), this);
        }

        const std::optional<result::TestResult> &result() const noexcept {
            return this->m_result;
        }

        const util::TestOutputLogger &logger() const noexcept {
            return this->m_logger;
        }

        util::TestOutputLogger &logger() noexcept {
            return this->m_logger;
        }

        const util::ChronoWatch &watch() const noexcept {
            return this->m_watch;
        }

        const std::string_view &name() const noexcept {
            return m_name;
        }

        bool isComplete() const noexcept {
            return this->m_watch.stopped() && this->result().has_value();
        }

        bool hasStarted() const noexcept {
            return this->m_watch.started();
        }

        explicit constexpr operator const std::string_view() const noexcept  {
            return m_name;
        }

    protected:
        tui::IPostable &m_postable;

        util::TestOutputLogger m_logger;
        std::optional<result::TestResult> m_result;
        util::ChronoWatch m_watch;
    private:
        const std::string_view m_name;
    };

    template<typename Func, typename... Args>
    requires std::invocable<Func, util::TestOutputLogger &, Args...>
    class Test : public AbstractTest {
    public:
        explicit constexpr Test(const std::string_view &name, tui::IPostable &postable, Func&& f, Args&&... args) : AbstractTest(name, postable), m_test(std::forward<Func>(f)), m_args(std::forward<Args>(args)...) {}

        constexpr const result::TestResult &operator()() override {
            auto args = std::tuple_cat(std::forward_as_tuple(this->m_logger), this->m_args);

            this->m_watch.start();

            try {
                if constexpr (std::same_as<std::invoke_result_t<decltype(this->m_test), util::TestOutputLogger &, Args...>, result::TestResult>) {
                    result::TestResult r = std::apply(this->m_test, args);
                    this->m_watch.stop();
                    this->m_result.emplace(r);
                } else if constexpr (std::same_as<std::invoke_result_t<decltype(this->m_test), util::TestOutputLogger &, Args...>, void>) {
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

    protected:
        Func m_test;
        std::tuple<Args...> m_args;
    };
}

#endif // TFW_TEST_H