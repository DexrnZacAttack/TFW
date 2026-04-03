//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#ifndef TFW_TESTLISTCOMPONENT_H
#define TFW_TESTLISTCOMPONENT_H
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component.hpp"
#include "TestFramework/ui/tui/components/TestButton.h"
#include "ftxui/dom/elements.hpp"

namespace tfw {
    namespace test {
        class AbstractTest;
    }

    class TestFramework;
}

namespace tfw {
    namespace ui::tui {
        class TerminalUserInterface;
    }
}

namespace tfw::ui::tui::components {
    class TestListComponent : public ftxui::ComponentBase {
    public:
        TestListComponent(TerminalUserInterface &tui, TestFramework &framework);

        ftxui::Element OnRender() override;
    private:
        ftxui::Component transformTest(long idx, tfw::test::tests::AbstractTest *test) const;
    private:
        int m_focused = 0;
        ftxui::Component m_list;
        ftxui::Components m_buttons;


        TerminalUserInterface &m_tui;
        TestFramework &m_framework;
    };
}

#endif // TFW_TESTLISTCOMPONENT_H