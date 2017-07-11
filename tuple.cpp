/*
  Little snippets of use of std::tuple, along with std::get<n> and std::tie.
  Use -std=c++14 or c++11 to compile, if it is not already the default.
*/
#include <iostream>
#include <vector>
#include <tuple> // In Apple LLVM Clang, tuple is already included in iostream.
                 // But not so in GCC 7.

std::tuple<int, int> MinMax(std::vector<int> vector);

struct Node;
Node* InsertInBST(Node* root, int value);
std::tuple<int, int> MinMaxBST(Node* root);

int main() {

    // make a vector, take its min-max

    std::vector<int> vec { 3, 2, 4, 9, 2, 1, 7 };

    auto minmax = MinMax(vec);
    std::cout << "min(vec) = " << std::get<0>(minmax) << ", max(vec) = " << std::get<1>(minmax) << std::endl;

    // make a BST, take its min-max

    Node* bst = nullptr;
    for (auto x : vec) bst = InsertInBST(bst, x);

    int min, max;
    std::tie(min, max) = MinMaxBST(bst);
    std::cout << "min(bst) = " << min << ", max(bst) = " << max << std::endl;

    return 0;
}

std::tuple<int, int> MinMax(std::vector<int> vector) {
    auto size = vector.size();
    if (size == 0) return { 0, 0 };

    auto min = vector[0];
    auto max = vector[0];

    for (std::vector<int>::size_type i = 1; i < size; i++) {
        min = std::min(min, vector[i]);
        max = std::max(max, vector[i]);
    }

    return { min, max };
}

struct Node {
    int value;
    Node* left;
    Node* right;
};

Node* InsertInBST(Node* root, int value) {
    auto node = new Node { value, nullptr, nullptr };

    if (root == nullptr) return node;

    Node* n = root;
    while (1) {
        if (value <= n->value) {
            if (n->left == nullptr) {
                n->left = node;
                return root;
            } else {
                n = n->left;
            }
        }
        else {
            if (n->right == nullptr) {
                n->right = node;
                return root;
            } else {
                n = n->right;
            }
        }
    }

    // unreachable code. the loop will either return from within or not terminate.
    return nullptr;
}

std::tuple<int, int> MinMaxBST(Node* root) {
    if (root == nullptr) return { 0, 0 };

    auto leftmostnode = root;
    while (leftmostnode->left != nullptr) leftmostnode = leftmostnode->left;
    auto min = leftmostnode->value;

    auto rightmostnode = root;
    while (rightmostnode->right != nullptr) rightmostnode = rightmostnode->right;
    auto max = rightmostnode->value;

    return { min, max };
}
