//
// Created by DexrnZacAttack on 2/27/26 using zPc-i2.
//
#ifndef TFW_TESTRESULT_H
#define TFW_TESTRESULT_H
#include <fstream>
#include <optional>
#include <string>
#include <string_view>
#include <exception>
#include <sstream>

namespace tfw::test::result {
    class TestResult {
    private:
        static std::string getExceptionReason(const std::exception_ptr &ex) {
            if (ex == nullptr)
                return "";

            try {
                std::rethrow_exception(ex);
            } catch (const std::exception &e) {
                return e.what();
            } catch (const std::string &e) {
                return std::string(e);
            } catch (const char *e) {
                return e;
            } catch (...) {
                return "";
            }
        }

        static std::string getExceptionName(const std::exception_ptr &ex) {
            if (ex == nullptr)
                return "";

            try {
                std::rethrow_exception(ex);
            } catch (const std::exception &e) {
                return typeid(e).name();
            } catch (const std::string &e) {
                return "String Exception";
            } catch (const char *e) {
                return "String Exception";
            } catch (...) {
                return "Unknown Exception";
            }
        }

    public:
        constexpr TestResult(const bool pass, const std::string &reason, const std::exception_ptr &exception) : passed(pass), reason("An exception of type '" + TestResult::getExceptionName(exception) + "' was thrown"), exception(exception) {};
        constexpr TestResult(const bool pass, const std::exception_ptr &exception) : passed(pass), reason("An exception of type '" + TestResult::getExceptionName(exception) + "' was thrown"), exception(exception) {};
        constexpr TestResult(const bool pass, const std::string &reason) : passed(pass), reason(reason) {};
        explicit constexpr TestResult(const bool pass) : passed(pass) {};

        std::string getExceptionReason() const {
            return TestResult::getExceptionReason(this->exception);
        }

        std::string getExceptionName() const {
            return TestResult::getExceptionName(this->exception);
        }

        std::string status() const {
            if (passed) {
                return "Passed";
            }

            if (exception != nullptr) {
                return "Failed - Exception thrown";
            }

            return "Failed";
        }

        std::string toString() const {
            std::stringstream ss;
            std::string r = this->reason;

            ss << "Status: " << this->status();

            if (exception) {
                ss << std::endl << "Type: " << this->getExceptionName();
                r = this->getExceptionReason();
            }

            if (!r.empty()) {
                ss << std::endl << "Reason: " << r;
            }

            return ss.str();
        }

        const bool passed;
        const std::string reason;
        const std::exception_ptr exception;
    };
}

#endif // TFW_TESTRESULT_H