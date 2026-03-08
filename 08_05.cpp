#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

/////////////////////////////////////////////////////////////////////////////////

template <typename D = std::chrono::duration<double>> class Timer {
public:
  Timer(std::string const &scope) : m_scope(scope), m_running(false) {}

  //  -----------------------------------------------------------------------------

  ~Timer() {
    if (m_running) {
      stop();
    }
    std::cout << m_scope << " : " << average() << " s (average)\n";
  }

  //  -----------------------------------------------------------------------------

  void start() {
    if (!m_running) {
      m_begin = clock_t::now();
      m_running = true;
    }
  }

  void stop() {
    if (m_running) {
      auto end_time = clock_t::now();
      m_intervals.push_back(std::chrono::duration_cast<D>(end_time - m_begin));
      m_running = false;
    }
  }

  double average() const {
    if (m_intervals.empty()) {
      return 0.0;
    }
    double sum = 0.0;
    for (const auto &interval : m_intervals) {
      sum += interval.count();
    }
    return sum / m_intervals.size();
  }

  //  -----------------------------------------------------------------------------

  auto elapsed() const {
    if (m_running) {
      return std::chrono::duration_cast<D>(clock_t::now() - m_begin);
    }
    return D(0);
  }

private:
  using clock_t = std::chrono::steady_clock;

  //  -----------------------------------------------------------------------------

  std::string m_scope;
  bool m_running;
  clock_t::time_point m_begin;
  std::vector<D> m_intervals;
};

/////////////////////////////////////////////////////////////////////////////////

auto calculate(std::size_t size) {
  auto x = 0.0;

  for (auto i = 0uz; i < size; ++i) {
    x += std::pow(std::sin(i), 2) + std::pow(std::cos(i), 2);
  }

  return x;
}

/////////////////////////////////////////////////////////////////////////////////

auto equal(double x, double y, double epsilon = 1e-6) {
  return std::abs(x - y) < epsilon;
}

/////////////////////////////////////////////////////////////////////////////////

int main() {
  Timer timer("main : timer");

  //  -----------------------------------------------
  // 1st run
  timer.start();
  assert(equal(calculate(1'000'000), 1'000'000));
  timer.stop();

  // 2nd run
  timer.start();
  assert(equal(calculate(1'000'000), 1'000'000));
  timer.stop();

  // 3rd run
  timer.start();
  assert(equal(calculate(1'000'000), 1'000'000));
  timer.stop();
}

/////////////////////////////////////////////////////////////////////////////////
