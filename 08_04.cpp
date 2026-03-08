#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int calculate_metric(const std::string &current, const std::string &target) {
  int diff = 0;
  for (std::size_t i = 0; i < current.size(); ++i) {
    if (current[i] != target[i]) {
      diff++;
    }
  }
  return diff;
}

int main() {
  const std::string target = "methinksitislikeaweasel";
  const std::size_t length = target.size(); // 23
  const int num_copies = 100;
  const double mutation_rate = 0.05;

  std::random_device rd;
  std::default_random_engine engine(rd());
  std::uniform_int_distribution<int> char_dist('a', 'z');
  std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

  std::string parent;
  parent.reserve(length);
  for (std::size_t i = 0; i < length; ++i) {
    parent += static_cast<char>(char_dist(engine));
  }

  int generation = 0;
  int best_metric = calculate_metric(parent, target);

  std::cout << "Generation " << generation << " | Metric: " << best_metric
            << " | String: " << parent << "\n";

  while (best_metric > 0) {
    generation++;

    std::vector<std::string> children(num_copies, parent);

    for (int i = 0; i < num_copies; ++i) {
      for (std::size_t j = 0; j < length; ++j) {
        if (prob_dist(engine) <= mutation_rate) {
          children[i][j] = static_cast<char>(char_dist(engine));
        }
      }
    }

    int next_best_metric = length + 1;
    std::string next_parent = parent;

    for (const auto &child : children) {
      int current_metric = calculate_metric(child, target);
      if (current_metric < next_best_metric) {
        next_best_metric = current_metric;
        next_parent = child;
      }
    }

    parent = next_parent;
    best_metric = next_best_metric;

    std::cout << "Generation " << generation << " | Metric: " << best_metric
              << " | String: " << parent << "\n";
  }

  std::cout << "Evolution completed in " << generation << " generations.\n";

  return 0;
}
