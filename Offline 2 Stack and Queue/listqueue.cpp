#include "queue.h"
#include <iostream>

using namespace std;

// Constructor implementation
ListQueue::ListQueue()
{
    front_node = nullptr;
    rear_node = nullptr;
    current_size = 0;
}

// Destructor implementation
ListQueue::~ListQueue()
{
    clear();
}

// Enqueue implementation (add an item at the rear of the queue)
void ListQueue::enqueue(int item)
{
    Node *node = new Node(item);
    if (rear_node == nullptr)
    {
        // Empty queue
        front_node = node;
    }
    else {
        rear_node->next = node;
    }
    rear_node = node;
    current_size++;
}

// Dequeue implementation (remove an item from the front of the queue)
int ListQueue::dequeue()
{
    if (front_node == nullptr) {
        // throw out_of_range("Queue is empty");
        cout << "Queue is empty" << endl;
        return -1;
    }

    int data = front_node->data;
    Node *next = front_node->next;

    delete front_node;

    front_node = next;
    if (front_node == nullptr) {
        rear_node = nullptr;
    }
    current_size--;

    return data;
}

// Clear implementation (delete all elements)
void ListQueue::clear()
{
    Node *node = front_node;
    while (node != nullptr) {
        Node *next = node->next;
        delete node;
        node = next;
    }
    front_node = nullptr;
    rear_node = nullptr;
    current_size = 0;
}

// Size implementation (return the current number of elements)
int ListQueue::size() const
{
    return current_size;
}

// Front implementation (get the element at the front of the queue)
int ListQueue::front() const
{
    if (front_node == nullptr) {
        // throw out_of_range("Queue is empty");
        cout << "Queue is empty" << endl;
        return -1;
    }
    return front_node->data;
}

// Back implementation (get the element at the back of the queue)
int ListQueue::back() const
{
    if (rear_node == nullptr) {
        // throw out_of_range("Queue is empty");
        cout << "Queue is empty" << endl;
        return -1;
    }
    return rear_node->data;
}

// Empty implementation (check if the queue is empty)
bool ListQueue::empty() const
{
    return front_node == nullptr;
}

// Print implementation (print elements from front to rear) in the format <elem1, elem2, ..., elemN|
string ListQueue::toString() const
{
    string result = "<";
    Node *node = front_node;
    while (node != nullptr) {
        result += to_string(node->data);
        if (node != rear_node) {
            result += ", ";
        }
        node = node->next;
    }
    result += "|";
    return result;
}
