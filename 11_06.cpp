#include <algorithm>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/graph_traits.hpp>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <vector>

int main() {
  const int num_vertices = 10;

  typedef boost::adjacency_matrix<boost::undirectedS, boost::no_property,
                                  boost::property<boost::edge_weight_t, int>>
      Graph;
  Graph g(num_vertices);

  std::random_device rd;
  std::default_random_engine engine(rd());
  std::uniform_int_distribution<int> dist(1, 10);

  for (int i = 0; i < num_vertices; ++i) {
    for (int j = i + 1; j < num_vertices; ++j) {
      int weight = dist(engine);
      auto [e, added] = boost::add_edge(i, j, g);
      boost::put(boost::edge_weight, g, e, weight);
    }
  }

  auto weights = boost::get(boost::edge_weight, g);

  std::cout << "--- Task 11.06: TSP with Boost.Graph Adjacency Matrix ---\n\n";

  std::cout << "Adjacency Matrix (10x10):\n";
  for (int i = 0; i < num_vertices; ++i) {
    for (int j = 0; j < num_vertices; ++j) {
      if (i == j) {
        std::cout << std::setw(3) << 0;
      } else {
        auto [e, exists] = boost::edge(i, j, g);
        std::cout << std::setw(3) << (exists ? weights[e] : 0);
      }
    }
    std::cout << "\n";
  }

  std::cout << "\nIncidence Matrix (10 vertices x 45 edges):\n";
  std::cout << "Rows: Vertices V0-V9, Columns: All unique edges\n";

  std::vector<std::pair<int, int>> edges;
  for (int i = 0; i < num_vertices; ++i) {
    for (int j = i + 1; j < num_vertices; ++j) {
      edges.push_back({i, j});
    }
  }

  for (int i = 0; i < num_vertices; ++i) {
    std::cout << "V" << i << ": ";
    for (const auto &edge_pair : edges) {
      if (edge_pair.first == i || edge_pair.second == i) {
        auto [e, exists] = boost::edge(edge_pair.first, edge_pair.second, g);
        std::cout << std::setw(2) << weights[e] << " ";
      } else {
        std::cout << " . ";
      }
    }
    std::cout << "\n";
  }

  std::vector<int> vertices(num_vertices - 1);
  std::iota(vertices.begin(), vertices.end(), 1);

  int min_cost = std::numeric_limits<int>::max();
  std::vector<int> best_path;

  do {
    int current_cost = 0;
    int prev = 0;

    for (int v : vertices) {
      auto [e, exists] = boost::edge(prev, v, g);
      current_cost += weights[e];
      prev = v;
    }

    auto [e_return, exists_return] = boost::edge(prev, 0, g);
    current_cost += weights[e_return];

    if (current_cost < min_cost) {
      min_cost = current_cost;
      best_path = vertices;
    }
  } while (std::next_permutation(vertices.begin(), vertices.end()));

  std::cout << "\nOptimal sequence (V0 is fixed as start/end):\n";
  std::cout << "0 -> ";
  for (int v : best_path) {
    std::cout << v << " -> ";
  }
  std::cout << "0\n";
  std::cout << "Total Cost: " << min_cost << std::endl;

  return 0;
}
