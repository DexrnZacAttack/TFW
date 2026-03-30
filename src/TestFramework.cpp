//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#include "TestFramework/TestFramework.h"

#include "TestFramework/integration/ArgumIntegration.h"

#include <argum/argum.h>
#include "fmt/base.h"
#include "TestFramework/test/Test.h"
#include "TestFramework/ui/EUserInterface.h"
#include "TestFramework/ui/util/UserInterfaceFactory.h"

#include <algorithm>
#include <future>
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"

#include <random>
#include <ranges>

namespace tfw {
    TestFramework *TestFramework::s_instance = nullptr;

    TestFramework::TestFramework(const ui::EUserInterface ui) : m_ui(ui::util::UserInterfaceFactory::get(*this, ui)) {
    }

    TestFramework *TestFramework::initFromArgs(const int argc, char *argv[]) {
        ui::EUserInterface type = ui::EUserInterface::TERMINAL;

        Argum::Parser parser;
        parser.add(
            Argum::Option("--help", "-h").
            help("show this help message and exit").
            handler([&]() {
            std::cout << parser.formatHelp("TFW");
        }));

        parser.add(
            integration::ArgumIntegration::getUserInterfaceTypeOption(type)
        );

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


    void TestFramework::run() {
        //task runner
        this->m_taskRunnerThread = std::jthread([this](const std::stop_token &stop) {
            // for each task, run it
            std::ranges::for_each(this->m_tests | std::ranges::views::enumerate,
                                  [this, &stop](const std::pair<int, const std::unique_ptr<tfw::test::AbstractTest> &> &t) {
                                      if (stop.stop_requested())
                                          return;

                                      const std::unique_ptr<tfw::test::AbstractTest> &test = t.second;
                                      int idx = t.first;

                                      //create new shit for this test
                                      this->m_runningTest = test.get();

                                      this->m_ui->currentTest = test.get();
                                      this->m_ui->postEvent(ui::Event::TEST_STARTING); //tell gui to rerender

                                      //start
                                      this->m_currentFuture =
                                          std::async(
                                              std::launch::async,
                                              test->asFunction());
                                      while (this->m_currentFuture.wait_for(
                                                 std::chrono::milliseconds(0))
                                             != std::future_status::ready) {
                                      }

                                      //we have our results, let's save them
                                      m_results.emplace(idx,
                                                        std::make_pair(test.get(), std::move(this->m_currentFuture.get()))
                                          );

                                      this->m_ui->postEvent(ui::Event::TEST_FINISHED); //test finished, tell gui to rerender
                                  });
        });

        //start gui loop
        this->m_ui->run();

        //if we left the render loop, we have quit the program.
        this->exit();
    }

    void TestFramework::exit() {
        this->m_taskRunnerThread.request_stop();

        if (const test::AbstractTest *t = this->m_runningTest.load(); t != nullptr && !t->isComplete())
            fmt::println("Waiting for test '{}' to finish.", this->m_runningTest.load()->name());

        this->m_taskRunnerThread.join(); //wait for the thread
    }
} // tfw