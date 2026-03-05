//
// Created by DexrnZacAttack on 3/4/26 using zPc-i2.
//
#ifndef TFW_COLORUTIL_H
#define TFW_COLORUTIL_H
#include "ftxui/screen/color.hpp"

namespace tfw::tui::util {
    class ColorUtil {
    public:
        static ftxui::Color lighten(const ftxui::Color c, const float amount = 0.5) {
            return c.Interpolate(amount, c, ftxui::Color::White);
        }

        static ftxui::Color darken(const ftxui::Color c, const float amount = 0.5) {
            return c.Interpolate(amount, c, ftxui::Color::Black);
        }
    };
}

#endif // TFW_COLORUTIL_H