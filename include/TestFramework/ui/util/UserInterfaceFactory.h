/** @file UserInterfaceFactory.h
 *
 * @author DexrnZacAttack
 * @date 3/28/26
 * 
 * @device zPc-i2
 */
#ifndef LODESTONE_USERINTERFACEFACTORY_H
#define LODESTONE_USERINTERFACEFACTORY_H
#include "TestFramework/ui/EUserInterface.h"
#include "TestFramework/ui/IUserInterface.h"

#include <memory>

namespace tfw {
    class TestFramework;
}

namespace tfw::ui::util {
    class UserInterfaceFactory {
    private:
        UserInterfaceFactory() = delete;
    public:
        static std::unique_ptr<IUserInterface> get(TestFramework &tfw, EUserInterface type);
    };
}

#endif // LODESTONE_USERINTERFACEFACTORY_H