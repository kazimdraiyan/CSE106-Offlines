#include <iostream>
#include <fstream>

#include "graph.cpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Error: enter filename in the command" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file)
    {
        cerr << "Unable to open file: " << argv[1];
        return 1;
    }

    int n;
    file >> n;

    UndirectedGraph undirected_graph(n);

    char ch; // For dumping irrelevant input characters
    int country1, country2;

    // Line format: [1, 2]
    while (file >> ch >> country1 >> ch >> country2 >> ch)
    {
        pair<int, int> edge(country1, country2);
        undirected_graph.add_edge(edge);
    }

    vector<vector<int>> groups = undirected_graph.find_components();
    int i = 1;
    for (vector<int> group : groups)
    {
        // Print group
        cout << "Group " << i << ": {";
        for (int country : group)
        {
            cout << country;
            if (group.back() != country)
               cout << ", ";
        }
        cout << "} | ";

        // Print matches that are yet to be played
        bool has_any_unplayed_match = false;
        int group_size = group.size();
        // Check every possible pair of contries from the group
        for (int j = 0; j < group_size - 1; j++) {
            for (int k = j + 1; k < group_size; k++) {
                if (!undirected_graph.has_edge(group[j], group[k])) {
                    // The current pair of countries doesn't have an edge in between them in the graph, so it is an unplayed match

                    if (has_any_unplayed_match)
                        cout << ", ";
                    else
                        has_any_unplayed_match = true;
                    
                    cout << "[" << group[j] << ", " << group[k] << "]";
                }
            }
        }
        if (!has_any_unplayed_match) {
            cout << "none";
        }

        cout << endl;
        i++;
    }
}
