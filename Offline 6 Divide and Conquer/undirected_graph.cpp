#include <unordered_map>
#include <vector>

using namespace std;

class UndirectedGraph
{
    unordered_map<int, vector<int>> adj_list;
    unordered_map<int, bool> is_visited;

    // Assumes the passed nodes exist in the graph
    bool is_connected_recursive(int node1, int node2)
    {
        if (node1 == node2)
            return true;

        is_visited[node1] = true;
        for (int neighbor : adj_list[node1])
        {
            if (!is_visited[neighbor] && is_connected_recursive(neighbor, node2))
                return true;
        }
        return false;
    }

public:
    // O(1)
    void add_edge(int node1, int node2)
    {
        adj_list[node1].push_back(node2);
        adj_list[node2].push_back(node1);
        is_visited[node1] = false;
        is_visited[node2] = false;
    }

    // O(n)
    bool is_connected(int node1, int node2)
    {
        if (adj_list.count(node1) && adj_list.count(node2)) {
            // If both nodes exist in the graph
            
            // Set every node as unvisited
            for (auto &entry : is_visited) {
                entry.second = false;
            }

            return is_connected_recursive(node1, node2);
        }
        return false;
    }

    // void debug_print() {
    //     cout << "Adjacency List:" << endl;
    //     for (auto entry : adj_list) {
    //         cout << entry.first << ": [";
    //         for (int node : entry.second) {
    //             cout << " " << node;
    //         }
    //         cout << " ]" << endl;
    //     }
    //     cout << endl;
    // }
};