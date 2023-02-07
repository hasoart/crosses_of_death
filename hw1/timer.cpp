#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>

using namespace std::chrono;

class Timer {
private:
    bool is_running = false;
    steady_clock::time_point start;
    steady_clock::time_point last_pause;
public:
    Timer() {  }
    
    void restart() {
        is_running = true;
        last_pause = start = steady_clock::now();
    }

    void pause() {
        is_running = false;
        last_pause = steady_clock::now();
    }

    void resume() {
        is_running = true;
        start = steady_clock::now() - (last_pause - start);
    }

    void toggle() {
        if (is_running)
            pause();
        else
            resume();
    }
    
    template <typename unit_t>
    uint64_t get_time() {
        if (is_running)
            return duration_cast<unit_t>(steady_clock::now() - start).count();
        else
            return duration_cast<unit_t>(last_pause - start).count();
    }

    bool get_state() { return is_running; }
};

int main() {
    Timer t;
    auto count = 0.;
    t.restart();
    std::this_thread::sleep_for(milliseconds(100));
    std::cout << t.get_time<milliseconds>() << std::endl;
    t.toggle();
    std::this_thread::sleep_for(milliseconds(200));
    std::cout << t.get_time<milliseconds>() << std::endl;
    t.toggle();
    std::this_thread::sleep_for(milliseconds(250));
    std::cout << t.get_time<milliseconds>() << std::endl;
    t.restart();
    std::this_thread::sleep_for(milliseconds(100));
    std::cout << t.get_time<milliseconds>() << std::endl;
    t.toggle();
    std::this_thread::sleep_for(milliseconds(200));
    std::cout << t.get_time<milliseconds>() << std::endl;
    t.toggle();
    std::this_thread::sleep_for(milliseconds(250));
    std::cout << t.get_time<milliseconds>() << std::endl;
    return 0;
}
