//
// Created by DexrnZacAttack on 2/27/26 using zPc-i2.
//
#ifndef TFW_TEST_H
#define TFW_TEST_H
#include "TestFramework/test/exception/TestFailedException.h"
#include "TestFramework/test/logging/ITestLoggerProvider.h"
#include "TestFramework/test/util/ChronoWatch.h"
#include "fmt/args.h"
#include "TestFramework/test/logging/loggers/TuiTestLogger.h"
#include "result/TestResult.h"

#include <exception>
#include <string_view>
#include <tuple>
#include <utility>

namespace tfw::tui {
    class TerminalUserInterface;
}

namespace tfw::test {
    class AbstractTest {
    public:
        virtual ~AbstractTest() = default;

        explicit constexpr AbstractTest(const std::string_view &name, ui::IPostable *postable, logging::ITestLoggerProvider *loggerProvider) : m_postable(postable), m_logger(loggerProvider->makeLogger()), m_name(
            name) {
        }

        constexpr virtual const result::TestResult &operator()() = 0;

        std::function<const result::TestResult &()> asFunction() {
            return std::bind(&AbstractTest::operator(), this);
        }

        const std::optional<result::TestResult> &result() const noexcept {
            return this->m_result;
        }

        const logging::loggers::ITestLogger *logger() const noexcept {
            return this->m_logger;
        }

        logging::loggers::ITestLogger *logger() noexcept {
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
        ui::IPostable *m_postable;

        logging::loggers::ITestLogger *m_logger;
        std::optional<result::TestResult> m_result;
        util::ChronoWatch m_watch;
    private:
        const std::string_view m_name;
    };

    template <typename T>
    concept PostableLoggerProvider = std::is_base_of_v<ui::IPostable, T> && std::is_base_of_v<logging::ITestLoggerProvider, T>;

    template<typename Func, typename... Args>
    requires std::invocable<Func, logging::loggers::ITestLogger &, Args...>
    class Test : public AbstractTest {
    public:
        explicit constexpr Test(const std::string_view &name, ui::IPostable *postable, logging::ITestLoggerProvider *loggerProvider, Func&& f, Args&&... args) : AbstractTest(name, postable, loggerProvider), m_test(std::forward<Func>(f)), m_args(std::forward<Args>(args)...) {}

        template <PostableLoggerProvider Provider>
        explicit constexpr Test(const std::string_view &name, Provider *postableLoggerProvider, Func&& f, Args&&... args) : AbstractTest(name, postableLoggerProvider, postableLoggerProvider), m_test(std::forward<Func>(f)), m_args(std::forward<Args>(args)...) {}

        constexpr const result::TestResult &operator()() override {
            auto args = std::tuple_cat(std::forward_as_tuple(*this->m_logger), this->m_args);

            this->m_watch.start();

#ifndef TFW_NO_SWALLOW_EXCEPTIONS
            try {
#endif
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
#ifndef TFW_NO_SWALLOW_EXCEPTIONS
            } catch (...) {
                this->m_watch.stop();
                this->m_result.emplace(false, std::current_exception());
            }
#endif

            return this->m_result.value();
        }

    protected:
        Func m_test;
        std::tuple<Args...> m_args;
    };
}

#endif // TFW_TEST_H