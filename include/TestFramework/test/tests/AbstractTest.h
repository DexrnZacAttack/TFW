/** @file AbstractTest.h
 *
 * @author DexrnZacAttack
 * @date 3/30/26
 * 
 * @device zPc-i2
 */
#ifndef TFW_ABSTRACTTEST_H
#define TFW_ABSTRACTTEST_H
#include "TestFramework/test/logging/ITestLoggerProvider.h"
#include "TestFramework/test/logging/loggers/ITestLogger.h"
#include "TestFramework/test/result/TestResult.h"
#include "TestFramework/test/util/ChronoWatch.h"

#include <functional>

namespace tfw::test::tests {
    class AbstractTest {
    public:
        virtual ~AbstractTest() = default;

        explicit constexpr AbstractTest(const std::string_view &name, ui::IPostable *postable, logging::ITestLoggerProvider *loggerProvider, const bool shouldThrow) : m_postable(postable), m_logger(loggerProvider->makeLogger()), m_shouldThrow(shouldThrow), m_name(name) {
        }

        constexpr virtual const result::TestResult &operator()() = 0;

        bool isComplete() const noexcept;

        bool hasStarted() const noexcept;

        std::function<const result::TestResult &()> asFunction();

        const std::optional<result::TestResult> &result() const noexcept;

        const logging::loggers::ITestLogger *logger() const noexcept;

        logging::loggers::ITestLogger *logger() noexcept;

        const util::ChronoWatch &watch() const noexcept;

        constexpr bool shouldThrow() const noexcept {
            return this->m_shouldThrow;
        }

        constexpr const std::string_view &name() const noexcept {
            return m_name;
        }

        explicit constexpr operator const std::string_view() const noexcept  {
            return this->name();
        }

    protected:
        ui::IPostable *m_postable;

        logging::loggers::ITestLogger *m_logger;
        std::optional<result::TestResult> m_result;
        util::ChronoWatch m_watch;

        bool m_shouldThrow = false;
    private:
        const std::string_view m_name;
    };
} // namespace tfw::test::tests

#endif // TFW_ABSTRACTTEST_H