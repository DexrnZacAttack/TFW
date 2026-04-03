/** @file AbstractTest.cpp
 *
 * @author DexrnZacAttack
 * @date 3/30/26
 * 
 * @device zPc-i2
 */
#include "TestFramework/test/tests/AbstractTest.h"

namespace tfw::test::tests {
    std::function<const result::TestResult &()> AbstractTest::asFunction() {
        return std::bind(&AbstractTest::operator(), this);
    }

    const std::optional<result::TestResult> &AbstractTest::result() const noexcept {
        return this->m_result;
    }
    const logging::loggers::ITestLogger *AbstractTest::logger() const noexcept {
        return this->m_logger;
    }

    logging::loggers::ITestLogger *AbstractTest::logger() noexcept {
        return this->m_logger;
    }

    const util::ChronoWatch &AbstractTest::watch() const noexcept {
        return this->m_watch;
    }
    bool AbstractTest::isComplete() const noexcept {
        return this->m_watch.stopped() && this->result().has_value();
    }

    bool AbstractTest::hasStarted() const noexcept {
        return this->m_watch.started();
    }
} // namespace tfw::test::tests