//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#ifndef TFW_TESTFAILEDEXCEPTION_H
#define TFW_TESTFAILEDEXCEPTION_H
#include <stdexcept>

namespace tfw::test::exception {
    class TestFailedException : public std::runtime_error {
    public:
        explicit TestFailedException(const std::string &__arg)
            : runtime_error(__arg) {
        }

        explicit TestFailedException(const char *string)
            : runtime_error(string) {
        }

        explicit TestFailedException(runtime_error &&runtimeError)
            : runtime_error(runtimeError) {
        }

        explicit TestFailedException(const runtime_error &runtimeError)
            : runtime_error(runtimeError) {
        }
    };
}

#endif // TFW_TESTFAILEDEXCEPTION_H