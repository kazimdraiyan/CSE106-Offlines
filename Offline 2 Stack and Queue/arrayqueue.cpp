#include "queue.h"
#include <iostream>

using namespace std;

// Constructor implementation
ArrayQueue::ArrayQueue(int initial_capacity)
{
    if (initial_capacity < 2) {
        initial_capacity = 2;
    }
    data = new int[initial_capacity];
    capacity = initial_capacity;
    front_idx = -1;
    rear_idx = -1;
}

// Destructor implementation
ArrayQueue::~ArrayQueue()
{
    delete[] data;
}

// Enqueue implementation (add an item to the rear of the queue)
void ArrayQueue::enqueue(int item)
{
    // If full, resize the array to double its current capacity
    if (size() == capacity)
    {
        // The queue is full
        resize(capacity * 2);
    }

    rear_idx = (rear_idx + 1) % capacity;
    data[rear_idx] = item;

    if (front_idx == -1)
    {
        // Empty queue
        front_idx = 0;
    }
}

// Dequeue implementation (remove an item from the front of the queue)
int ArrayQueue::dequeue()
{
    if (empty()) {
        // throw out_of_range("Queue is empty");
        cout << "Queue is empty" << endl;
        return -1;
    }

    int value = data[front_idx];

    if (front_idx == rear_idx) {
        // Only one item
        front_idx = -1;
        rear_idx = -1;
    }
    else {
        front_idx = (front_idx + 1) % capacity;
    }

    // If the array is less than 25% full, resize it to half its current capacity (but not less than 2)
    if (capacity >= 4 && size() * 4 < capacity) {
        resize(capacity / 2);
    }
    
    return value;
}

// Clear implementation
void ArrayQueue::clear()
{
    front_idx = -1;
    rear_idx = -1;
    capacity = 2;
    resize(capacity);
}

// Size implementation
int ArrayQueue::size() const
{
    if (front_idx == -1) return 0;
    return front_idx <= rear_idx ? rear_idx - front_idx + 1
                                 : rear_idx + capacity - front_idx + 1;
}

// Front implementation
int ArrayQueue::front() const
{
    if (empty()) {
        // throw out_of_range("Queue is empty");
        cout << "Queue is empty" << endl;
        return -1;
    }
    return data[front_idx];
}

// Back implementation (get the element at the back of the queue)
int ArrayQueue::back() const
{
    if (empty()) {
        // throw out_of_range("Queue is empty");
        cout << "Queue is empty" << endl;
        return -1;
    }
    return data[rear_idx];
}

// Empty implementation
bool ArrayQueue::empty() const
{
    return size() == 0;
}

// Print implementation in the format: <elem1, elem2, ..., elemN|
string ArrayQueue::toString() const
{
    int count = size();
    string result = "<";
    for (int i = 0; i < count; i++)
    {
        int idx = (front_idx + i) % capacity;
        result += to_string(data[idx]);
        if (idx != rear_idx)
        {
            result += ", ";
        }
    }
    result += "|";
    return result;
}

// Resize implementation
void ArrayQueue::resize(int new_capacity)
{
    int *new_array = new int[new_capacity];

    int count = size();
    for (int i = 0; i < count; i++)
    {
        int idx = (front_idx + i) % capacity;
        new_array[i] = data[idx];
    }

    delete[] data;
    data = new_array;
    front_idx = count == 0 ? -1 : 0;
    rear_idx = count - 1;
    capacity = new_capacity;
}

int ArrayQueue::getCapacity() const
{
    return capacity;
}

// void ArrayQueue::debug_print() const {
//     cout << "Capacity: " << capacity << endl;
//     cout << "Size: " << size() << endl;
//     cout << "Front Index: " << front_idx << endl;
//     cout << "Rear Index: " << rear_idx << endl;
//     for (int i = 0; i < capacity; i++) {
//         cout << data[i] << ", ";
//     }
//     cout << endl;
//     cout << toString() << endl << endl;
// }