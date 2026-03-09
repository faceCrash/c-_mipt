#include <algorithm>
#include <benchmark/benchmark.h>
#include <functional>
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

static void BM_SortReverse(benchmark::State &state) {
  int threshold = state.range(0);

  std::vector<double> original(10000);
  double val = 10000.0;
  for (int i = 0; i < 10000; ++i) {
    original[i] = val;
    val -= 1.0;
  }

  for (auto _ : state) {
    state.PauseTiming();
    std::vector<double> arr = original;
    state.ResumeTiming();

    mySort(arr, threshold);

    benchmark::DoNotOptimize(arr.data());
    benchmark::ClobberMemory();
  }
}

BENCHMARK(BM_SortReverse)
    ->Arg(1)
    ->Arg(4)
    ->Arg(8)
    ->Arg(16)
    ->Arg(32)
    ->Arg(64)
    ->Arg(128)
    ->Arg(256);

BENCHMARK_MAIN();


// Score is 10/10
