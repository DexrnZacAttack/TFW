//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#ifndef TFW_TESTSTATUSCOMPONENT_H
#define TFW_TESTSTATUSCOMPONENT_H

#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"

namespace tfw {
    namespace tui {
        class TUI;
    }

    class TestFramework;
}

namespace tfw::tui::components {
    class TestStatusComponent : public ftxui::ComponentBase {
    public:
        TestStatusComponent(TUI &tui, TestFramework &framework);

        ftxui::Element OnRender() override;

    private:
        TUI &m_tui;
        TestFramework &m_framework;
    };
}

#endif // TFW_TESTSTATUSCOMPONENT_H