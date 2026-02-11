#ifndef LISTBST_H
#define LISTBST_H

#include "BST.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

/**
 * Binary Search Tree implementation using linked list structure
 *
 * @tparam Key - The type of keys stored in the BST
 * @tparam Value - The type of values associated with keys
 */
template <typename Key, typename Value>
class ListBST : public BST<Key, Value>
{
private:
    /**
     * Node class for the binary search tree
     */
    class Node
    {
    public:
        Key key;
        Value value;
        Node *left;
        Node *right;

        Node(Key k, Value v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node *root;
    size_t node_count;

    // Helper functions
    void print_node(Node *node) const
    {
        if (node != nullptr)
        {
            cout << node->key << ":" << node->value;
        }
    }

    void print_nested_parentheses(Node *root) const
    {
        cout << "(";

        if (root != nullptr)
        {
            print_node(root);
            if (root->left != nullptr || root->right != nullptr)
            {
                cout << " ";
                print_nested_parentheses(root->left);
            }
            if (root->right != nullptr)
            {
                cout << " ";
                print_nested_parentheses(root->right);
            }
        }

        cout << ")";
    }

    void print_preorder(Node *root) const
    {
        if (root != nullptr)
        {
            cout << "(";
            print_node(root);
            cout << ") ";

            print_preorder(root->left);
            print_preorder(root->right);
        }
    }

    void print_inorder(Node *root) const
    {
        if (root != nullptr)
        {
            print_inorder(root->left);

            cout << "(";
            print_node(root);
            cout << ") ";

            print_inorder(root->right);
        }
    }

    void print_postorder(Node *root) const
    {
        if (root != nullptr)
        {
            print_postorder(root->left);
            print_postorder(root->right);

            cout << "(";
            print_node(root);
            cout << ") ";
        }
    }

    void clear_recursively(Node *root)
    {
        if (root != nullptr)
        {
            clear_recursively(root->left);
            clear_recursively(root->right);
            delete root;
        }
    }

    Node *find_node(Key key) const
    {
        Node *current_node = root;
        while (current_node != nullptr)
        {
            if (key > current_node->key)
                current_node = current_node->right;
            else if (key < current_node->key)
                current_node = current_node->left;
            else
                return current_node;
        }
        return nullptr;
    }

    Node *find_node_with_min_key(Node *root) const
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        Node *current_node = root;
        while (current_node->left != nullptr)
        {
            current_node = current_node->left;
        }
        return current_node;
    }

public:
    /**
     * Constructor
     */
    ListBST() : root(nullptr), node_count(0) {}

    /**
     * Destructor
     */
    ~ListBST()
    {
        clear();
    }

    /**
     * Insert a key-value pair into the BST
     */
    bool insert(Key key, Value value) override
    {
        if (root == nullptr)
        {
            root = new Node(key, value);
            node_count++;
            return true;
        }

        Node *current_node = root;
        while (true)
        {
            if (key > current_node->key)
            {
                if (current_node->right == nullptr)
                {
                    Node *node = new Node(key, value);
                    current_node->right = node;
                    break;
                }
                else
                {
                    current_node = current_node->right;
                }
            }
            else if (key < current_node->key)
            {
                if (current_node->left == nullptr)
                {
                    Node *node = new Node(key, value);
                    current_node->left = node;
                    break;
                }
                else
                {
                    current_node = current_node->left;
                }
            }
            else
            {
                return false;
            }
        }
        node_count++;
        return true;
    }

