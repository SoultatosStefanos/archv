#include "graph_visualizer/clustering.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>

struct Vertex {
    int label;
};

inline auto operator<<(std::ostream& os, const Vertex& v) -> auto&
{
    return os << v.label;
}

struct Edge {
    int weight;
};

inline auto operator<<(std::ostream& os, const Edge& e) -> auto&
{
    return os << e.weight;
}

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, Vertex, Edge>;

void output(const Graph& g, std::ostream& os = std::cout)
{
    auto [vfirst, vlast] = boost::vertices(g);

    for (auto iter = vfirst; iter != vlast; ++iter) {
        os << "vertex: " << g[*iter];

        os << ", edges: ";
        auto [efirst, elast] = boost::out_edges(*iter, g);
        for (auto iter = efirst; iter != elast; ++iter) os << " " << g[*iter];

        os << '\n';
    }
}

auto main() -> int
{
    try {
        using namespace GV;

        Graph g;

        auto v1 = boost::add_vertex({1}, g);
        auto v2 = boost::add_vertex({2}, g);
        auto v3 = boost::add_vertex({3}, g);
        auto v4 = boost::add_vertex({4}, g);
        auto v5 = boost::add_vertex({5}, g);

        boost::add_edge(v1, v2, {7}, g);
        boost::add_edge(v1, v3, {2}, g);
        boost::add_edge(v1, v4, {4}, g);
        boost::add_edge(v2, v3, {3}, g);
        boost::add_edge(v2, v4, {5}, g);
        boost::add_edge(v3, v4, {2}, g);
        boost::add_edge(v3, v5, {6}, g);
        boost::add_edge(v4, v5, {4}, g);

        std::cout << "\nBefore: \n";
        output(g);

        // Clustering::k_spanning_tree(
        //     g, 3,
        //     [](const auto& g) {
        //         using VertexDescriptor = Graph::vertex_descriptor;
        //         using PredecessorMap = std::map<VertexDescriptor, VertexDescriptor>;

        //         Graph mst;

        //         PredecessorMap p_map;

        //         boost::prim_minimum_spanning_tree(g, boost::associative_property_map(p_map));

        //         return mst;
        //     },
        //     boost::get(&Edge::weight, g));

        // Clustering::k_spanning_tree(
        //     g, 3,
        //     [](const auto& g) {
        //         Graph mst;

        //         auto v1 = boost::add_vertex({1}, mst);
        //         auto v2 = boost::add_vertex({2}, mst);
        //         auto v3 = boost::add_vertex({3}, mst);
        //         auto v4 = boost::add_vertex({4}, mst);
        //         auto v5 = boost::add_vertex({5}, mst);

        //         boost::add_edge(v1, v3, {2}, mst);
        //         boost::add_edge(v2, v3, {3}, mst);
        //         boost::add_edge(v3, v4, {2}, mst);
        //         boost::add_edge(v4, v5, {4}, mst);

        //         return mst;
        //     },
        //     boost::get(&Edge::weight, g));

        std::cout << "\nAfter: \n";
        output(g);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';

        return EXIT_FAILURE;
    }
    catch (...) {
        std::cerr << "Unexpected error!\n";

        return EXIT_FAILURE;
    }
}