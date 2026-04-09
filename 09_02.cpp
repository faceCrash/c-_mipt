#include <iostream>
#include <memory>
#include <queue>

/**
 * @brief Class Tree represents a binary tree.
 */
class Tree {
public:
    /**
     * @brief Node structure represents a tree node.
     * Nested publicly in Tree class.
     */
    struct Node {
        int value;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> parent; // Use weak_ptr to avoid circular dependencies

        Node(int val) : value(val) {}
        
        // Destructor to demonstrate correct cleanup
        ~Node() {
            std::cout << "[DEBUG] Deleting node with value: " << value << std::endl;
        }
    };

    std::shared_ptr<Node> root;

    /**
     * @brief BFS traversal (Breadth-First Search).
     */
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
            
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        std::cout << std::endl;
    }

    /**
     * @brief DFS traversal (Depth-First Search - Pre-order).
     */
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
    void dfs_preorder(const std::shared_ptr<Node>& node) {
        if (!node) return;
        
        std::cout << node->value << " ";
        dfs_preorder(node->left);
        dfs_preorder(node->right);
    }
};

/**
 * Helper to create a child node and set its parent.
 */
std::shared_ptr<Tree::Node> create_child(int val, std::shared_ptr<Tree::Node> parent, bool is_left) {
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
        
        // Constructing the tree:
        // Level 0: Root
        // Level 1: 2 nodes
        // Level 2: 4 nodes
        
        // Root
        tree.root = std::make_shared<Tree::Node>(1);
        
        // Intermediate Level
        auto n2 = create_child(2, tree.root, true);
        auto n3 = create_child(3, tree.root, false);
        
        // Leaf Level
        create_child(4, n2, true);
        create_child(5, n2, false);
        create_child(6, n3, true);
        create_child(7, n3, false);
        
        std::cout << "Tree constructed (7 nodes total)." << std::endl;
        
        // Demonstrate traversals
        tree.traverse_v1();
        tree.traverse_v2();
        
        std::cout << "Tree object is about to go out of scope." << std::endl;
    }
    
    std::cout << "Tree should be fully destroyed now." << std::endl;
    
    return 0;
}