    /**
     * Remove a key-value pair from the BST
     */
    bool remove(Key key) override
    {
        if (root == nullptr)
        {
            // Empty BST
            return false;
        }

        Node *parent = nullptr;
        Node *node = root;
        while (node != nullptr)
        {
            if (key > node->key)
            {
                parent = node;
                node = node->right;
            }
            else if (key < node->key)
            {
                parent = node;
                node = node->left;
            }
            else
                break;
        }
        if (node == nullptr)
        {
            // Key not found
            return false;
        }

        Node *left = node->left;
        Node *right = node->right;

        if (left == nullptr && right == nullptr)
        {
            // Case 1: Leaf node

            if (parent != nullptr)
            {
                if (parent->right == node)
                {
                    parent->right = nullptr;
                }
                else if (parent->left == node)
                {
                    parent->left = nullptr;
                }
            }
            else
            {
                // The BST has only one node: the root node
                root = nullptr;
            }
            delete node;
        }
        else if (left == nullptr)
        {
            // Case 2: Node has only right subtree

            if (parent != nullptr)
            {
                if (parent->right == node)
                {
                    parent->right = node->right;
                }
                else if (parent->left == node)
                {
                    parent->left = node->right;
                }
            }
            else
            {
                // Node is root node
                root = node->right;
            }
            delete node;
        }
        else if (right == nullptr)
        {
            // Case 2: Node has only left subtree

            if (parent != nullptr)
            {
                if (parent->right == node)
                {
                    parent->right = node->left;
                }
                else if (parent->left == node)
                {
                    parent->left = node->left;
                }
            }
            else
            {
                // Node is root node
                root = node->left;
            }
            delete node;
        }
        else
        {
            // Case 3: Node has both left and right subtrees

            // Find successor (right child's leftmost node)
            Node *successor = find_node_with_min_key(node->right);
            // Copy successor's content to node, and remove successor
            Key successor_key = successor->key;
            Value successor_value = successor->value;
            remove(successor_key); // We know for sure that either Case 1 or Case 2 will be executed, because successor does not have a left subtree.
            node->key = successor_key;
            node->value = successor_value;
            node_count++; // Because it thinks we removed 2 nodes. But actually we removed 1 node, and another node's content is just changed.
        }
        node_count--;
        return true;
    }

    /**
     * Find if a key exists in the BST
     */
    bool find(Key key) const override
    {
        Node *node = find_node(key);
        return node != nullptr;
    }

    /**
     * Find a value associated with a given key
     */
    Value get(Key key) const override
    {
        Node *node = find_node(key);
        if (node != nullptr)
        {
            return node->value;
        }
        throw runtime_error("Key not found.");
    }

    /**
     * Update the value associated with a given key
     */
    void update(Key key, Value value) override
    {
        Node *node = find_node(key);
        if (node != nullptr)
        {
            node->value = value;
        }
        else
        {
            throw runtime_error("Key not found.");
        }
    }

    /**
     * Clear all elements from the BST
     */
    void clear() override
    {
        clear_recursively(root);
        root = nullptr;
        node_count = 0;
    }

    /**
     * Get the number of keys in the BST
     */
    size_t size() const override
    {
        return node_count;
    }

    /**
     * Check if the BST is empty
     */
    bool empty() const override
    {
        return node_count == 0;
    }

    /**
     * Find the minimum key in the BST
     */
    Key find_min() const override
    {
        Node *min_node = find_node_with_min_key(root);
        if (min_node == nullptr)
        {
            throw runtime_error("BST is empty.");
        }
        return min_node->key;
    }

    /**
     * Find the maximum key in the BST
     */
    Key find_max() const override
    {
        if (root == nullptr)
        {
            throw runtime_error("BST is empty.");
        }

        Node *current_node = root;
        while (current_node->right != nullptr)
        {
            current_node = current_node->right;
        }
        return current_node->key;
    }

    /**
     * Print the BST using specified traversal method
     */
    void print(char traversal_type = 'D') const override
    {
        if (traversal_type == 'D' || traversal_type == 'd')
            print_nested_parentheses(root);
        else if (traversal_type == 'I' || traversal_type == 'i')
            print_inorder(root);
        else if (traversal_type == 'P' || traversal_type == 'p')
            print_preorder(root);
        else if (traversal_type == 'O' || traversal_type == 'o')
            print_postorder(root);
        else
            throw invalid_argument("Invalid traversal type.");
    }

    // void debug_print() const
    // {
    //     cout << "Number of nodes: " << node_count << endl;
    //     cout << "BST: ";
    //     print();
    //     cout << endl;

    //     cout << "Preorder: ";
    //     print('P');
    //     cout << endl;

    //     cout << "Inorder: ";
    //     print('I');
    //     cout << endl;

    //     cout << "Postorder: ";
    //     print('O');
    //     cout << endl
    //          << endl;
    // }
};

#endif // LISTBST_H