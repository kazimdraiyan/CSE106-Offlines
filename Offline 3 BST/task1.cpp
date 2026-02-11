#include <iostream>
#include <fstream>
#include "listBST.hpp"

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "Usage: filename" << "\n";
        return 1;
    }
    ifstream in_file(argv[1]);
    if (!in_file)
    {
        cerr << "Unable to open file\n";
        return 2;
    }
    char c, str[5];
    int val;
    BST<int, int> *bst = new ListBST<int, int>();
    if (!bst)
    {
        cerr << "Memory allocation failed\n";
        return 3;
    }
    while (in_file >> c)
    {
        if (c == 'F')
        {
            in_file >> val;
            cout << "Key " << val << (bst->find(val) ? "" : " not") << " found in BST." << endl;
        }
        else if (c == 'E')
        {
            if (bst->empty())
            {
                cout << "Empty" << endl;
            }
            else
            {
                cout << "Not empty" << endl;
            }
        }
        else if (c == 'I')
        {
            in_file >> val;
            bool success = bst->insert(val, val);
            if (success)
            {
                cout << "Key " << val << " inserted into BST, ";
            }
            else
            {
                cout << "Insertion failed! Key " << val << " already exists in BST, ";
            }
            cout << "BST (Default): ";
            bst->print();
            cout << endl;
        }
        else if (c == 'M')
        {
            in_file >> str;
            if (string(str) == "Max")
            {
                try
                {
                    cout << "Maximum value: " << bst->find_max() << endl;
                }
                catch (const runtime_error &e)
                {
                    cout << "Maximum value not found! BST is empty." << endl;
                }
            }
            else if (string(str) == "Min")
            {
                try
                {
                    cout << "Minimum value: " << bst->find_min() << endl;
                }
                catch (const runtime_error &e)
                {
                    cout << "Minimum value not found! BST is empty." << endl;
                }
            }
        }
        else if (c == 'D')
        {
            in_file >> val;
            bool success = bst->remove(val);
            if (success)
            {
                cout << "Key " << val << " removed from BST, ";
            }
            else
            {
                cout << "Removal failed! Key " << val << " not found in BST, ";
            }
            cout << "BST (Default): ";
            bst->print();
            cout << endl;
        }
        else if (c == 'T')
        {
            in_file >> str;
            if (string(str) == "Pre")
            {
                cout << "BST (Pre-order): ";
                bst->print('P');
            }
            else if (string(str) == "In")
            {
                cout << "BST (In-order): ";
                bst->print('I');
            }
            else if (string(str) == "Post")
            {
                cout << "BST (Post-order): ";
                bst->print('O');
            }
            else
            {
                cout << "Invalid traversal mode.";
            }
            cout << endl;
        }
        else if (c == 'S')
        {
            cout << "Size: " << bst->size() << endl;
        }
        else
        {
            cout << "Invalid operation" << endl;
        }
    }
    in_file.close();
    delete bst;
    return 0;
}
