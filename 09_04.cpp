#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

template <typename Iter, typename Compare = std::less<
                             typename std::iterator_traits<Iter>::value_type>>
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

template <typename Iter, typename Compare = std::less<
                             typename std::iterator_traits<Iter>::value_type>>
void quickSort_iter(Iter first, Iter last, Compare comp = Compare()) {
  if (std::distance(first, last) > 1) {
    auto pi = partition_iter(first, last, comp);
    quickSort_iter(first, pi, comp);
    quickSort_iter(std::next(pi), last, comp);
  }
}

bool compare_descending(int a, int b) { return a > b; }

int main() {
  std::cout << "--- Task 11.03 ---" << std::endl;

  std::vector<int> arr = {34, 7, 23, 32, 5, 62, 32, 7, 0};

  std::cout << "Original vector: ";
  for (int x : arr)
    std::cout << x << " ";
  std::cout << "\n\n";

  std::vector<int> arr1 = arr;
  quickSort_iter(arr1.begin(), arr1.end(), std::less<int>());
  std::cout << "1. Sorted with std::less:             ";
  for (int x : arr1)
    std::cout << x << " ";
  std::cout << "\n";

  std::vector<int> arr2 = arr;
  quickSort_iter(arr2.begin(), arr2.end(), compare_descending);
  std::cout << "2. Sorted with free function (desc):  ";
  for (int x : arr2)
    std::cout << x << " ";
  std::cout << "\n";

  std::vector<int> arr3 = {10, -5, 20, -30, 0, 5};
  quickSort_iter(arr3.begin(), arr3.end(),
                 [](int a, int b) { return std::abs(a) < std::abs(b); });
  std::cout << "3. Absolute sort with lambda:         ";
  for (int x : arr3)
    std::cout << x << " ";
  std::cout << "\n";

  return 0;
}

// Score is 10/10
