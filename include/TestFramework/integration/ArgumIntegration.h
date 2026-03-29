/** @file ArgumIntegration.h
 *
 * @author DexrnZacAttack
 * @date 3/28/26
 * 
 * @device zPc-i2
 */
#ifndef LODESTONE_ARGUMINTEGRATION_H
#define LODESTONE_ARGUMINTEGRATION_H
#include "TestFramework/ui/EUserInterface.h"

#include <argum/argum.h>

namespace tfw::integration {
    class ArgumIntegration {
    private:
        ArgumIntegration() = delete;
    public:
        static Argum::Option getUserInterfaceTypeOption(ui::EUserInterface &type);
    };
}

#endif // LODESTONE_ARGUMINTEGRATION_H