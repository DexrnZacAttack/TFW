//
// Created by DexrnZacAttack on 3/3/26 using zPc-i2.
//
#ifndef TFW_TESTFRAMEWORK_H
#define TFW_TESTFRAMEWORK_H
#include "TestFramework/test/TestManager.h"
#include "TestFramework/ui/EUserInterface.h"
#include "TestFramework/ui/tui/TerminalUserInterface.h"

#include <future>
#include <memory>

namespace tfw {
    class TestFramework {
    private:
        explicit TestFramework(ui::EUserInterface ui);
    public:
        /** Parses args from the main function and initializes TestFramework
         *
         * ### Args:
         * - @code--ui-mode@endcode: Selects which user interface to use
         *   - terminal
         *   - headless
         */
        static TestFramework *initFromArgs(int argc, char *argv[]);
        /** Initializes TestFramework with the given UI type */
        static TestFramework *init(ui::EUserInterface ui);

        /** Returns the global TestFramework instance
         *
         * @throws std::invalid_argument if TestFramework::init/initFromArgs has not been called yet.
         */
        static TestFramework *getInstance();

        test::TestManager &testManager();

        void run();

    private:
        void exit();

    private:
        static TestFramework *s_instance;
        std::unique_ptr<tfw::ui::IUserInterface> m_ui;

        /// Thread that runs the tasks
        std::jthread m_taskRunnerThread;

        test::TestManager m_testManager;
    };
} // tfw

#endif // TFW_TESTFRAMEWORK_H