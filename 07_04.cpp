#include <algorithm>
#include <functional>
#include <gtest/gtest.h>
#include <vector>

template <typename T, typename Compare = std::less<T>>
void insertionSort(std::vector<T> &arr, int low, int high,
                   Compare comp = Compare()) {
  for (int i = low + 1; i <= high; ++i) {
    T key = arr[i];
    int j = i - 1;
    while (j >= low && comp(key, arr[j])) {
      arr[j + 1] = arr[j];
      j = j - 1;
    }
    arr[j + 1] = key;
  }
}

template <typename T, typename Compare = std::less<T>>
int partition(std::vector<T> &arr, int low, int high,
              Compare comp = Compare()) {
  T pivot = arr[high];
  int i = low - 1;

  for (int j = low; j < high; ++j) {
    if (!comp(pivot, arr[j])) {
      ++i;
      std::swap(arr[i], arr[j]);
    }
  }
  std::swap(arr[i + 1], arr[high]);
  return i + 1;
}

template <typename T, typename Compare = std::less<T>>
void hybridSort(std::vector<T> &arr, int low, int high, int threshold = 16,
                Compare comp = Compare()) {
  while (low < high) {
    if (high - low < threshold) {
      insertionSort(arr, low, high, comp);
      break;
    } else {
      int pi = partition(arr, low, high, comp);

      if (pi - low < high - pi) {
        hybridSort(arr, low, pi - 1, threshold, comp);
        low = pi + 1;
      } else {
        hybridSort(arr, pi + 1, high, threshold, comp);
        high = pi - 1;
      }
    }
  }
}

template <typename T, typename Compare = std::less<T>>
void mySort(std::vector<T> &arr, int threshold = 16, Compare comp = Compare()) {
  if (!arr.empty()) {
    hybridSort(arr, 0, arr.size() - 1, threshold, comp);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////

TEST(SortTest, EmptyVector) {
  std::vector<int> v;
  mySort(v);
  EXPECT_TRUE(v.empty());
}

TEST(SortTest, SingleElement) {
  std::vector<int> v = {42};
  mySort(v);
  EXPECT_EQ(v, std::vector<int>{42});
}

TEST(SortTest, SortedVector) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  mySort(v);
  EXPECT_EQ(v, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(SortTest, ReverseSortedVector) {
  std::vector<int> v = {5, 4, 3, 2, 1};
  mySort(v);
  EXPECT_EQ(v, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(SortTest, Duplicates) {
  std::vector<int> v = {3, 1, 2, 1, 3, 2};
  mySort(v);
  EXPECT_EQ(v, (std::vector<int>{1, 1, 2, 2, 3, 3}));
}

TEST(SortTest, DoubleVector) {
  std::vector<double> v = {3.4, 1.2, 5.6, 0.1};
  mySort(v);
  EXPECT_EQ(v, (std::vector<double>{0.1, 1.2, 3.4, 5.6}));
}

TEST(SortTest, CustomComparator) {
  std::vector<int> v = {1, 4, 2, 5, 3};
  mySort(v, 16, std::greater<int>());
  EXPECT_EQ(v, (std::vector<int>{5, 4, 3, 2, 1}));
}
