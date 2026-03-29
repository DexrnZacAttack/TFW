//
// Created by DexrnZacAttack on 2/27/26 using zPc-i2.
//
#include "TestFramework/test/util/ChronoWatch.h"

#include <cstring>
#include <functional>

namespace tfw::test::util {
    void ChronoWatch::start() {
        this->m_end = std::nullopt;

        this->m_start = std::chrono::steady_clock::now();
    }

    void ChronoWatch::stop() {
        this->m_end = std::chrono::steady_clock::now();
    }

    bool ChronoWatch::stopped() const {
        return this->m_end.has_value();
    }

    bool ChronoWatch::started() const {
        return this->m_start.has_value();
    }

    bool ChronoWatch::running() const noexcept {
        return this->started() && !this->stopped();
    }

    std::chrono::duration<double> ChronoWatch::difference() const {
        if (!this->started())
            return std::chrono::milliseconds(0);

        if (!this->stopped())
            return std::chrono::steady_clock::now() - this->m_start.value();

        return this->m_end.value() - this->m_start.value();
    }

    std::string ChronoWatch::duration() const {
        if (!this->stopped()) {
            return "Not finished";
        }

        // hunk of junk
        static std::array<std::pair<std::pair<const char *, const char *>, std::function<double(std::chrono::nanoseconds &)>>, 4> s_converters = {{
        { {"h", ":"}, [](std::chrono::nanoseconds &ns) { return dec<std::chrono::hours>(ns).count();       } },
        { {"m", ":"}, [](std::chrono::nanoseconds &ns) { return dec<std::chrono::minutes>(ns).count();     } },
        { {"s", "."}, [](std::chrono::nanoseconds &ns) { return dec<std::chrono::seconds>(ns).count();     } },
        { {"ns", ""}, [](std::chrono::nanoseconds &ns) { return dec<std::chrono::nanoseconds>(ns).count(); } },
        }};

        std::chrono::nanoseconds nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(this->difference());
        bool printed = false;

        std::stringstream ss;
        for (auto [t, conv] : s_converters) { // this is slop, I had to add comments cuz I was getting lost
            // if our conv result is zero and we haven't printed yet, we want to not print out this time unit
            if (const long long c = conv(nanos); c != 0 || printed) {  //if we have printed, we can print all time units below.
                if (!std::strcmp(t.second, "")) { //are we at last unit?
                    if (!printed) { // we are, but have we printed yet?
                        ss << c << t.first; // we haven't, so we'll shove the unit type at the end
                    } else {
                        ss << std::setw(9) << std::setfill('0') << c << "s"; // we have, so our logical last unit is seconds
                    }
                } else { // we arent
                    printed = true; // now we've printed
                    ss << c << t.second; //add count and seconds
                }
            }
        }

        return ss.str();
    }
}