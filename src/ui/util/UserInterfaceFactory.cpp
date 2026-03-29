/** @file UserInterfaceFactory.cpp
 *
 * @author DexrnZacAttack
 * @date 3/28/26
 * 
 * @device zPc-i2
 */
#include "../../../include/TestFramework/ui/util/UserInterfaceFactory.h"

#include "TestFramework/ui/headless/HeadlessUserInterface.h"
#include "TestFramework/ui/tui/TerminalUserInterface.h"

namespace tfw::ui::util {
    std::unique_ptr<IUserInterface> UserInterfaceFactory::get(TestFramework &tfw, const EUserInterface type) {
        switch (type) {
            case EUserInterface::TERMINAL: return std::make_unique<tui::TerminalUserInterface>(tfw);
            case EUserInterface::HEADLESS: return std::make_unique<headless::HeadlessUserInterface>(tfw);
            default: throw std::invalid_argument("Invalid EUserInterface");
        }
    }
}