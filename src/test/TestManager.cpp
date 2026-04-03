/** @file TestManager.cpp
 *
 * @author DexrnZacAttack
 * @date 3/30/26
 * 
 * @device zPc-i2
 */
#include "TestFramework/test/TestManager.h"

#include "TestFramework/TestFramework.h"

#include <algorithm>

namespace tfw::test {
    TestManager::StagedTestRunner &TestManager::StagedTestRunner::then(const std::function<void(tests::AbstractTest *, result::TestResult)> &onFinish) {
        this->m_onFinish.push_back(onFinish);

        return *this;
    }

    void TestManager::StagedTestRunner::run() {
        result::TestResult tr = this->m_test->asFunction()();
        std::ranges::for_each(this->m_onFinish, [&tr, this](auto &predicate) {
           predicate(this->m_test, tr);
        });
    }

    TestManager::TestManager(ui::IUserInterface *ui) :m_ui(ui) {}

    TestManager::StagedTestRunner TestManager::stage(tests::AbstractTest *test) {
        this->m_runningTest = test;

        return StagedTestRunner(this->m_runningTest.load())
        .then([this](tests::AbstractTest *t, result::TestResult result) {
            this->m_results.emplace(t, result);
        });;
    }
} // namespace tfw::test