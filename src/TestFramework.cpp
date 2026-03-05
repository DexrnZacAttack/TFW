//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#include "TestFramework/TestFramework.h"

#include "fmt/base.h"
#include "TestFramework/test/Test.h"
#include "TestFramework/tui/Tui.h"

#include <algorithm>
#include <future>
#include <iostream>
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"

#include <random>
#include <ranges>

namespace tfw {
    TestFramework::TestFramework() : m_tui(*this) {
    }

    TestFramework *TestFramework::getInstance() {
        static TestFramework s_framework;
        return &s_framework;
    }

    void TestFramework::run() {
        //task runner
        this->m_taskRunnerThread = std::jthread([this](std::stop_token stop) {
            // for each task, run it
            std::ranges::for_each(this->m_tests | std::ranges::views::enumerate, [this, stop](const std::pair<int, const std::unique_ptr<tfw::test::AbstractTest>&> &t) {
                if (stop.stop_requested())
                    return;

              const std::unique_ptr<tfw::test::AbstractTest> &test = t.second;
              int idx = t.first;

              //setup logger
                //no longer needed
              // test::util::TestOutputLogger logger(this->m_tui);

              //create new shit for this test
              this->m_runningTest = test.get();

              this->m_tui.currentTest = test.get();
              this->m_tui.currentOutput = &test.get()->logger().stream();
              this->m_tui.postEvent(ftxui::Event::Custom); //tell gui to rerender

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

              this->m_tui.postEvent(ftxui::Event::Custom); //test finished, tell gui to rerender
          });
        });

        //start gui loop
        this->m_tui.runRender();

        //if we left the render loop, we have quit the program.
        this->exit();
    }

    void TestFramework::exit() {
        this->m_taskRunnerThread.request_stop();
        if (this->m_forceExit) { //user has requested forceful exit
            //this is a bad idea, but it's a user invoked action
            // this mainly breaks continuing in the user's program
            // I wish I could just cancel the future
            std::_Exit(0);
        }

        if (const test::AbstractTest *t = this->m_runningTest.load(); t != nullptr && !t->isComplete())
            fmt::println("Waiting for test '{}' to finish.", this->m_runningTest.load()->name());

        this->m_taskRunnerThread.join(); //wait for the thread
    }
} // tfw