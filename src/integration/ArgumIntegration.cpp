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
        static Argum::ChoiceParser s_choices;
        s_choices.addChoice("terminal");
        s_choices.addChoice("headless");

        return Argum::Option("--ui-mode")
            .help("Changes the UI mode used by TFW.")
            .argName(s_choices.description())
            .occurs(Argum::zeroOrMoreTimes)
            .handler([&](const std::string_view value) {
                type = static_cast<ui::EUserInterface>(s_choices.parse(value));
            });
    }
}