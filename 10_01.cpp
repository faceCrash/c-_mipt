#include <cmath>
#include <deque>
#include <iomanip>
#include <iostream>
#include <vector>

void investigate_vector() {
  std::vector<int> v;
  size_t last_capacity = 0;
  std::cout << "--- Vector Capacity Growth ---" << std::endl;
  std::cout << std::setw(10) << "Size" << std::setw(15) << "Capacity"
            << std::setw(15) << "Ratio" << std::endl;

  for (int i = 0; i <= 512; ++i) {
    if (v.capacity() != last_capacity) {
      double ratio = (last_capacity == 0)
                         ? 0
                         : static_cast<double>(v.capacity()) / last_capacity;
      std::cout << std::setw(10) << v.size() << std::setw(15) << v.capacity()
                << std::setw(15) << std::fixed << std::setprecision(2) << ratio
                << std::endl;
      last_capacity = v.capacity();
    }
    v.push_back(i);
  }
  std::cout << std::endl;
}

void investigate_deque() {
  std::deque<int> d;
  std::cout << "--- Deque Page Size Investigation ---" << std::endl;

  int *last_ptr = nullptr;
  int first_jump_idx = -1;
  int page_size = 0;

  for (int i = 0; i < 5000; ++i) {
    d.push_back(i);
    int *current_ptr = &d.back();

    if (last_ptr != nullptr) {
      ptrdiff_t diff = current_ptr - last_ptr;

      if (std::abs(diff) != 1) {
        if (first_jump_idx == -1) {
          first_jump_idx = i;
        } else if (page_size == 0) {
          page_size = i - first_jump_idx;
          std::cout << "Memory jump detected at index " << i << std::endl;
          std::cout << "Elements per Page: " << page_size << std::endl;
          std::cout << "Page Size in Bytes: " << page_size * sizeof(int)
                    << " bytes" << std::endl;
          break;
        }
        first_jump_idx = i;
      }
    }
    last_ptr = current_ptr;
  }
}

int main() {
  std::cout << "Task 10.01: Container Capacity and Memory Layout" << std::endl
            << std::endl;
  investigate_vector();
  investigate_deque();
  return 0;
}
