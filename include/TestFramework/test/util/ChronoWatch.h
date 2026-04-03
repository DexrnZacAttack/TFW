//
// Created by DexrnZacAttack on 2/27/26 using zPc-i2.
//
#ifndef TFW_CHRONOWATCH_H
#define TFW_CHRONOWATCH_H
#include <chrono>

namespace tfw::test::util {
        class ChronoWatch {
        public:
            void start();
            void stop();

            bool stopped() const;
            bool started() const;
            bool running() const noexcept;

            std::chrono::duration<double> difference() const;
            std::string duration() const;
        private:
            template<typename T>
            static T dec(std::chrono::nanoseconds &ns) {
                const T h = std::chrono::duration_cast<T>(ns);
                ns -= h;

                return h;
            }

            std::optional<std::chrono::time_point<std::chrono::steady_clock>> m_start;
            std::optional<std::chrono::time_point<std::chrono::steady_clock>> m_end;
        };
} // tfw

#endif // TFW_CHRONOWATCH_H