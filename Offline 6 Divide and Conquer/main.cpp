#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>

#include "city.cpp"
#include "undirected_graph.cpp"

using namespace std;

typedef vector<City *>::iterator city_iterator;
typedef pair<City *, City *> city_pair;

// O(1)
bool compare_cities_by_x(const City *city1, const City *city2)
{
    return city1->x < city2->x;
}

// O(1)
bool compare_edges_by_cost(const city_pair &edge1, const city_pair &edge2)
{
    double edge1_cost = edge1.first->distance_from(*edge1.second);
    double edge2_cost = edge2.first->distance_from(*edge2.second);
    return edge1_cost < edge2_cost;
}

// O(m^3)
double connect_cities_naively(city_iterator begin, city_iterator end, vector<city_pair> &edges)
{
    vector<city_pair> all_edges;

    // O(m^2)
    for (auto i = begin; i != end; i++)
    {
        for (auto j = i + 1; j != end; j++)
        {
            city_pair edge(*i, *j);
            all_edges.push_back(edge);
        }
    }

    // all_edges now contain mC2 edges, which is O(m^2)

    // O(m^2 logm)
    sort(all_edges.begin(), all_edges.end(), compare_edges_by_cost);

    double total_cost = 0;
    UndirectedGraph city_graph;
    // O(m^3) because O(m) for each edge of the O(m^2) edges
    for (city_pair &edge : all_edges)
    {
        // O(m)
        if (!city_graph.is_connected(edge.first->id, edge.second->id))
        {
            city_graph.add_edge(edge.first->id, edge.second->id);
            edges.push_back(edge);
            total_cost += edge.first->distance_from(*edge.second);
        }
    }
    return total_cost;
}

// O(m^2)
double connect_split_cities(city_iterator mid, vector<city_pair> &edges, int depth) {
    if (depth <= 0) {
        throw invalid_argument("Depth must be a positive integer.");
    }

    double min_distance = INFINITY;
    city_pair min_distance_edge;
    for (auto i = mid - depth; i != mid; i++) {
        for (auto j = mid; j != mid + depth; j++) {
            double distance = (*i)->distance_from(*(*j));
            if (distance < min_distance) {
                min_distance = distance;
                city_pair edge(*i, *j);
                min_distance_edge = edge;
            }
        }
    }
    edges.push_back(min_distance_edge);
    return min_distance;
}

// T(n) = 2T(n/2) + O(m^2), if n > m; O(n^3), if n <= m. Because of m being a small constant, T(n) = 2T(n/2) + O(1), therefore O(n)
double connect_cities(city_iterator begin, city_iterator end, vector<city_pair> &edges, int base_case_threshold)
{
    int size = end - begin;

    if (size <= base_case_threshold)
    {
        return connect_cities_naively(begin, end, edges);
    }

    auto mid = begin + size / 2;
    double first_half_cost = connect_cities(begin, mid, edges, base_case_threshold);
    double last_half_cost = connect_cities(mid, end, edges, base_case_threshold);

    double merging_cost = connect_split_cities(mid, edges, min(size / 2, base_case_threshold)); // Maximum depth is base_case_threshold (m)

    return first_half_cost + last_half_cost + merging_cost;
}

int main()
{
    ifstream file("sample_io/input1.txt");

    if (!file) {
        cout << "Cannot open input file." << endl;
        return 1;
    }

    int n, base_case_threshold;
    file >> n >> base_case_threshold;

    vector<City *> cities;
    vector<city_pair> edges;

    int id;
    double x, y;
    // O(n)
    for (int i = 0; i < n; i++)
    {
        file >> id >> x >> y;
        cities.push_back(new City(id, x, y));
    }

    // O(nlogn)
    sort(cities.begin(), cities.end(), compare_cities_by_x);

    double total_cost = connect_cities(cities.begin(), cities.end(), edges, base_case_threshold);
    cout << "Total Cost: " << fixed << setprecision(2) << total_cost << endl; // TODO: What does "fixed" do?
    cout << "Edges:" << endl;
    // O(n), because number of edges in the optimally connected graph will be n-1
    for (city_pair edge : edges)
    {
        cout << edge.first->id << " " << edge.second->id << endl;
    }

    // Free memory
    for (City *city : cities) {
        delete city;
    }

    return 0;
}
