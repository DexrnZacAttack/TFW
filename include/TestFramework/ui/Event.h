/** @file Event.h
 *
 * @author DexrnZacAttack
 * @date 3/28/26
 * 
 * @device zPc-i2
 */
#ifndef TFW_EVENT_H
#define TFW_EVENT_H

namespace tfw::ui {
    enum class Event {
        // UI
        RERENDER, /**< We need the UI to rerender for whatever reason */
        // TUI
        SWITCH_OBSERVED_TEST,
        // Test
        TEST_LOG_OUTPUT, /**< Characters were sent into the test logger */
        TEST_STARTING, /**< A new Test is starting */
        TEST_FINISHED /**< Current Test is ending */
    };
}

#endif // TFW_EVENT_H