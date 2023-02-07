#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>

using namespace std::chrono;

template <typename unit_t>
class Timer {
private:
    bool is_running = false;
    uint64_t time = 0;
    steady_clock::time_point start;
public:
    Timer() {  }
    
    void restart() {
        is_running = true;
        start = steady_clock::now();
        time = 0;
    }

    void pause() {
        is_running = false;
        time += duration_cast<unit_t>(steady_clock::now() - start).count();
    }

    void resume() {
        is_running = true;
        start = steady_clock::now();
    }

    void toggle() {
        if (is_running)
            pause();
        else
            resume();
    }
    
    uint64_t get_time() {
        if (is_running)
            return time + duration_cast<unit_t>(steady_clock::now() - start).count();
        else
            return time;
    }

    bool get_state() { return is_running; }
};

int main() {
    Timer<milliseconds> t;
    auto count = 0.;
    t.restart();
    std::this_thread::sleep_for(milliseconds(100));
    std::cout << t.get_time() << std::endl;
    t.toggle();
    std::this_thread::sleep_for(milliseconds(200));
    std::cout << t.get_time() << std::endl;
    t.toggle();
    std::this_thread::sleep_for(milliseconds(250));
    std::cout << t.get_time() << std::endl;
    t.restart();
    std::this_thread::sleep_for(milliseconds(100));
    std::cout << t.get_time() << std::endl;
    t.toggle();
    std::this_thread::sleep_for(milliseconds(200));
    std::cout << t.get_time() << std::endl;
    t.toggle();
    std::this_thread::sleep_for(milliseconds(250));
    std::cout << t.get_time() << std::endl;
    return 0;
}
