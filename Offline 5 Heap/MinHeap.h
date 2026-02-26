#ifndef MINHEAP_H
#define MINHEAP_H

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <climits>

#define MAX_CAPACITY 1000 // Defines the maximum capacity of the heap

class MinHeap
{
private:
    int heap[MAX_CAPACITY]; // Array to store heap elements
    int size;               // Current number of elements in the heap

    int parentOf(int i)
    {
        if (i == 0)
            return -1; // There's no parent of the root element
        return (i - 1) / 2;
    }

    int leftChildOf(int i)
    {
        return 2 * i + 1;
    }

    int rightChildOf(int i)
    {
        return 2 * i + 2;
    }

    int smallerChildOf(int i)
    {
        int leftChildIndex = leftChildOf(i);
        if (!withinRange(leftChildIndex))
            return -1; // The element has no child

        int rightChildIndex = rightChildOf(i);
        if (!withinRange(rightChildIndex))
            return leftChildIndex; // The element has only left child

        return heap[leftChildIndex] <= heap[rightChildIndex] ? leftChildIndex : rightChildIndex;
    }

    int lastInternalNodeIndex()
    {
        return size / 2 - 1; // If empty or size = 1, -1 is returned as there is no internal node in these cases.
    }

    void swapValues(int i, int j)
    {
        int temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    };

    // Checks whether the given index is valid
    bool withinRange(int i)
    {
        return i >= 0 && i < size;
    }

    /**
     * Sifts up the node at index i to maintain heap property.
     */
    void siftUp(int i)
    {
        int parentIndex = parentOf(i);

        while (withinRange(parentIndex) && heap[i] < heap[parentIndex])
        {
            swapValues(i, parentIndex);
            i = parentIndex;
            parentIndex = parentOf(i);
        }
    }

    /**
     * Sifts down the node at index i to maintain heap property.
     */
    void siftDown(int i)
    {
        int smallerChildIndex = smallerChildOf(i);

        while (withinRange(smallerChildIndex) && heap[smallerChildIndex] < heap[i])
        {
            swapValues(i, smallerChildIndex);
            i = smallerChildIndex;
            smallerChildIndex = smallerChildOf(i);
        }
    }

    bool isValidMinHeapRecursively(int rootIndex)
    {
        if (!withinRange(rootIndex))
            return true; // Empty heap, so vacuously true

        int leftChildIndex = leftChildOf(rootIndex);
        if (withinRange(leftChildIndex) && heap[rootIndex] > heap[leftChildIndex])
            return false;

        int rightChildIndex = rightChildOf(rootIndex);
        if (withinRange(rightChildIndex) && heap[rootIndex] > heap[rightChildIndex])
            return false;
        
        return isValidMinHeapRecursively(leftChildIndex) && isValidMinHeapRecursively(rightChildIndex);
    }

public:
    // Constructor initializes an empty heap
    MinHeap() : size(0) {}

    /**
     * Inserts a new element x into the heap.
     */
    void insert(int x)
    {
        if (size == MAX_CAPACITY)
        {
            throw std::runtime_error("Insufficient capacity. Cannot insert element.");
        }

        heap[size] = x;
        size++;
        siftUp(size - 1);
    }

    /**
     * Returns the minimum element without removing it.
     */
    int findMin()
    {
        if (size == 0)
        {
            throw std::runtime_error("Heap is empty.");
        }

        return heap[0];
    }

    /**
     * Removes and returns the minimum element from the heap.
     */
    int extractMin()
    {
        int min = findMin();

        heap[0] = heap[--size]; // Replacing root with the last element
        siftDown(0);

        return min;
    }

    /**
     * Returns the number of elements in the heap.
     */
    int getSize()
    {
        return size;
    }

    /**
     * Checks if the heap is empty.
     * Returns true if empty, false otherwise.
     */
    bool isEmpty()
    {
        return size == 0;
    }

    /**
     * Decreases the value of the element at index i to newValue.
     */
    void decreaseKey(int i, int newValue)
    {
        if (!withinRange(i))
        {
            throw std::out_of_range("Index out of bound.");
        }

        if (newValue >= heap[i])
        {
            throw std::invalid_argument("New value must be less than the current value.");
        }

        heap[i] = newValue; // newValue < heap[i]
        siftUp(i);
    }

    /**
     * Deletes the element at index i.
     */
    void deleteKey(int i)
    {
        if (!withinRange(i))
        {
            throw std::out_of_range("Index out of bound.");
        }

        decreaseKey(i, INT_MIN);
        extractMin();
    }

    /**
     * Prints the heap's content to the output file.
     * Format: "elem1 elem2 elem3 ..." (space-separated)
     */
    void printHeap(std::ofstream &outfile)
    {
        if (size == 0)
        {
            outfile << "Empty heap" << std::endl;
            return;
        }

        for (int i = 0; i < size; i++)
        {
            outfile << heap[i];
            if (i != size - 1)
                outfile << " ";
        }
        outfile << std::endl;
    }

    /**
     * Checks whether the Min Heap property is preserved.
     * Returns true if valid, false otherwise.
     */
    bool isValidMinHeap()
    {
        return isValidMinHeapRecursively(0);
    }

    /**
     * Builds a heap from an unsorted array using bottom-up heapify.
     */
    void heapify(int arr[], int n)
    {
        if (n < 0)
        {
            throw std::invalid_argument("Invalid size.");
        }
        if (n > MAX_CAPACITY)
        {
            throw std::runtime_error("Insufficient capacity. Cannot heapify.");
        }

        for (int i = 0; i < n; i++)
        {
            heap[i] = arr[i];
        }
        size = n;

        for (int i = lastInternalNodeIndex(); i >= 0; i--)
        {
            siftDown(i);
        }
    }

    /**
     * Returns all elements in sorted (ascending) order.
     * The original heap should remain UNCHANGED after this operation.
     */
    void heapSort(std::ofstream &outfile)
    {
        MinHeap heapCopy;
        heapCopy.heapify(heap, size);

        for (int i = 0; i < size; i++)
        {
            outfile << heapCopy.extractMin();
            if (i != size - 1)
                outfile << " ";
        }
        outfile << std::endl;
    }

    /**
     * Replaces the minimum element with a new value x in a single operation.
     * Returns the old minimum value.
     */
    int replaceMin(int x)
    {
        if (size == 0)
        {
            throw std::runtime_error("Heap is empty.");
        }

        int val = heap[0];

        heap[0] = x;
        siftDown(0);

        return val;
    }
};

#endif // MINHEAP_H
