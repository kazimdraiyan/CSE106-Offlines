#include <iostream>
using namespace std;

class Node
{
public:
    int value;
    Node *next;
    Node *prev;

    Node(int val) : value(val)
    {
        next = nullptr;
        prev = nullptr;
    }
};

class LinkedList
{
    Node *head;
    Node *tail;
    Node *current;

    void init()
    {
        head = nullptr;
        tail = nullptr;
        current = nullptr;
    }

    void delete_nodes()
    {
        Node *node = head;
        while (node != nullptr)
        {
            Node *temp = node->next;
            delete node;
            node = temp;
        }
    }

    Node *find_node(int index)
    {
        if (index < 0)
            return nullptr;

        int i = 0;
        Node *node = head;
        while (node != nullptr)
        {
            if (i == index)
                return node;
            node = node->next;
            i++;
        }
        return nullptr;
    }

public:
    LinkedList()
    {
        init();
    }

    ~LinkedList()
    {
        delete_nodes();
    }

    void clear()
    {
        delete_nodes();
        init();
    }

    int size()
    {
        Node *node = head;
        int i = 0;
        while (node != nullptr)
        {
            i++;
            node = node->next;
        }
        return i;
    }

    bool is_present(int value)
    {
        Node *node = head;
        while (node != nullptr)
        {
            if (node->value == value)
                return true;
            node = node->next;
        }
        return false;
    }

    int find(int index)
    {
        Node *node = find_node(index);
        if (node != nullptr)
        {
            current = node;
            return node->value;
        }
        else
            return -1;
    }

    int update(int index, int value)
    {
        Node *node = find_node(index);
        if (node != nullptr)
        {
            int old_value = node->value;
            node->value = value;

            current = node;

            return old_value;
        }
        else
            return -1;
    }

    int search(int value)
    {
        int index = 0;
        Node *node = head;
        while (node != nullptr)
        {
            if (node->value == value)
            {
                current = node;
                return index;
            }
            node = node->next;
            index++;
        }
        return -1;
    }

    void insert(int value)
    {
        if (current == nullptr) // Empty list
        {
            head = new Node(value);
            tail = head;
            current = head;
        }
        else
        {
            Node *node = new Node(value);
            node->next = current->next;
            node->prev = current;
            current->next = node;
            if (node->next == nullptr)
                tail = node;
            else
                node->next->prev = node;
            current = node;
        }
    }

    void append(int value)
    {
        if (tail == nullptr) // Empty list
            insert(value);
        else
        {
            Node *node = new Node(value);
            node->prev = tail;
            tail->next = node;
            tail = node;
        }
    }

    int delete_cur()
    {
        if (current == nullptr)
            return -1;

        int deleted_value = current->value;

        if (current != head)
            current->prev->next = current->next;
        else
            head = current->next;

        if (current != tail)
        {
            current->next->prev = current->prev;
            Node *temp = current->next;
            delete current;
            current = temp;
        }
        else
        {
            tail = current->prev;
            delete current;
            current = tail;
        }

        return deleted_value;
    }

    int trim()
    {
        if (current == tail)
            return delete_cur();
        else
        {
            Node *old_current = current;
            current = tail;
            int deleted_value = delete_cur();
            current = old_current;
            return deleted_value;
        }
    }

    bool delete_item(int value)
    {
        int index = search(value);
        if (index != -1)
        {
            delete_cur();
            return true;
        }
        else
            return false;
    }

    void swap_ind(int index1, int index2)
    {
        Node *node1 = find_node(index1);
        Node *node2 = find_node(index2);
        if (node1 != nullptr && node2 != nullptr)
        {
            int temp = node1->value;
            node1->value = node2->value;
            node2->value = temp;
        }
    }

    void prev(int n)
    {
        for (int i = 0; i < n; i++)
        {
            if (current != nullptr && current->prev != nullptr)
                current = current->prev;
            else
                break;
        }
    }

    void next(int n)
    {
        for (int i = 0; i < n; i++)
        {
            if (current != nullptr && current->next != nullptr)
                current = current->next;
            else
                break;
        }
    }

    void reverse()
    {
        if (head != tail) // When size is not 0 or 1
        {
            Node *left = head;
            Node *right = tail;
            while (true)
            {
                int temp = left->value;
                left->value = right->value;
                right->value = temp;
                if (left->next == right || left->next == right->prev)
                    break;
                left = left->next;
                right = right->prev;
            }
        }
    }

    void print()
    {
        if (head == nullptr)
            cout << "[ . ]" << endl;
        else
        {
            Node *node = head;
            cout << "[ ";
            while (node != nullptr)
            {
                cout << node->value;
                if (node == current)
                    cout << "|";
                cout << " ";
                node = node->next;
            }
            cout << "]" << endl;
        }
    }

    // void debug_print()
    // {
    //     print();
    //     cout << "Head: " << head << endl;
    //     cout << "Tail: " << tail << endl;
    //     cout << "Current: " << current << endl;

    //     // Chain print (forward)
    //     Node *node = head;
    //     cout << "[Head] ";
    //     while (node != nullptr)
    //     {
    //         cout << node << " ";
    //         if (node == current)
    //             cout << "[Current] ";
    //         if (node->next != nullptr)
    //             cout << "-> ";
    //         node = node->next;
    //     }
    //     cout << "[Tail]" << endl;

    //     // Chain print (backward)
    //     node = tail;
    //     cout << "[Tail] ";
    //     while (node != nullptr)
    //     {
    //         cout << node << " ";
    //         if (node == current)
    //             cout << "[Current] ";
    //         if (node->prev != nullptr)
    //             cout << "-> ";
    //         node = node->prev;
    //     }
    //     cout << "[Head]" << endl
    //          << endl;
    // }
};