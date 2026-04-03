//
// Created by DexrnZacAttack on 3/4/26 using zPc-i2.
//
#ifndef TFW_TESTBUTTON_H
#define TFW_TESTBUTTON_H
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/component_base.hpp"

namespace tfw::test::tests {
    class AbstractTest;
}

namespace tfw::ui::tui::components {
    class TestButton : public ftxui::ComponentBase {
    public:
        struct Options {
            struct HoverableAnimatedColorOption : ftxui::AnimatedColorOption {
                ftxui::Color hover;
            };

            struct HoverableAnimatedColorOptions {
                HoverableAnimatedColorOption foreground;
                HoverableAnimatedColorOption background;
            };

            tfw::test::tests::AbstractTest *test;

            std::function<void(TestButton &button)> invokeEvent = [](TestButton &button) {};

            std::function<ftxui::Element(const ftxui::EntryState&)> transform;
        };

        explicit TestButton(const Options &options);
        TestButton(tfw::test::tests::AbstractTest *test, const std::function<void(TestButton &button)> &invokeEvent, const Options &options);
        TestButton(tfw::test::tests::AbstractTest *test, const std::function<void(TestButton &button)> &invokeEvent);

        static std::shared_ptr<TestButton> create(tfw::test::tests::AbstractTest *test, std::function<void(TestButton &button)> invokeEvent, Options options);
        static std::shared_ptr<TestButton> create(tfw::test::tests::AbstractTest *test, std::function<void(TestButton &button)> invokeEvent);

        ftxui::Element OnRender() override;
        bool OnEvent(ftxui::Event) override;

        Options &options();

        bool Focusable() const override;
        bool active() const;

        static ftxui::Color getColor(const tfw::test::tests::AbstractTest *test);
        static Options::HoverableAnimatedColorOptions getColorOptions(const tfw::test::tests::AbstractTest *test);
    private:
        bool m_hovered;
        ftxui::Box m_bb;

        Options m_options;
    };
}

#endif // TFW_TESTBUTTON_H