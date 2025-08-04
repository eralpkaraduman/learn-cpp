#include <iostream>
#include <format>
#include <vector>
#include <ranges>

int main() {
    // Modern C++23 features example
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    // Using ranges and format (C++20/C++23 features)
    std::cout << std::format("Numbers: ");
    for (auto n : numbers | std::views::filter([](int x) { return x % 2 == 0; })) {
        std::cout << std::format("{} ", n);
    }
    std::cout << "\n";
    
    std::cout << "C++23 setup ready for 'A Tour of C++'\n";
    return 0;
}