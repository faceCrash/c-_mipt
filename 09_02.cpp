#include <iostream>
#include <memory>
#include <queue>

class Tree {
public:
  struct Node {
    int value;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    std::weak_ptr<Node> parent;

    Node(int val) : value(val) {}

    ~Node() {
      std::cout << "[DEBUG] Deleting node with value: " << value << std::endl;
    }
  };

  std::shared_ptr<Node> root;

  void traverse_v1() {
    std::cout << "BFS Traversal: ";
    if (!root) {
      std::cout << "(empty)" << std::endl;
      return;
    }

    std::queue<std::shared_ptr<Node>> q;
    q.push(root);

    while (!q.empty()) {
      auto current = q.front();
      q.pop();

      std::cout << current->value << " ";

      if (current->left)
        q.push(current->left);
      if (current->right)
        q.push(current->right);
    }
    std::cout << std::endl;
  }

  void traverse_v2() {
    std::cout << "DFS Traversal (Pre-order): ";
    if (!root) {
      std::cout << "(empty)" << std::endl;
      return;
    }
    dfs_preorder(root);
    std::cout << std::endl;
  }

private:
  void dfs_preorder(const std::shared_ptr<Node> &node) {
    if (!node)
      return;

    std::cout << node->value << " ";
    dfs_preorder(node->left); // Use std::stack, recursion can cause stack overflow in big trees
    dfs_preorder(node->right);
  }
};

std::shared_ptr<Tree::Node>
create_child(int val, std::shared_ptr<Tree::Node> parent, bool is_left) {
  auto child = std::make_shared<Tree::Node>(val);
  child->parent = parent;
  if (is_left) {
    parent->left = child;
  } else {
    parent->right = child;
  }
  return child;
}

int main() {
  {
    Tree tree;

    tree.root = std::make_shared<Tree::Node>(1);

    auto n2 = create_child(2, tree.root, true);
    auto n3 = create_child(3, tree.root, false);

    create_child(4, n2, true);
    create_child(5, n2, false);
    create_child(6, n3, true);
    create_child(7, n3, false);

    std::cout << "Tree constructed (7 nodes total)." << std::endl;

    tree.traverse_v1();
    tree.traverse_v2();

    std::cout << "Tree object is about to go out of scope." << std::endl;
  }

  std::cout << "Tree should be fully destroyed now." << std::endl;

  return 0;
}

// Score is 9/10
