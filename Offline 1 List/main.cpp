#include <iostream>
#include "array_list.cpp"
#include "linked_list.cpp"

using namespace std;

int main()
{
    FILE *file = fopen("input.txt", "r");
    if (file == NULL)
        return 1;

    ArrayList list;
    // LinkedList list;

    int func, arg1, arg2;
    while (fscanf(file, "%d", &func) == 1 && func != 0)
    {
        if (func == 1 && fscanf(file, "%d", &arg1) == 1)
        {
            cout << "Insert " << arg1 << endl;
            list.insert(arg1);
            list.print();
        }
        else if (func == 2)
        {
            cout << "Delete current item" << endl;
            int deleted_elem = list.delete_cur();
            if (deleted_elem != -1)
            {
                list.print();
                cout << deleted_elem << " is deleted" << endl;
            }
            else
                cout << "List is empty" << endl;
        }
        else if (func == 3 && fscanf(file, "%d", &arg1) == 1)
        {
            cout << "Append " << arg1 << endl;
            list.append(arg1);
            list.print();
        }
        else if (func == 4)
        {
            cout << "Size of the list is " << list.size() << endl;
        }
        else if (func == 5 && fscanf(file, "%d", &arg1) == 1)
        {
            cout << "Prev " << arg1 << endl;
            list.prev(arg1);
            list.print();
        }
        else if (func == 6 && fscanf(file, "%d", &arg1) == 1)
        {
            cout << "Next " << arg1 << endl;
            list.next(arg1);
            list.print();
        }
        else if (func == 7 && fscanf(file, "%d", &arg1) == 1)
        {
            int is_present = list.is_present(arg1);
            if (is_present)
                cout << arg1 << " is present" << endl;
            else
                cout << arg1 << " is not present" << endl;
        }
        else if (func == 8)
        {
            cout << "Clear list" << endl;
            list.clear();
            list.print();
        }
        else if (func == 9 && fscanf(file, "%d", &arg1) == 1)
        {
            cout << "Delete " << arg1 << endl;
            bool is_succesful = list.delete_item(arg1);
            if (is_succesful)
                list.print();
            else
                cout << arg1 << " not found" << endl;
        }
        else if (func == 10 && fscanf(file, "%d", &arg1) == 1 && fscanf(file, "%d", &arg2) == 1)
        {
            cout << "Swap index " << arg1 << " and " << arg2 << endl;
            list.swap_ind(arg1, arg2);
            list.print();
        }
        else if (func == 11 && fscanf(file, "%d", &arg1) == 1)
        {
            cout << "Search " << arg1 << endl;
            int index = list.search(arg1);
            if (index >= 0)
            {
                list.print();
                cout << arg1 << " is found at " << index << endl;
            }
            else
                cout << arg1 << " is not found" << endl;
        }
        else if (func == 12 && fscanf(file, "%d", &arg1) == 1)
        {
            cout << "Find " << arg1 << endl;
            int elem = list.find(arg1);
            if (elem != -1)
            {
                list.print();
                cout << elem << " is found at " << arg1 << endl;
            }
            else
                cout << arg1 << " is not a valid index" << endl;
        }
        else if (func == 13 && fscanf(file, "%d", &arg1) == 1 && fscanf(file, "%d", &arg2) == 1)
        {
            cout << "Update element at " << arg1 << endl;
            int old_elem = list.update(arg1, arg2);
            if (old_elem != -1)
            {
                list.print();
                cout << old_elem << " is updated by " << arg2 << endl;
            }
            else
                cout << arg1 << " is not a valid index" << endl;
        }
        else if (func == 14)
        {
            cout << "Trim" << endl;
            int deleted_last_elem = list.trim();
            if (deleted_last_elem != -1)
            {
                list.print();
                cout << deleted_last_elem << " removed" << endl;
            }
            else
                cout << "List is empty" << endl;
        }
        else if (func == 15)
        {
            cout << "Reverse" << endl;
            list.reverse();
            list.print();
        }
    }
    cout << "End" << endl;

    return 0;
}

/*
Operations:
1. Insert
2. Delete current element
3. Append
4. Size
5. Prev
6. Next
7. Is present
8. Clear
9. Delete element
10. Swap index
11. Search
12. Find (array access)
13. Update element
14. Trim
15. Reverse
*/