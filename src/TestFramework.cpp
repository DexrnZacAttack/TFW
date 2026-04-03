//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#include "TestFramework/TestFramework.h"

#include "TestFramework/integration/ArgumIntegration.h"

#include "TestFramework/test/tests/Test.h"
#include "TestFramework/ui/EUserInterface.h"
#include "TestFramework/ui/util/UserInterfaceFactory.h"
#include "fmt/base.h"
#include <argum/argum.h>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <algorithm>
#include <future>

#include <random>
#include <ranges>

namespace tfw {
    TestFramework *TestFramework::s_instance = nullptr;

    TestFramework::TestFramework(const ui::EUserInterface ui) : m_ui(ui::util::UserInterfaceFactory::get(*this, ui)), m_testManager(m_ui.get()) {}

    TestFramework *TestFramework::initFromArgs(const int argc, char *argv[]) {
        ui::EUserInterface type = ui::EUserInterface::TERMINAL;

        Argum::Parser parser;
        parser.add(integration::ArgumIntegration::getUserInterfaceTypeOption(type));

        parser.add(Argum::Option("--help", "-h").help("show this help message and exit").handler([&]() {
            const Argum::Colorizer c = Argum::colorizerForFile(Argum::environmentColorStatus(), stdout);
            std::cout << parser.formatHelp(argc ? argv[0] : "TFW", Argum::terminalWidth(stdout), c);
            std::exit(EXIT_SUCCESS);
        }));

        parser.parse(argc, argv);

        s_instance = new TestFramework(type);

        return getInstance();
    }

    TestFramework *TestFramework::init(const ui::EUserInterface ui) {
        s_instance = new TestFramework(ui);
        return getInstance();
    }

    TestFramework *TestFramework::getInstance() {
        if (s_instance == nullptr)
            throw std::invalid_argument("TestFramework has not been initialized yet.");

        return s_instance;
    }

    test::TestManager &TestFramework::testManager() { return this->m_testManager; }

    void TestFramework::run() {
        // task runner
        this->m_taskRunnerThread = std::jthread([this](const std::stop_token &stop) {
            // for each task, run it
            std::ranges::for_each(this->m_testManager.tests(), [this, &stop](const std::unique_ptr<tfw::test::tests::AbstractTest> &test) {
                if (stop.stop_requested())
                    return;

                // stage test
                test::TestManager::StagedTestRunner runner = this->m_testManager.stage(test.get());

                this->m_ui->currentTest = test.get();
                this->m_ui->postEvent(ui::Event::TEST_STARTING); // tell gui to rerender

                // start
                runner
                    .then([this](tfw::test::tests::AbstractTest *, test::result::TestResult) {
                        this->m_ui->postEvent(ui::Event::TEST_FINISHED); // test finished, tell gui to rerender
                    })
                    .run();
            });
        });

        // start gui loop
        this->m_ui->run();

        // if we left the render loop, we have quit the program.
        this->exit();
    }

    void TestFramework::exit() {
        this->m_taskRunnerThread.request_stop();

        if (const tfw::test::tests::AbstractTest *t = this->m_testManager.currentTest().load(); t != nullptr && !t->isComplete())
            fmt::println("Waiting for test '{}' to finish.", t->name());

        this->m_taskRunnerThread.join(); // wait for the thread
    }
} // namespace tfw