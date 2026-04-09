#include <boost/multi_array.hpp>
#include <iostream>

typedef boost::multi_array<int, 2> Grid;

void print_grid(const Grid &grid) {
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      std::cout << (grid[i][j] ? "■ " : "· ");
    }
    std::cout << "\n";
  }
  std::cout << std::endl;
}

int count_neighbors(const Grid &grid, int x, int y) {
  int count = 0;
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0)
        continue;
      int nx = x + dx;
      int ny = y + dy;

      if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10) {
        count += grid[nx][ny];
      }
    }
  }
  return count;
}

void update_generation(const Grid &current, Grid &next) {
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      int neighbors = count_neighbors(current, i, j);
      if (current[i][j]) {

        next[i][j] = (neighbors == 2 || neighbors == 3);
      } else {

        next[i][j] = (neighbors == 3);
      }
    }
  }
}

int main() {
  std::cout << "Task 10.03: Game of Life (10x10 Field)\n" << std::endl;

  Grid current(boost::extents[10][10]);
  Grid next(boost::extents[10][10]);

  current[1][2] = 1;
  current[2][3] = 1;
  current[3][1] = 1;
  current[3][2] = 1;
  current[3][3] = 1;

  for (int iter = 0; iter < 10; ++iter) {
    std::cout << "Iteration " << iter << ":" << std::endl;
    print_grid(current);
    update_generation(current, next);
    current = next;
  }

  return 0;
}
