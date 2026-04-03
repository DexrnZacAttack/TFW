/** @file ArgumIntegration.cpp
 *
 * @author DexrnZacAttack
 * @date 3/28/26
 * 
 * @device zPc-i2
 */
#include "TestFramework/integration/ArgumIntegration.h"

namespace tfw::integration {
    Argum::Option ArgumIntegration::getUserInterfaceTypeOption(ui::EUserInterface &type) {
        Argum::ChoiceParser choices;
        choices.addChoice("terminal");
        choices.addChoice("headless");

        return Argum::Option("--ui-mode")
            .help("Changes the UI mode used by TFW.")
            .argName(choices.description())
            .occurs(Argum::zeroOrMoreTimes)
            .handler([&type, choices](const std::string_view value) {
                type = static_cast<ui::EUserInterface>(choices.parse(value));
            });
    }
}