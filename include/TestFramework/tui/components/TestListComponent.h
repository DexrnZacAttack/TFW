//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#ifndef TFW_TESTLISTCOMPONENT_H
#define TFW_TESTLISTCOMPONENT_H
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component.hpp"
#include "TestFramework/tui/components/TestButton.h"
#include "ftxui/dom/elements.hpp"

namespace tfw {
    namespace test {
        class AbstractTest;
    }

    class TestFramework;
}

namespace tfw {
    namespace tui {
        class TUI;
    }
}

namespace tfw::tui::components {
    class TestListComponent : public ftxui::ComponentBase {
    public:
        TestListComponent(TUI &tui, TestFramework &framework);

        ftxui::Element OnRender() override;
    private:
        ftxui::Component transformTest(std::tuple<long, const std::unique_ptr<test::AbstractTest> &> p) const;
    private:
        int m_focused = 0;
        ftxui::Component m_list;
        ftxui::Components m_buttons;


        TUI &m_tui;
        TestFramework &m_framework;
    };
}

#endif // TFW_TESTLISTCOMPONENT_H