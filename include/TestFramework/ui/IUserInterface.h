/** @file IUserInterface.h
 *
 * @author DexrnZacAttack
 * @date 3/28/26
 * 
 * @device zPc-i2
 */
#ifndef TFW_IUSERINTERFACE_H
#define TFW_IUSERINTERFACE_H
#include "TestFramework/test/logging/ITestLoggerProvider.h"
#include "TestFramework/ui/IPostable.h"

#include <atomic>
#include <iosfwd>

namespace tfw {
    class TestFramework;
    namespace test {
        class AbstractTest;
    }
}

namespace tfw::ui {
    class IUserInterface : public tfw::ui::IPostable, public test::logging::ITestLoggerProvider {
    public:
        explicit IUserInterface(TestFramework &framework);

        virtual void run() = 0;
    protected:
        //other
        TestFramework &m_framework;
    public:
        //state
        std::atomic<test::AbstractTest *> currentTest;
        const std::stringstream *currentOutput; // this is probably nasty when paired with threading, but it works so idc
    };
}

#endif // TFW_IUSERINTERFACE_H