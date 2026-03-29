/** @file HeadlessUserInterface.h
 *
 * @author DexrnZacAttack
 * @date 3/28/26
 * 
 * @device zPc-i2
 */
#ifndef LODESTONE_HEADLESSUSERINTERFACE_H
#define LODESTONE_HEADLESSUSERINTERFACE_H
#include "TestFramework/ui/IUserInterface.h"

namespace tfw::ui::headless {
    class HeadlessUserInterface : public IUserInterface {
    public:
        explicit HeadlessUserInterface(TestFramework &framework)
            : IUserInterface(framework) {
        }

        void postEvent(const Event &ev) override;
        void run() override;

        test::logging::loggers::ITestLogger *makeLogger() override;
    private:
        std::atomic<int> m_testsRan = 0; // if this is not volatile, compilers will optimize out the while loop found in run();
        size_t m_loggerPos = 0;
    };
}

#endif // LODESTONE_HEADLESSUSERINTERFACE_H