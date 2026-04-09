#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <iomanip>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

/**
 * @brief Item 11.06 (11.29): Solving the Traveling Salesperson Problem (TSP).
 * Uses Boost.Graph for graph representation and brute-force 
 * permutation for finding the optimal path in a 10-vertex complete graph.
 */

int main() {
    const int num_vertices = 10;
    
    // Note: adjacency_list is used here instead of adjacency_matrix due to 
    // compatibility issues with modern C++ standard libraries and Boost.Graph.
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, 
                                 boost::no_property, boost::property<boost::edge_weight_t, int>> Graph;
    Graph g(num_vertices);
    auto weights = boost::get(boost::edge_weight, g);

    // Random weight generation
    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int> dist(1, 10);

    // List to keep track of edges for incidence matrix output
    std::vector<std::pair<int, int>> edges_list;

    // Initialize a full graph (every vertex connected to every other vertex)
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = i + 1; j < num_vertices; ++j) {
            int weight = dist(engine);
            boost::add_edge(i, j, weight, g);
            edges_list.push_back({i, j});
        }
    }

    std::cout << "--- Task 11.06: Traveling Salesperson Problem (TSP) ---\n\n";

    // Output Adjacency Matrix
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

    // Output Incidence Matrix
    // Vertices as rows, Edges as columns
    std::cout << "\nIncidence Matrix (10 vertices x 45 edges, weights shown at endpoints):\n";
    for (int i = 0; i < num_vertices; ++i) {
        std::cout << "V" << i << ": ";
        for (const auto& edge_pair : edges_list) {
            if (edge_pair.first == i || edge_pair.second == i) {
                auto [e, exists] = boost::edge(edge_pair.first, edge_pair.second, g);
                std::cout << std::setw(2) << weights[e] << " ";
            } else {
                std::cout << " . ";
            }
        }
        std::cout << "\n";
    }

    // Solving TSP using brute-force (all permutations of vertices 1 to 9)
    // Starting and ending at vertex 0.
    std::vector<int> path_permutation(num_vertices - 1);
    std::iota(path_permutation.begin(), path_permutation.end(), 1); // {1, 2, ..., 9}

    int min_total_cost = std::numeric_limits<int>::max();
    std::vector<int> optimal_path;

    do {
        int current_path_cost = 0;
        int current_vertex = 0;

        // Traverse through the permutation
        for (int next_vertex : path_permutation) {
            auto [e, exists] = boost::edge(current_vertex, next_vertex, g);
            current_path_cost += weights[e];
            current_vertex = next_vertex;
        }

        // Return to the starting vertex 0
        auto [e_return, exists_return] = boost::edge(current_vertex, 0, g);
        current_path_cost += weights[e_return];

        if (current_path_cost < min_total_cost) {
            min_total_cost = current_path_cost;
            optimal_path = path_permutation;
        }
    } while (std::next_permutation(path_permutation.begin(), path_permutation.end()));

    // Output results
    std::cout << "\nOptimal Sequence (starting and ending at V0):\n";
    std::cout << "0 -> ";
    for (int v : optimal_path) {
        std::cout << v << " -> ";
    }
    std::cout << "0\n";
    std::cout << "Total Path Cost: " << min_total_cost << std::endl;

    return 0;
}
