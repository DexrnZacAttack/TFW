//
// Created by DexrnZacAttack on 2/27/26 using zPc-i2.
//
#include "TestFramework/TestFramework.h"
#include "TestFramework/test/exception/TestFailedException.h"
#include <ranges>

int main() {
    tfw::TestFramework::getInstance()->addTest("Implicit Succeed", [](tfw::test::util::TestOutputLogger &output) {
        //will succeed because it didn't throw
    });

    tfw::TestFramework::getInstance()->addTest("Explicit Succeed", [](tfw::test::util::TestOutputLogger &output) {
        return tfw::test::result::TestResult(true, "Yay, it worked!");
    });

    tfw::TestFramework::getInstance()->addTest("Fail", [](tfw::test::util::TestOutputLogger &output) {
        return tfw::test::result::TestResult(false);
    });

    tfw::TestFramework::getInstance()->addTest("Throw", [](tfw::test::util::TestOutputLogger &output) {
        throw tfw::test::exception::TestFailedException("womp womp");
    });

    constexpr int COMPARE_VALUE = 0;
    tfw::TestFramework::getInstance()->addComparableTest("Pass comparison", COMPARE_VALUE, [](tfw::test::util::TestOutputLogger &output) {
        return 0;
    });

    tfw::TestFramework::getInstance()->addComparableTest("Fail comparison", COMPARE_VALUE, [](tfw::test::util::TestOutputLogger &output) {
        return 1;
    });

    tfw::TestFramework::getInstance()->addTest("Print", [](tfw::test::util::TestOutputLogger &output) {
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

    tfw::TestFramework::getInstance()->run();

    return 0;
}