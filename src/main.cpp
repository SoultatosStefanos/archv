#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <cassert>
#include <cstdlib>
#include <iostream>

struct Vertex {
    std::string name;
};

struct Edge {
    int weight;
};

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, Vertex, Edge>;

auto main(int argc, char* argv[]) -> int
{
    try {
        Graph g;

        auto stef = boost::add_vertex({"Stefanos"}, g);
        auto soult = boost::add_vertex({"Soultatos"}, g);
        assert(boost::num_vertices(g) == 2);

        boost::add_edge(stef, soult, g);
        assert(boost::num_edges(g) == 1);
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