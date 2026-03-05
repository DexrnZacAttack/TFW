//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#ifndef TFW_IPOSTABLE_H
#define TFW_IPOSTABLE_H

#include "ftxui/component/event.hpp"

namespace tfw::tui {
    class IPostable {
    public:
        virtual ~IPostable() = default;

        virtual void postEvent(const ftxui::Event &ev) = 0;
    };
}

#endif // TFW_IPOSTABLE_H