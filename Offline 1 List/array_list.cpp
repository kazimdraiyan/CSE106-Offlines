#include <iostream>
using namespace std;

class ArrayList
{
    int *array;
    int capacity;
    int length;
    int current_index;

    void init()
    {
        capacity = 2;
        array = new int[capacity];
        length = 0;
        current_index = -1; // Empty array
    }

    // Moves the elements to a new array of new_capacity
    void move_to_new_array(int new_capacity)
    {
        int *new_array = new int[new_capacity];
        for (int i = 0; i < length; i++)
        {
            new_array[i] = array[i];
        }
        delete[] array;
        array = new_array;
        capacity = new_capacity;
    }

    // Doubles the capacity if size >= half the capacity
    void increase_capacity()
    {
        if (length * 2 >= capacity)
        {
            cout << "Capacity increased from " << capacity << " to " << (capacity * 2) << endl;
            move_to_new_array(capacity * 2);
        }
    }

    // Halves the capacity if size < 25% of the capacity
    void decrease_capacity()
    {
        if (length * 4 < capacity)
        {
            cout << "Capacity decreased from " << capacity << " to " << (capacity / 2) << endl;
            move_to_new_array(capacity / 2);
        }
    }

    void move_current_index(int n)
    {
        if (length == 0)
            return;
        
        int new_index = current_index + n;
        if (new_index < 0)
            current_index = 0;
        else if (new_index >= length)
            current_index = length - 1;
        else
            current_index = new_index;
    }

    bool withinBoundary(int index)
    {
        return index >= 0 && index < length;
    }

public:
    ArrayList()
    {
        init();
    }

    ~ArrayList()
    {
        delete[] array;
    }

    void clear()
    {
        delete[] array;
        init();
    }

    int size()
    {
        return length;
    }

    bool is_present(int value)
    {
        for (int i = 0; i < length; i++)
        {
            if (array[i] == value)
                return true;
        }
        return false;
    }

    int find(int index)
    {
        if (!withinBoundary(index))
            return -1; // It is assumed that the list only contains non-negative integers.

        current_index = index;
        return array[index];
    }

    int update(int index, int value)
    {
        int old_element = find(index);
        if (old_element == -1)
            return -1; // Not a valid index

        array[index] = value;
        current_index = index;
        return old_element;
    }

    int search(int value)
    {
        for (int i = 0; i < length; i++)
        {
            if (array[i] == value)
            {
                current_index = i;
                return i;
            }
        }
        return -1;
    }

    void insert(int value)
    {
        increase_capacity();

        if (current_index < length - 1)
        {
            // Shifts the latter elements to the right by 1
            for (int i = length - 1; i > current_index; i--)
            {
                array[i + 1] = array[i];
            }
        }

        array[++current_index] = value;
        length++;
    }

    void append(int value)
    {
        if (length == 0)
            current_index = 0;

        increase_capacity();

        array[length++] = value;
    }

    int delete_cur()
    {
        if (length == 0)
            return -1;

        int deleted_elem = array[current_index];

        if (current_index < length - 1)
        {
            // Shifts the latter elements to the left by 1
            for (int i = current_index + 1; i < length; i++)
            {
                array[i - 1] = array[i];
            }
        }

        length--;
        if (current_index == length)
            current_index--;

        decrease_capacity();

        return deleted_elem;
    }

    int trim()
    {
        if (length == 0)
            return -1;

        if (current_index == length - 1)
            current_index--;

        int last = array[--length];

        decrease_capacity();

        return last;
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
        if (withinBoundary(index1) && withinBoundary(index2))
        {
            int temp = array[index1];
            array[index1] = array[index2];
            array[index2] = temp;
        }
    }

    void prev(int n)
    {
        move_current_index(-n);
    }

    void next(int n)
    {
        move_current_index(n);
    }

    void reverse()
    {
        for (int i = 0; i < length / 2; i++)
        {
            int temp = array[i];
            array[i] = array[length - i - 1];
            array[length - i - 1] = temp;
        }
    }

    void print()
    {
        if (length == 0)
            cout << "[ . ]" << endl;
        else
        {
            cout << "[ ";
            for (int i = 0; i < length; i++)
            {
                cout << array[i];
                if (i == current_index)
                    cout << "|";
                cout << " ";
            }
            cout << "]" << endl;
        }
    }
};