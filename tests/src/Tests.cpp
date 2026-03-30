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
    
    tfw->addTest("Implicit Succeed", [](tfw::test::logging::loggers::ITestLogger &output) {
        //will succeed because it didn't throw
    });

    tfw->addTest("Explicit Succeed", [](tfw::test::logging::loggers::ITestLogger &output) {
        return tfw::test::result::TestResult(true, "Yay, it worked!");
    });

    tfw->addTest("Fail", [](tfw::test::logging::loggers::ITestLogger &output) {
        return tfw::test::result::TestResult(false);
    });

    tfw->addTest("Throw", [](tfw::test::logging::loggers::ITestLogger &output) {
        throw tfw::test::exception::TestFailedException("womp womp");
    });

    constexpr int COMPARE_VALUE = 0;
    tfw->addComparableTest("Pass comparison", COMPARE_VALUE, [](tfw::test::logging::loggers::ITestLogger &output) {
        return 0;
    });

    tfw->addComparableTest("Fail comparison", COMPARE_VALUE, [](tfw::test::logging::loggers::ITestLogger &output) {
        return 1;
    });

    tfw->addTest("Pass args (0)", acceptParameters, 0);
    tfw->addTest("Pass args (1)", acceptParameters, 1);

    tfw->addTest("Print", [](tfw::test::logging::loggers::ITestLogger &output) {
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

    tfw->addTest("Wait 5 seconds", [](tfw::test::logging::loggers::ITestLogger &output) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    });
    

    tfw->run();

    return 0;
}