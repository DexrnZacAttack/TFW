/** @file IUserInterface.cpp
 *
 * @author DexrnZacAttack
 * @date 3/28/26
 * 
 * @device zPc-i2
 */
#include "TestFramework/ui/IUserInterface.h"

namespace tfw::ui {
    IUserInterface::IUserInterface(TestFramework &framework) : m_framework(framework), currentTest(nullptr) {

    }
}