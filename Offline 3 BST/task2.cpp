#include <iostream>
#include <fstream>
#include <string>
#include "listBST.hpp"
using namespace std;

class Item
{
    string name;
    int current_bid_amount;
    int successful_bid_count;
    int rejected_bid_count;

public:
    Item(string name, int initial_bid_amount)
    {
        this->name = name;
        current_bid_amount = initial_bid_amount;
        successful_bid_count = 0;
        rejected_bid_count = 0;
    }

    bool bid(int bid_amount)
    {
        if (bid_amount > current_bid_amount)
        {
            current_bid_amount = bid_amount;
            successful_bid_count++;
            return true;
        }
        else
        {
            rejected_bid_count++;
            return false;
        }
    }

    int get_current_bid_amount() const
    {
        return current_bid_amount;
    }

    int get_successful_bid_count() const
    {
        return successful_bid_count;
    }

    int get_rejected_bid_count() const
    {
        return rejected_bid_count;
    }

    int get_total_bid_count() const
    {
        return successful_bid_count + rejected_bid_count;
    }

    void print_stats(bool compact = false)
    {
        if (compact)
        {
            cout << name << ": Current bid: " << current_bid_amount << ", Total bids: " << get_total_bid_count() << ", Successful: " << successful_bid_count << ", Rejected: " << rejected_bid_count << endl;
        }
        else
        {
            cout << "Statistics for " << name << ":" << endl;
            cout << "  Current highest bid: " << current_bid_amount << endl;
            cout << "  Total bids placed: " << get_total_bid_count() << endl;
            cout << "  Successful bids: " << successful_bid_count << endl;
            cout << "  Rejected bids: " << rejected_bid_count << endl;
        }
    }
};

// Print behavior of Item *
ostream &operator<<(ostream &os, const Item *item)
{
    if (!item)
        return os << "Null Item";
    return os << (item->get_current_bid_amount());
}

void print_items(BST<string, Item *> *items_bst)
{
    cout << "BST (In-order): ";
    items_bst->print('I');
    cout << endl;
}

void add_new_item(ifstream &in_file, BST<string, Item *> *items, bool print = true)
{
    string item_name;
    in_file >> item_name;
    int initial_bid_amount;
    in_file >> initial_bid_amount;

    if (items->find(item_name))
    {
        cout << "Cannot add item! Item already exists." << endl;
        return;
    }

    Item *item = new Item(item_name, initial_bid_amount);

    items->insert(item_name, item);
    if (print)
    {
        cout << "Item " << item_name << " added with starting bid " << initial_bid_amount << endl;
        print_items(items);
    }
}

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
        return 1;
    }

    BST<string, Item *> *items = new ListBST<string, Item *>();

    int n;
    in_file >> n;
    for (int i = 0; i < n; ++i)
    {
        // Initialize statistics tracking for each item
        add_new_item(in_file, items, false);
    }

    cout << "Initial auction items:" << endl;
    print_items(items);

    cout << "\nAuction starts!\n\n";
    cout << "==============================\n";

    string operation;
    string item_name;
    int bid_amount;
    while (in_file >> operation)
    {
        // For BID operations: update statistics (total bids, successful/rejected counts)
        // For STATS operations: display statistics for the specified item
        // For REPORT operations: display comprehensive auction statistics
        // For other operations: print auction state using in-order traversal

        if (operation == "ADD")
        {
            add_new_item(in_file, items);
        }
        else if (operation == "BID")
        {
            in_file >> item_name;
            in_file >> bid_amount;
            try
            {
                Item *item = items->get(item_name);
                if (item->bid(bid_amount))
                    cout << "Bid of " << bid_amount << " on " << item_name << " accepted. Current bid: " << bid_amount << endl;
                else
                    cout << "Bid of " << bid_amount << " on " << item_name << " rejected. Current bid: " << item->get_current_bid_amount() << endl;
                print_items(items);
            }
            catch (const runtime_error &e)
            {
                cout << "Item not found." << endl;
            }
        }
        else if (operation == "CHECK")
        {
            in_file >> item_name;
            try
            {
                Item *item = items->get(item_name);
                cout << "Current bid for " << item_name << ": " << item->get_current_bid_amount() << endl;
                print_items(items);
            }
            catch (const runtime_error &e)
            {
                cout << "Item not found." << endl;
            }
        }
        else if (operation == "STATS")
        {
            in_file >> item_name;
            try
            {
                Item *item = items->get(item_name);
                item->print_stats();
            }
            catch (const runtime_error &e)
            {
                cout << "Item not found." << endl;
            }
        }
        else if (operation == "REPORT")
        {
            cout << "Auction Report: " << endl;

            BST<string, Item *> *items_temp = new ListBST<string, Item *>();

            // Move each item to items_temp in sorted order
            int total_successful_bids = 0;
            int total_rejected_bids = 0;
            while (!items->empty())
            {
                string item_name = items->find_min(); // For ascending order
                Item *item = items->get(item_name);

                total_successful_bids += item->get_successful_bid_count();
                total_rejected_bids += item->get_rejected_bid_count();

                items_temp->insert(item_name, item);
                items->remove(item_name);
            }
            cout << "Total items: " << items_temp->size() << endl;
            cout << "Total bids placed: " << total_successful_bids + total_rejected_bids << endl;
            cout << "Total successful bids: " << total_successful_bids << endl;
            cout << "Total rejected bids: " << total_rejected_bids << endl;

            // Move each item back to items and print its details
            cout << endl
                 << "Item Statistics:" << endl;
            while (!items_temp->empty())
            {
                string item_name = items_temp->find_min();
                Item *item = items_temp->get(item_name);

                cout << "  ";
                item->print_stats(true);

                items->insert(item_name, item);
                items_temp->remove(item_name);
            }

            delete items_temp;
        }
        else
        {
            cout << "Invalid operation!" << endl;
        }
        cout << "==============================\n";
    }

    in_file.close();

    // Free memory
    while (!items->empty())
    {
        string item_name = items->find_min();
        Item *item = items->get(item_name);
        items->remove(item_name);
        delete item;
    }
    delete items;

    return 0;
}
