#include <iostream>
#include <vector>

using namespace std;

class UndirectedGraph
{
    int n; // Number of nodes
    vector<int> *adj_list;
    bool *visited_nodes;
    int visited_node_count;

    bool all_nodes_visited()
    {
        return visited_node_count == n;
    }

    int first_unvisited_node()
    {
        for (int i = 0; i < n; i++)
        {
            if (!visited_nodes[i])
                return i;
        }
        return -1;
    }

public:
    UndirectedGraph(int n)
    {
        this->n = n;
        adj_list = new vector<int>[n]; // n empty vectors are initialized

        visited_nodes = new bool[n];
        for (int i = 0; i < n; i++)
        {
            visited_nodes[i] = false;
        }
        visited_node_count = 0;
    }

    ~UndirectedGraph()
    {
        delete[] adj_list;
        delete[] visited_nodes;
    }

    void add_edge(pair<int, int> edge)
    {
        adj_list[edge.first].push_back(edge.second);
        adj_list[edge.second].push_back(edge.first);
    }

    bool has_edge(int node1, int node2) {
        for (int neighbor : adj_list[node1]) {
            if (neighbor == node2) {
                return true;
            }
        }
        return false;
    }

    // If the graph is disconnected, returns the collection of nodes of the connected subgraphs of the graph.
    // If the graph is connected, returns one collection containing all nodes.
    vector<vector<int>> find_components()
    {
        vector<vector<int>> components;

        while (!all_nodes_visited())
        {
            vector<int> cluster; // Collection of all nodes of a connected subgraph (component)

            // Visit the first unvisited node (depth-first)
            cluster_connected_nodes(first_unvisited_node(), cluster);

            components.push_back(cluster);
        }

        return components;
    }

    // Given a root node, visit every node that is connected to the root node depth-first and add to cluster
    void cluster_connected_nodes(int node, vector<int> &cluster)
    {
        if (visited_nodes[node])
            return;

        // Visit current node
        cluster.push_back(node);
        visited_nodes[node] = true;
        visited_node_count++;

        for (int neighbor : adj_list[node])
        {
            if (!visited_nodes[neighbor])
            {
                // Visit the neighbor (depth-first)
                cluster_connected_nodes(neighbor, cluster);
            }
        }
    }

    void print_adj_list()
    {
        for (int i = 0; i < n; i++)
        {
            cout << i << ": ";

            cout << "[";
            for (int node : adj_list[i])
            {
                cout << node;
                if (adj_list[i].back() != node)
                    cout << ", ";
            }
            cout << "]" << endl;
        }
    }
};