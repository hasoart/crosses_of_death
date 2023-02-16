#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <random>
#include <iterator>
#include <chrono>
#include <numeric>

#define range(container) std::begin(container), std::end(container)

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    std::copy(range(v), std::ostream_iterator<T>(os, " "));

    return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& v) {
    std::copy(std::istream_iterator<T>(is), std::istream_iterator<T>(), std::back_inserter(v));
    return is;
}

bool is_prime(int x) {
    if (x <= 1) return false;
    if (x == 2) return true;

    for (unsigned i = 2; i < std::sqrt(x) + 1; ++i) {
        if (!(x % i)) return false;
    }

    return true;
}

int main() {
    // пункт 1
    std::vector<int> p1(10);
    std::iota(range(p1), 1);
    std::cout << "1) " << p1 << std::endl;
    
    // пункт 2
    std::cin >> p1;
    std::cout << "2) " << p1 << std::endl;

    // пункт 3
    std::mt19937_64 mt(std::chrono::steady_clock::now().time_since_epoch().count());
    std::shuffle(range(p1), mt);
    std::cout << "3) " << p1 << std::endl;

    // пункт 4
    std::set<int> unique_elements;
    std::vector<int> p1_unique;
    for (auto elem: p1) {
        if (unique_elements.find(elem) == unique_elements.end()) {
            unique_elements.insert(elem);
            p1_unique.push_back(elem);
        }
    }
    
    p1 = p1_unique;
    std::cout << "4) " << p1 << std::endl;

    // пункт 5
    unsigned odd_count = std::count_if(range(p1), [](int x){return x%2;});
    std::cout << "5) " << odd_count << std::endl;

    // пункт 6
    auto minmax = std::minmax_element(range(p1));
    std::cout << "6) min: " << *minmax.first << ", max: " << *minmax.second << std::endl; 

    // пункт 7
    auto first_prime = std::find_if(range(p1), is_prime);
    if (first_prime == std::end(p1))
        std::cout << "7) There is no prime number in the sequence." << std::endl; 
    else
        std::cout << "7) First prime number: " << *first_prime << std::endl;
    
    // пункт 8
    std::vector<int> p1_squared;
    std::transform(range(p1), std::back_inserter(p1_squared), [](int x){return x*x;});
    p1 = p1_squared;
    std::cout << "8) " << p1_squared << std::endl;

    // пункт 9
    std::uniform_int_distribution<int> d(-50, 200);
    std::vector<int> p2(p1.size());
    for (auto &i: p2) i = d(mt);
    std::cout << "9) " << p2 << std::endl;

    // пункт 10
    std::cout << "10) " << std::accumulate(range(p2), 0) << std::endl;

    // пункт 11
    std::fill(std::begin(p2), std::begin(p2) + 3, 1);
    std::cout << "11) " << p2 << std::endl;

    // пункт 12
    std::vector<int> p3;
    std::transform(range(p2), std::begin(p1), std::back_inserter(p3),
                  [](auto lha, auto rha){return lha - rha;});
    std::cout << "12) " << p3 << std::endl;

    // пункт 13
    std::replace_if(range(p3), [](auto i){return i < 0;}, 0);
    std::cout << "13) " << p3 << std::endl;

    // пункт 14
    p3.erase(std::remove_if(range(p3), [](auto i){return i == 0;}), std::end(p3));
    std::cout << "14) " << p3 << std::endl;

    // пункт 15
    std::vector<int> temp;
    std::reverse_copy(range(p3), std::back_inserter(temp));
    p3 = temp;
    std::cout << "15) " << p3 << std::endl;

    // пункт 16
    auto p3_copy(p3);
    std::nth_element(std::begin(p3_copy), std::begin(p3_copy) + 3, std::end(p3_copy),
                     [](auto lha, auto rha){return lha > rha;});
    std::cout << "16) " << p3_copy[0] << ", " << p3_copy[1] << ", " << p3_copy[2] << std::endl;

    // пункт 17
    std::sort(range(p1));
    std::sort(range(p2));

    std::cout << "17) p1: " << p1 << "\n    p2: " << p2 << std::endl;

    // пункт 18
    std::vector<int> p4;
    std::merge(range(p1), range(p2), std::back_inserter(p4));
    std::cout << "18) " << p4 << std::endl;

    // пункт 19
    // [first, last)
    auto partition_first = std::partition(std::begin(p4), std::end(p4), [](auto x){return x < 1;});
    auto partition_last = std::partition(p4.rbegin(), p4.rend(), [](auto x){return x > 1;});
    std::cout << "19) first: " << partition_first - std::begin(p4) << ", last: "
        << p4.size() - (partition_last - p4.rbegin()) << std::endl;

    // пункт 20
    std::cout << "/---------20---------/" <<
        "\np1: " << p1 << "\n" <<
        "\np2: " << p2 << "\n" <<
        "\np3: " << p3 << "\n" <<
        "\np4: " << p4 << std::endl;

    return 0;
}
