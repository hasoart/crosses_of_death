#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <future>
#include <numeric>

template<class It, class UnaryFunction>
void for_each(It begin, It end, UnaryFunction &f) {
    auto num_threads = std::thread::hardware_concurrency();
    auto block_size = std::distance(begin, end) / num_threads;

    std::vector<std::future<void>> handles;
    
	for (auto i = 0u; i + 1 < num_threads; ++i) {
        handles.push_back(std::async(
            std::launch::async,
            std::for_each,
            std::next(begin, i * block_size),
            std::next(begin, (i + 1) * block_size),
            f));
	}

    handles.push_back(std::async(
        std::launch::async,
        std::for_each,
        std::next(begin, (num_threads - 1) * block_size),
        end,
        f));

}

int main() {
    size_t size;
    std::cout << "Enter array size: ";
    std::cin >> size;
    std::vector<int> arr(size);
    std::iota(std::begin(arr), std::end(arr), 0);
    
    for_each(std::begin(arr), std::end(arr), [](auto& x){ x = x * x; });

    const auto line_max = 10;
    unsigned counter = 1;
    for (auto el: arr) {
        std::cout << el << " ";
        if (counter % 10 == 0) {
            std::cout << std::endl;
            counter = 0;
        }
        ++counter;
    }
    std::cout << std::endl;
    return 0;
}
