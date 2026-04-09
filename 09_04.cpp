#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>

/**
 * @brief Partition function using iterators.
 * Uses the last element as pivot.
 */
template<typename Iter, typename Compare = std::less<typename std::iterator_traits<Iter>::value_type>>
Iter partition_iter(Iter first, Iter last, Compare comp = Compare()) {
    auto pivot_iter = std::prev(last);
    auto pivot = *pivot_iter;
    auto i = first;

    for (auto j = first; j != pivot_iter; ++j) {
        if (comp(*j, pivot)) {
            std::iter_swap(i, j);
            std::advance(i, 1);
        }
    }
    std::iter_swap(i, pivot_iter);
    return i;
}

/**
 * @brief QuickSort using iterators and half-open ranges [first, last).
 */
template<typename Iter, typename Compare = std::less<typename std::iterator_traits<Iter>::value_type>>
void quickSort_iter(Iter first, Iter last, Compare comp = Compare()) {
    if (std::distance(first, last) > 1) {
        auto pi = partition_iter(first, last, comp);
        quickSort_iter(first, pi, comp);          // Left part
        quickSort_iter(std::next(pi), last, comp); // Right part
    }
}

int main() {
    std::vector<int> arr = {34, 7, 23, 32, 5, 62, 32, 7, 0};

    std::cout << "Original vector: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    quickSort_iter(arr.begin(), arr.end());

    std::cout << "Sorted vector:   ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    // Works with std::array as well
    int c_arr[] = {10, -1, 5, 2, 0};
    quickSort_iter(std::begin(c_arr), std::end(c_arr));
    
    std::cout << "Sorted C array:  ";
    for (int x : c_arr) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
