#include <thread>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <random>
#include <functional>

using steady_clock_t = std::chrono::steady_clock;

template <typename T>
class Timer {
public:
	Timer(const std::string& message): message(message), begin(steady_clock_t::now()) {}
	Timer(): message(), begin(steady_clock_t::now()) {}
	
    auto operator()() {
        return std::chrono::duration_cast<T>(steady_clock_t::now() - begin).count();
    }

	~Timer() {
        if (message == "")
            return;
		std::cout << message << ": " << std::chrono::duration_cast<T>(steady_clock_t::now() - begin).count() << std::endl;
	}
private:
	steady_clock_t::time_point begin;
	std::string message;
};

template<typename Iter, typename T>
void accumulate_block(Iter begin, Iter end, T init, T& result) {
	result = std::accumulate(begin, end, init);
}

template<typename Iter, typename T>
T parallel_accumulate(Iter begin, Iter end, T init, unsigned num_threads) {
	std::vector<std::thread> threads;
	std::vector<T> results(num_threads - 1);
	auto block_size = std::distance(begin, end) / num_threads;
	for (auto i = 0u; i + 1 < num_threads; i++) {
		threads.push_back(std::thread(
			accumulate_block<Iter, T>,
			std::next(begin, i * block_size),
			std::next(begin, (i + 1) * block_size),
			0,
			std::ref(results[i]))
		);
	}
	T last_result;
	accumulate_block(std::next(begin, (num_threads - 1) * block_size),
			end, init, last_result);
	std::for_each(std::begin(threads),
                  std::end(threads),
                  std::mem_fn(&std::thread::join));
	return std::accumulate(std::begin(results), std::end(results), last_result);
}

int main() {
    const unsigned test_count = 10;
    std::vector<unsigned int> test_thread_counts =
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 16, 100, 500, 2000};

    std::vector<size_t> test_sizes = {100, 1'000, 10'000, 100'000,
        1'000'000, 10'000'000, 100'000'000};

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution d(1, 10);

    // time in microsecond
    std::cout << "size,thread_count,sum,time" << std::endl;

    for(auto size: test_sizes) {
    for(auto i = 0u; i != test_count; ++i) {
        std::vector<int64_t> numbers(size);
        std::generate_n(std::back_inserter(numbers),
                        size, [&gen, &d](){return d(gen);});

        for(auto thread_count: test_thread_counts) {
            Timer<std::chrono::microseconds> timer;
            auto result = parallel_accumulate(
                            std::begin(numbers),
                            std::end(numbers),
                            0ll,
                            thread_count);
            std::cout << size << "," << thread_count << "," <<
                result << "," << timer() << std::endl;
            
        }
    }      
    }
}
