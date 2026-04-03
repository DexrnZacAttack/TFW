//
// Created by DexrnZacAttack on 2/27/26 using zPc-i2.
//
#include "TestFramework/TestFramework.h"
#include "TestFramework/test/exception/TestFailedException.h"
#include <ranges>

tfw::test::result::TestResult acceptParameters(tfw::test::logging::loggers::ITestLogger &output, const int i) {
    return { true, "I received " + std::to_string(i) };
}

int main(const int argc, char *argv[]) {
    tfw::TestFramework *tfw = tfw::TestFramework::initFromArgs(argc, argv);
    tfw::test::TestManager &mgr = tfw->testManager();
    
    mgr.addTest("Implicit Succeed", [](tfw::test::logging::loggers::ITestLogger &output) {
        //will succeed because it didn't throw
    });

    mgr.addTest("Explicit Succeed", [](tfw::test::logging::loggers::ITestLogger &output) {
        return tfw::test::result::TestResult(true, "Yay, it worked!");
    });

    mgr.addTest("Fail", [](tfw::test::logging::loggers::ITestLogger &output) {
        return tfw::test::result::TestResult(false);
    });

    mgr.addTest("Throw", [](tfw::test::logging::loggers::ITestLogger &output) {
        throw tfw::test::exception::TestFailedException("womp womp");
    });

    constexpr int COMPARE_VALUE = 0;
    mgr.addComparableTest("Pass comparison", COMPARE_VALUE, [](tfw::test::logging::loggers::ITestLogger &output) {
        return 0;
    });

    mgr.addComparableTest("Fail comparison", COMPARE_VALUE, [](tfw::test::logging::loggers::ITestLogger &output) {
        return 1;
    });

    mgr.addTest("Pass args (0)", acceptParameters, 0);
    mgr.addTest("Pass args (1)", acceptParameters, 1);

    mgr.addTest("Print", [](tfw::test::logging::loggers::ITestLogger &output) {
        constexpr std::array<const char *, 25> LINES = {
            {
                "I", "just", "need", "to", "see", "if", "this", "lets", "you", "scroll", "properly", "in", "the", "logging", "window.",
                " ",
                "The", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog."
            }};

        for (auto line : LINES) {
            output << line << "\n";
        }
    });

    mgr.addTest("Wait 5 seconds", [](tfw::test::logging::loggers::ITestLogger &output) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    });

    mgr.addThrowingTest("Always throw and stop", [](tfw::test::logging::loggers::ITestLogger &output) {
        throw std::runtime_error("it threw");
    });

    tfw->run();

    return 0;
}